#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__
#include "cgmath.h"

struct trackball
{
	int		button;
	int		mods;
	bool	b_tracking = false;
	bool	b_panning = false;
	bool	b_zooming = false;
	float	scale;			// controls how much rotation is applied
	mat4	view_matrix0;	// initial view matrix
	vec2	m0;				// the last mouse position

	trackball( float rot_scale=0.1f ) : scale(rot_scale){}
	bool is_tracking() const { return b_tracking; }
	bool is_panning() const { return b_panning; }
	bool is_zooming() const { return b_zooming; }
	void begin( const mat4& view_matrix, vec2 m);
	void end() { b_tracking = false; b_panning = false; b_zooming = false; }
	mat3 update( vec2 m )const;
	vec3 updatePan(vec2 m, ivec2 window_size);
	vec3 updateZoom(vec2 m, ivec2 window_size);
};

inline void trackball::begin( const mat4& view_matrix, vec2 m)
{
	b_tracking = true;			// enable trackball tracking
	b_panning = true;
	b_zooming = true;
	m0 = m;						// save current mouse position
	view_matrix0 = view_matrix;	// save current view matrix
}

inline mat3 trackball::update( vec2 m ) const
{
	// project a 2D mouse position to a unit sphere
	static const vec3 p0 = vec3(0,0,1.0f);	// reference position on sphere
	vec3 p1 = vec3(m-m0,0);					// displacement
	if( !b_tracking || length(p1)<0.0001f ) return mat3::identity();		// ignore subtle movement
	p1 *= scale;														// apply rotation scale
	p1 = vec3(p1.x,p1.y,sqrtf(max(0,1.0f-length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

	// find rotation axis and angle in world space
	// - trackball self-rotation should be done at first in the world space
	// - mat3(view_matrix0): rotation-only view matrix
	// - mat3(view_matrix0).transpose(): inverse view-to-world matrix
	vec3 v = mat3(view_matrix0).transpose()*p0.cross(p1);
	float theta = asin( min(v.length(),1.0f) );
	// resulting view matrix, which first applies
	// trackball rotation in the world space
	//return view_matrix0*mat4::rotate(v.normalize(),theta);
	return (mat3)(mat4::rotate(v.normalize(), theta));
}


inline vec3 trackball::updatePan(vec2 m, ivec2 window_size)
{
	vec3 u = vec3(view_matrix0._11, view_matrix0._12, view_matrix0._13);
	vec3 v = vec3(view_matrix0._21, view_matrix0._22, view_matrix0._23);
	vec2 xydiff = vec2((m.x - m0.x)* float(window_size.x - 1)*0.1f, (m.y - m0.y)* float(window_size.y - 1)*0.1f);
	m0 = m;
	if (!b_panning || length(xydiff) < 0.0001f) return vec3(0,0,0);
	vec3 uvdiff = u * xydiff.x + v * xydiff.y;
	return uvdiff;

}
inline vec3 trackball::updateZoom(vec2 m, ivec2 window_size) 
{
	vec3 n = vec3(view_matrix0._31, view_matrix0._32, view_matrix0._33);
	vec2 xydiff = vec2((m.x - m0.x) * float(window_size.x - 1) * 0.2f, (m.y - m0.y) * float(window_size.y - 1) * 0.2f);
	m0 = m;
	if (!b_zooming || length(xydiff) < 0.0001f) return vec3(0, 0, 0);
	vec3 ndiff = n * xydiff.y;
	return ndiff;
}

// utility function
inline vec2 cursor_to_ndc( dvec2 cursor, ivec2 window_size )
{
	// normalize window pos to [0,1]^2
	vec2 npos = vec2( float(cursor.x)/float(window_size.x-1),
					  float(cursor.y)/float(window_size.y-1) );
	
	// normalize window pos to [-1,1]^2 with vertical flipping
	// vertical flipping: window coordinate system defines y from
	// top to bottom, while the trackball from bottom to top
	return vec2(npos.x*2.0f-1.0f,1.0f-npos.y*2.0f);
}

struct sphere_t
{
	vec3	center = vec3(0);		// 2D position for translation
	float	radius = 1.0f;		// radius
	float	theta = 0.0f;
	float	speed = 0.0f;
	float	distance = 0.0f;
	int		planet_num = 0;
	int		center_num = 0;
	//vec4	color;				// RGBA color in [0,1]
	mat4	model_matrix;		// modeling transformation

	// public functions
	void	update(std::vector<sphere_t> spheres, int num);
};

struct circle_t
{
	vec3	center = vec3(0);		// 2D position for translation
	float	radius = 1.0f;		// radius
	int		center_num = 0;
	mat4	model_matrix;		// modeling transformation
	// public functions
	void	update(std::vector<sphere_t> spheres, int num);
};


inline std::vector<sphere_t> create_spheres()
{
	std::vector<sphere_t> spheres;
	sphere_t s;
	s = { vec3(0,0,0),10.0f,0.0f,1.0f, 0.0f,1};		//Sun
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.0f,0.0f,6.0f, 15.0f,2 };	//Mercury
	spheres.emplace_back(s);
	s = { vec3(0,0,0),2.0f,0.0f,3.7f, 20.0f,3 };	//Venus
	spheres.emplace_back(s);
	s = { vec3(0,0,0),4.0f,0.0f,5.2f, 35.0f,4 };	//Earth
	spheres.emplace_back(s);
	s = { vec3(0,0,0),3.0f,0.0f,2.3f, 50.0f,5 };	//Mars
	spheres.emplace_back(s);
	s = { vec3(0,0,0),7.0f,0.0f,1.3f, 85.0f,6 };	//Jupiter	
	spheres.emplace_back(s);
	s = { vec3(0,0,0),6.0f,0.0f,3.7f, 130.0f,7 };	//Saturn
	spheres.emplace_back(s);
	s = { vec3(0,0,0),4.0f,0.0f,0.3f, 160.0f,8 };	//Uranus
	spheres.emplace_back(s);
	s = { vec3(0,0,0),3.0f,0.0f,1.2f, 190.0f,9 };	//Neptune
	spheres.emplace_back(s);


	s = { vec3(0,0,0),1.0f,0.0f,15.0f,8.0f,10,3};	//Earth
	spheres.emplace_back(s);

	s = { vec3(0,0,0),1.0f,0.0f,30.0f,9.0f,10,5 };	//Jupiter
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.5f,0.0f,15.0f,12.0f,10,5 };
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.0f,0.0f,25.0f,15.0f,10,5 };
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.5f,0.0f,20.0f,18.0f,10,5 };
	spheres.emplace_back(s);

	s = { vec3(0,0,0),0.5f,0.0f,25.0f,5.0f,10,7 };	//Uranus
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.0f,0.0f,35.0f,7.0f,10,7 };
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.0f,0.0f,20.0f,10.0f,10,7 };
	spheres.emplace_back(s);
	s = { vec3(0,0,0),0.5f,0.0f,15.0f,12.0f,10,7 };
	spheres.emplace_back(s);
	s = { vec3(0,0,0),1.0f,0.0f,25.0f,14.0f,10,7 };
	spheres.emplace_back(s);

	s = { vec3(0,0,0),0.5f,0.0f,15.0f,5.0f,10,8 };	//Neptune
	spheres.emplace_back(s);
	s = { vec3(0,0,0),0.7f,0.0f,25.0f,7.0f,10,8 };
	spheres.emplace_back(s);
	return spheres;
}

