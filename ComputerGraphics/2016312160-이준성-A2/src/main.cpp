#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "sphere.h"		// sphere class definition

//*************************************
// global constants
static const char*	window_name = "cgbase - sphere";
static const char*	vert_shader_path = "../bin/shaders/circ.vert";
static const char*	frag_shader_path = "../bin/shaders/circ.frag";
static const uint	MIN_TESS = 3;		// minimum tessellation factor (down to a triangle)
static const uint	MAX_TESS = 256;		// maximum tessellation factor (up to 256 triangles)
uint				NUM_TESS = 72;		// initial tessellation factor of the sphere as a polygon


//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint	program = 0;		// ID holder for GPU program
GLuint	vertex_array = 0;	// ID holder for vertex array object

//*************************************
// global variables
int		frame = 0;						// index of rendering frames
float	t = 0.0f;						// current simulation parameter
bool	b_solid_color = false;			// use sphere's color?
bool	b_tc_xy = true;
bool	b_tc_xxx = true;
bool	b_rotate = false;
bool	b_index_buffer = true;			// use index buffering?
#ifndef GL_ES_VERSION_2_0
bool	b_wireframe = false;
#endif
auto	spheres = std::move(create_spheres());
struct { bool add=false, sub=false; operator bool() const { return add||sub; } } b; // flags of keys for smooth changes

//*************************************
// holder of vertices and indices of a unit sphere
std::vector<vertex>	unit_sphere_vertices;	// host-side vertices
std::vector<uint>	indices;

//*************************************
void update()
{
	// update global simulation parameter
	t = float(glfwGetTime())*0.4f;

	// tricky aspect correction matrix for non-square window
	float aspect = window_size.x/float(window_size.y);
	mat4 aspect_matrix =
	{
		min(1 / aspect,0.5625f), 0, 0, 0,
		0, min(aspect * 0.5625f,1.0f), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	// update common uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation( program, "b_solid_color" );	if(uloc>-1) glUniform1i( uloc, b_solid_color );
	uloc = glGetUniformLocation(program, "b_tc_xy");	if (uloc > -1) glUniform1i(uloc, b_tc_xy);
	uloc = glGetUniformLocation(program, "b_tc_xxx");	if (uloc > -1) glUniform1i(uloc, b_tc_xxx);
	uloc = glGetUniformLocation( program, "aspect_matrix" );	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, aspect_matrix );

	mat4 view_projection_matrix = { 0,1,0,0,0,0,1,0,-1,0,0,1,0,0,0,1 };
	uloc = glGetUniformLocation(program, "view_projection_matrix");
	glUniformMatrix4fv(uloc, 1, GL_TRUE, view_projection_matrix);

	// update vertex buffer by the pressed keys
	void update_tess(); // forward declaration
	if(b) update_tess(); 
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// notify GL that we use our own program
	glUseProgram( program );

	// bind vertex array object
	glBindVertexArray( vertex_array );

	// render two spheres: trigger shader program to process vertex data
	for( auto& s : spheres )
	{
		// per-sphere update
		s.update(t,b_rotate);

		// update per-sphere uniforms
		GLint uloc;
		uloc = glGetUniformLocation( program, "solid_color" );		if(uloc>-1) glUniform4fv( uloc, 1, s.color );	// pointer version
		uloc = glGetUniformLocation( program, "model_matrix" );		if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, s.model_matrix );

		// per-sphere draw calls
		if(b_index_buffer)	glDrawElements( GL_TRIANGLES, (uint)indices.size(), GL_UNSIGNED_INT, nullptr );
		else				glDrawArrays( GL_TRIANGLES, 0, NUM_TESS*3 ); // NUM_TESS = N
	}

	// swap front and back buffers, and display to screen
	glfwSwapBuffers( window );
}

void reshape( GLFWwindow* window, int width, int height )
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

void print_help()
{
	printf( "[help]\n" );
	printf( "- press ESC or 'q' to terminate the program\n" );
	printf( "- press F1 or 'h' to see help\n" );
	printf( "- press 'd' to toggle between texture coordinates\n" );
	printf( "- press '+/-' to increase/decrease tessellation factor (min=%d, max=%d)\n", MIN_TESS, MAX_TESS );
	printf( "- press 'i' to toggle between index buffering and simple vertex buffering\n" );
	printf("-press 'r' to rotate or not\n");
#ifndef GL_ES_VERSION_2_0
	printf( "- press 'w' to toggle wireframe\n" );
#endif
	printf( "\n" );
}

std::vector<vertex> create_sphere_vertices( uint N )
{
	//std::vector<vertex> v = { { vec4(0,0,0,1), vec3(0,0,1.0f), vec2(0,1.0f) } }; // origin
	std::vector<vertex> v;
	float dp = 2 * PI / N;
	float dt = PI / (N / 2);
	for (uint l = 0; l <= N/2; l++) 
	{
		float t = l * dt, ct = cos(t), st = sin(t);
		for( uint k = 0; k <= N; k++ )
		{
			float p = k * dp, cp = cos(p), sp = sin(p);
			v.push_back( {vec3(1.0f*st * cp, 1.0f*st * sp,1.0f* ct), vec3(st * cp, st * sp, ct), vec2(p / (2 * PI), 1-(t / PI))});
		}
	}
	return v;
}

