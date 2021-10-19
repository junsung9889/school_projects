#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__
#include "cgmath.h"

ivec2		window_size = ivec2(1280, 720);

struct universe {

	bool	b_tracking = false;
	bool	b_panning = false;
	bool	b_zooming = false;

	float	scale;			// controls how much rotation is applied

	mat4	view_matrix0;	// initial view matrix
	vec2	m0;				// the last mouse position
	vec3	eye0;
	vec3	at0;
	vec3	up0;
	mat4	r_matrix;

	universe(float rot_scale = 1.0f) : scale(rot_scale) {}

	void begin(const mat4& view_matrix, vec2 m, vec3 eye, vec3 at, vec3 up);
	void panning_begin(const mat4& view_matrix, vec2 m, vec3 eye, vec3 at, vec3 up);
	void zooming_begin(const mat4& view_matrix, vec2 m, vec3 eye, vec3 at, vec3 up);

	void end() {
		b_tracking = false;
		b_panning = false;
		b_zooming = false;
	}

	vec3 update_eye(vec2 m, vec3 eye, vec3 at, vec3 up);
	vec3 update_at(vec2 m, vec3 eye, vec3 at, vec3 up);
	vec3 update_up(vec2 m, vec3 eye, vec3 at, vec3 up);

	vec3 update_pan(vec2 m, vec3 eye, vec3 at, vec3 up);
	vec3 update_zoom(vec2 m, vec3 eye, vec3 at, vec3 up);
};
inline void universe::begin(const mat4& view_matrix, vec2 m, vec3 eye, vec3 at, vec3 up)
{
	b_tracking = true;			// enable trackball tracking
	m0 = m;						// save current mouse position
	view_matrix0 = view_matrix;	// save current view matrix
	eye0 = eye;
	at0 = at;
	up0 = up;
}

inline void universe::panning_begin(const mat4& view_matrix, vec2 m, vec3 eye, vec3 at, vec3 up)
{
	b_panning = true;			// enable trackball tracking
	m0 = m;						// save current mouse position
	view_matrix0 = view_matrix;	// save current view matrix
	eye0 = eye;
	at0 = at;
	up0 = up;
}


inline void universe::zooming_begin(const mat4& view_matrix, vec2 m, vec3 eye, vec3 at, vec3 up)
{
	b_zooming = true;			// enable trackball tracking
	m0 = m;						// save current mouse position
	view_matrix0 = view_matrix;	// save current view matrix
	eye0 = eye;
	at0 = at;
	up0 = up;
}

inline vec2 cursor_to_ndc(dvec2 cursor, ivec2 window_size)
{
	// normalize window pos to [0,1]^2
	vec2 npos = vec2(float(cursor.x) / float(window_size.x - 1),
		float(cursor.y) / float(window_size.y - 1));

	return vec2(npos.x * 2.0f - 1.0f, 1.0f - npos.y * 2.0f);
}

inline mat3 change_to_uvn(vec3 eye, vec3 at, vec3 up) {
	vec3 n = (eye - at).normalize();
	vec3 u = up.cross(n).normalize();
	vec3 v = n.cross(u).normalize();

	return mat3(u.x, u.y, u.z, v.x, v.y, v.z, n.x, n.y, n.z);
}

inline vec3 universe::update_eye(vec2 m, vec3 eye, vec3 at, vec3 up)
{
	// project a 2D mouse position to a unit sphere
	static const vec3 p0 = vec3(0, 0, 1.0f);	// reference position on sphere
	vec3 p1 = vec3(m - m0, 0);					// displacement

	if (length(p1) < 0.0001f) return eye;		// ignore subtle movement

	p1 *= 0.47f;														// apply rotation scale
	p1 = vec3(p1.x, p1.y, sqrtf(max(0, 1.0f - length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

	vec3 v = mat3(view_matrix0).transpose() * p0.cross(p1);
	float theta = asin(min(v.length(), 1.0f));

	mat3 rotation_matrix = mat3(mat4::rotate(v.normalize(), theta));
	p1 *= 0.95f;														// apply rotation scale
	p1 = vec3(p1.x, p1.y, sqrtf(max(0, 1.0f - length2(p1)))).normalize();	// back-project z=0 onto the unit sphere
	v = mat3(view_matrix0).transpose() * p0.cross(p1);
	theta = asin(min(v.length(), 1.0f));
	r_matrix = mat4::rotate(v.normalize(), -theta);
	return eye0 * rotation_matrix;
}

inline vec3 universe::update_up(vec2 m, vec3 eye, vec3 at, vec3 up)
{
	// project a 2D mouse position to a unit sphere
	static const vec3 p0 = vec3(0, 0, 1.0f);	// reference position on sphere
	vec3 p1 = vec3(m - m0, 0);					// displacement

	if (length(p1) < 0.0001f) return up;	// ignore subtle movement
	p1 *= 0.47f;														// apply rotation scale
	p1 = vec3(p1.x, p1.y, sqrtf(max(0, 1.0f - length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

	vec3 v = mat3(view_matrix0).transpose() * p0.cross(p1);
	float theta = asin(min(v.length(), 1.0f));

	mat3 rotation_matrix = mat3(mat4::rotate(v.normalize(), theta));
	return up0 * rotation_matrix;
}

inline vec3 universe::update_pan(vec2 m, vec3 eye, vec3 at, vec3 up) {
	vec3 p1 = vec3(m - m0, 0);					// displacement
	if (length(p1) < 0.0001f) return vec3(0, 0, 0);			// ignore subtle movement

	mat3 uvn_plane = change_to_uvn(eye, at, up);

	vec3 u_plane = vec3(uvn_plane[0], uvn_plane[1], uvn_plane[2]);
	vec3 v_plane = vec3(uvn_plane[3], uvn_plane[4], uvn_plane[5]);

	return vec3(u_plane * p1.x * float(window_size.x - 1) * 0.02f + v_plane * p1.y * float(window_size.y - 1) * 0.02f);
}

inline vec3 universe::update_zoom(vec2 m, vec3 eye, vec3 at, vec3 up) {
	vec3 p1 = vec3(m - m0, 0);					// displacement
	if (length(p1) < 0.0001f) return vec3(0, 0, 0);			// ignore subtle movement

	mat3 uvn_plane = change_to_uvn(eye, at, up);
	vec3 n_plane = vec3(uvn_plane[6], uvn_plane[7], uvn_plane[8]);

	return vec3(n_plane * (p1.y) * float(window_size.y - 1) * 0.02f);
}

#endif // __TRACKBALL_H__
