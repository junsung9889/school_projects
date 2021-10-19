#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "trackball.h"	// virtual trackball

//*************************************
// global constants
static const char*	window_name = "cgbase - trackball";
static const char*	vert_shader_path = "../bin/shaders/trackball.vert";
static const char*	frag_shader_path = "../bin/shaders/trackball.frag";
//static const char*	mesh_vertex_path = "../bin/mesh/dragon.vertex.bin";
//static const char*	mesh_index_path	= "../bin/mesh/dragon.index.bin";

//*************************************
// common structures
struct camera
{
	vec3	eye = vec3( 200, 0, 0 );
	vec3	at = vec3( 0, 0, 0 );
	vec3	up = vec3( 0, 0, 1 );
	mat4	view_matrix = mat4::look_at( eye, at, up );

	float	fovy = PI/4.0f; // must be in radian
	float	aspect;
	float	dnear = 1.0f;
	float	dfar = 1000.0f;
	mat4	projection_matrix;
};

//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint	program	= 0;	// ID holder for GPU program
GLuint	vertex_array = 0;	// ID holder for vertex array object
uint	N = 72;
std::vector<vertex>	unit_sphere_vertices;	// host-side vertices
std::vector<uint>	indices;

//*************************************
// global variables
int		frame = 0;				// index of rendering frames

//*************************************
// scene objects
mesh*		pMesh = nullptr;
camera		cam;
trackball	tb;

auto	spheres = std::move(create_spheres());
float	t = 0.0f;						// current simulation parameter

//*************************************
void update()
{
	t = float(glfwGetTime()) * 0.4f;
	// update projection matrix
	cam.aspect = window_size.x/float(window_size.y);
	cam.projection_matrix = mat4::perspective( cam.fovy, cam.aspect, cam.dnear, cam.dfar );

	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation( program, "view_matrix" );			if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam.view_matrix );
	uloc = glGetUniformLocation( program, "projection_matrix" );	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam.projection_matrix );
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// notify GL that we use our own program
	glUseProgram( program );

	// bind vertex array object
	glBindVertexArray(vertex_array);

	for (auto& s : spheres)
	{
		// per-sphere update
		s.update(t);

		// update per-sphere uniforms
		GLint uloc;
		uloc = glGetUniformLocation(program, "model_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, s.model_matrix);
		// render vertices: trigger shader programs to process vertex data
		glDrawElements(GL_TRIANGLES, (uint)indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	// swap front and back buffers, and display to screen
	glfwSwapBuffers( window );
}

std::vector<vertex> create_sphere_vertices(uint N)
{
	std::vector<vertex> v;
	float radius = 1.0f;
	float dp = 2 * PI / N;
	float dt = PI / (N / 2);
	for (uint l = 0; l <= N / 2; l++)
	{
		float t = l * dt, ct = cos(t), st = sin(t);
		for (uint k = 0; k <= N; k++)
		{
			float p = k * dp, cp = cos(p), sp = sin(p);
			v.push_back({ vec3(radius * st * cp, radius * st * sp, radius * ct), vec3(st * cp, st * sp, ct), vec2(p / (2 * PI), 1 - (t / PI)) });
		}
	}
	return v;
}

void update_vertex_buffer(const std::vector<vertex>& vertices, uint N)
{
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	// create buffers
	for (uint l = 0; l < N / 2; l++)
	{
		uint k1 = l * (N + 1);
		uint k2 = k1 + N + 1;
		for (uint k = 0; k < N; k++, k1++, k2++)
		{
			if (l != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
			if (l != N / 2 - 1) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	// generation of vertex buffer: use vertices as it is
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// geneation of index buffer
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
	vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
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
	printf( "- press Home to reset camera\n" );
	printf( "\n" );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_ESCAPE||key==GLFW_KEY_Q)	glfwSetWindowShouldClose( window, GL_TRUE );
		else if(key==GLFW_KEY_H||key==GLFW_KEY_F1)	print_help();
		else if(key==GLFW_KEY_HOME)					cam=camera();
	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	if(button==GLFW_MOUSE_BUTTON_LEFT)
	{
		dvec2 pos; glfwGetCursorPos(window,&pos.x,&pos.y);
		vec2 npos = cursor_to_ndc( pos, window_size );
		if(action==GLFW_PRESS)			tb.begin( cam.view_matrix, npos);
		else if(action==GLFW_RELEASE)	tb.end();
	}
	else if (tb.button == GLFW_MOUSE_BUTTON_MIDDLE || (tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_CONTROL))) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos);
		else if (action == GLFW_RELEASE)	tb.end();
	}
	else if (tb.button == GLFW_MOUSE_BUTTON_RIGHT || (tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_SHIFT))) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos);
		else if (action == GLFW_RELEASE)	tb.end();
	}
	tb.button = button;
	tb.mods = mods;

}

void motion( GLFWwindow* window, double x, double y )
{
	if (tb.button == GLFW_MOUSE_BUTTON_LEFT && tb.mods == 0) {
		if (!tb.is_tracking()) return;
		vec2 npos = cursor_to_ndc(dvec2(x, y), window_size);
		mat3 t = tb.update(npos);
		/*float k = (cam.eye-cam.at).length();
		vec3 n = vec3(cam.view_matrix._31, cam.view_matrix._32, cam.view_matrix._33);
		vec3 v = vec3(cam.view_matrix._21, cam.view_matrix._22, cam.view_matrix._23);
		cam.eye = cam.at + n.normalize() * k;
		cam.up = v.normalize();*/
		cam.eye = cam.eye * t;
		cam.up = cam.up * t;
		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);
	}
	else if (tb.button == GLFW_MOUSE_BUTTON_MIDDLE || (tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_CONTROL))) {
		if (!tb.is_panning()) return;
		vec2 npos = cursor_to_ndc(dvec2(x, y), window_size);
		vec3 uvdiff = tb.updatePan(npos,window_size);
		cam.eye -= uvdiff;
		cam.at -= uvdiff;
		cam.view_matrix =  mat4::look_at(cam.eye, cam.at, cam.up);
	}
	else if (tb.button == GLFW_MOUSE_BUTTON_RIGHT || (tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_SHIFT))) {
		if (!tb.is_zooming()) return;
		vec2 npos = cursor_to_ndc(dvec2(x, y), window_size);
		vec3 ndiff = tb.updateZoom(npos, window_size);
		if ((cam.eye + ndiff - cam.at).normalize().operator==((cam.eye-cam.at).normalize()))
			cam.eye += ndiff;
		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);
	}
}

bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests

	// define the position of four corner vertices
	unit_sphere_vertices = std::move(create_sphere_vertices(N));

	// create vertex buffer; called again when index buffering mode is toggled
	update_vertex_buffer(unit_sphere_vertices, N);

	return true;
}

void user_finalize()
{
}

int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// enters rendering/event loop
	float t1 = (float)glfwGetTime();
	float t2 = (float)glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		while (t2 - t1 < 0.0001f) {
			t2 = (float)glfwGetTime();
		}
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
		t1 = t2;
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}