inline void sphere_t::update(std::vector<sphere_t> spheres,int num)
{		// simple animation
	//if (b_rotate)
	theta += 0.002f * speed;
	float c = cos(theta), s = sin(theta);
	center = spheres[num].center;
	center.x += distance * c;
	center.y += distance * s;

	// these transformations will be explained in later transformation lecture
	mat4 scale_matrix =
	{
		radius, 0, 0, 0,
		0, radius, 0, 0,
		0, 0, radius, 0,
		0, 0, 0, 1
	};

	mat4 rotation_matrix =
	{
		c,-s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 translate_matrix =
	{
		1, 0, 0, center.x,
		0, 1, 0, center.y,
		0, 0, 1, center.z,
		0, 0, 0, 1
	};

	model_matrix = translate_matrix * rotation_matrix * scale_matrix;
}

inline std::vector<circle_t> create_circles() 
{
	std::vector<circle_t> circles;
	circle_t c;
	c = { vec3(0,0,0),15.0f,6 };
	circles.emplace_back(c);
	c = { vec3(0,0,0),10.0f,7 };
	circles.emplace_back(c);
	return circles;
}

inline void circle_t::update(std::vector<sphere_t> spheres, int num)
{
	center.x = spheres[num].center.x;
	center.y = spheres[num].center.y;
	center.z = 0;
	mat4 scale_matrix =
	{
		radius, 0, 0, 0,
		0, radius, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 translate_matrix =
	{
		1, 0, 0, center.x,
		0, 1, 0, center.y,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	model_matrix = translate_matrix * scale_matrix;
}
#endif // __TRACKBALL_H__