void update_vertex_buffer( const std::vector<vertex>& vertices, uint N )
{
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if(vertex_buffer)	glDeleteBuffers( 1, &vertex_buffer );	vertex_buffer = 0;
	if(index_buffer)	glDeleteBuffers( 1, &index_buffer );	index_buffer = 0;

	// check exceptions
	if(vertices.empty()){ printf("[error] vertices is empty.\n"); return; }

	// create buffers
	if(b_index_buffer)
	{
		for (uint l = 0; l < N / 2; l++)
		{
			uint k1 = l * (N + 1);
			uint k2 = k1 + N + 1;
			for( uint k=0; k < N; k++,k1++,k2++)
			{
				if (l != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if (l != N / 2 - 1) {
					indices.push_back(k1+1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		// generation of vertex buffer: use vertices as it is
		glGenBuffers( 1, &vertex_buffer );
		glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// geneation of index buffer
		glGenBuffers( 1, &index_buffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), &indices[0], GL_STATIC_DRAW );
	}
	else
	{
		std::vector<vertex> v; // triangle vertices
		for( uint k=0; k < N ; k++ )
		{
			v.push_back(vertices[0]);	// the origin
			v.push_back(vertices[k+1]);
			v.push_back(vertices[k+2]);
		}

		// generation of vertex buffer: use triangle_vertices instead of vertices
		glGenBuffers( 1, &vertex_buffer );
		glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertex)*v.size(), &v[0], GL_STATIC_DRAW );
	}

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if(vertex_array) glDeleteVertexArrays(1,&vertex_array);
	vertex_array = cg_create_vertex_array( vertex_buffer, index_buffer );
	if(!vertex_array){ printf("%s(): failed to create vertex aray\n",__func__); return; }
}

void update_tess()
{
	uint n = NUM_TESS; if(b.add) n++; if(b.sub) n--;
	if(n==NUM_TESS||n<MIN_TESS||n>MAX_TESS) return;
	
	unit_sphere_vertices = create_sphere_vertices(NUM_TESS=n);
	update_vertex_buffer( unit_sphere_vertices, NUM_TESS );
	printf( "> NUM_TESS = % -4d\r", NUM_TESS );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)	glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_H || key == GLFW_KEY_F1)	print_help();
		else if (key == GLFW_KEY_KP_ADD || (key == GLFW_KEY_EQUAL && (mods & GLFW_MOD_SHIFT)))	b.add = true;
		else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) b.sub = true;
		else if (key == GLFW_KEY_I)
		{
			b_index_buffer = !b_index_buffer;
			update_vertex_buffer(unit_sphere_vertices, NUM_TESS);
			printf("> using %s buffering\n", b_index_buffer ? "index" : "vertex");
		}
		else if (key == GLFW_KEY_D)
		{
			if (b_tc_xy == false && b_tc_xxx == false) {
				b_tc_xy = true;
				b_tc_xxx = true;
				printf("using tc_xy as color\n");
			}
			else if (b_tc_xy == false) {
				b_tc_xxx = false;
				printf("using tc_yyy as color\n");
			}
			else {
				b_tc_xy = false;
				printf("using tc_xxx as color\n");
			}
		}
		else if (key == GLFW_KEY_R) {
			b_rotate = !b_rotate;
		}
#ifndef GL_ES_VERSION_2_0
		else if(key==GLFW_KEY_W)
		{
			b_wireframe = !b_wireframe;
			glPolygonMode( GL_FRONT_AND_BACK, b_wireframe ? GL_LINE:GL_FILL );
			printf( "> using %s mode\n", b_wireframe ? "wireframe" : "solid" );
		}
#endif
	}
	else if(action==GLFW_RELEASE)
	{
		if(key==GLFW_KEY_KP_ADD||(key==GLFW_KEY_EQUAL&&(mods&GLFW_MOD_SHIFT)))	b.add = false;
		else if(key==GLFW_KEY_KP_SUBTRACT||key==GLFW_KEY_MINUS) b.sub = false;
	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	if(button==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS )
	{
		dvec2 pos; glfwGetCursorPos(window,&pos.x,&pos.y);
		printf( "> Left mouse button pressed at (%d, %d)\n", int(pos.x), int(pos.y) );
	}
}

void motion( GLFWwindow* window, double x, double y )
{
}

bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glLineWidth( 1.0f );
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests
	
	// define the position of four corner vertices
	unit_sphere_vertices = std::move(create_sphere_vertices( NUM_TESS ));

	// create vertex buffer; called again when index buffering mode is toggled
	update_vertex_buffer( unit_sphere_vertices, NUM_TESS );

	return true;
}

void user_finalize()
{
}

int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// init OpenGL extensions

	// initializations and validations of GLSL program
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movements

	// enters rendering/event loop
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
	}
	
	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}
