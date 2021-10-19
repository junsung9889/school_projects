#pragma once
#ifndef __CIRCLE_H__
#define __CIRCLE_H__
using namespace std;
#include <numeric>

struct circle_t
{
	vec2	center=vec2(0);		// 2D position for translation
	float	radius=1.0f;		// radius
	float	theta=0.0f;			// rotation angle
	vec4	color;				// RGBA color in [0,1]
	mat4	model_matrix;		// modeling transformation
	vec2	velocity = vec2(0);
	float	m = 0.0f;

	// public functions
	void	update( float t );
	void	checkBoundary();
};

inline bool checkC(circle_t c, vector<circle_t> circles) {
	if (!circles.empty()) {
		for (auto& circle : circles) {
			float distance = (float)sqrt(pow(c.center.x - circle.center.x, 2) + pow(c.center.y - circle.center.y, 2));
			if (distance <= (c.radius + circle.radius)) {
				return false;
			}
		}
	}
	if (c.center.x - c.radius < -1.77f || c.center.x + c.radius > 1.77f)
		return false;
	if (c.center.y - c.radius < -1.0f || c.center.y + c.radius > 1.0f)
		return false;
	return true;
}

inline vector<circle_t> updateRadius(vector<circle_t> circles, uint N) {
	if (!circles.empty() && N > 1) {
		for (auto& c : circles) {
			c.radius = c.radius * (float)(sqrt((double)(N-1)) / sqrt((double)N));
		}
	}
	return circles;
}

inline std::vector<circle_t> create_circles()
{
	std::vector<circle_t> circles;
	circle_t c;

	for (int i = 1;i <= 30;i++) {
		circles = updateRadius(circles, i);
		do {
			c.center.x = rand() % 354 * 0.01f - 1.77f;
			c.center.y = rand() % 200 * 0.01f - 1.0f;
			c.radius = (rand()%400*0.001f + 0.2f) * (float)(1.0 / sqrt((double)i));
		} while (!checkC(c,circles));
		c.color.r = rand() % 101 * 0.01f;
		c.color.g = rand() % 101 * 0.01f;
		c.color.b = rand() % 101 * 0.01f;
		c.color.a = 1.0f;
		float xVariation = rand() % 300 * 0.0001f - 0.015f;
		float yVariation = rand() % 300 * 0.0001f - 0.015f;
		c.velocity = vec2(xVariation, yVariation);
		c.m = (float)pow(c.radius, 2) * 4.0f * 3.14f;
		circles.emplace_back(c);
	}
	return circles;
}

inline std::vector<circle_t> create_circle(vector<circle_t> circles) {
	circle_t c;
	int N = (int)circles.size();
	circles = updateRadius(circles, N);
	do {
		c.center.x = rand() % 354 * 0.01f - 1.77f;
		c.center.y = rand() % 200 * 0.01f - 1.0f;
		c.radius = (rand() % 400 * 0.001f + 0.2f) * (float)(1.0 / sqrt((double)N));
	} while (!checkC(c, circles));
	c.color.r = rand() % 101 * 0.01f;
	c.color.g = rand() % 101 * 0.01f;
	c.color.b = rand() % 101 * 0.01f;
	c.color.a = 1.0f;
	float xVariation = rand() % 300 * 0.0001f - 0.015f;
	float yVariation = rand() % 300 * 0.0001f - 0.015f;
	c.velocity = vec2(xVariation, yVariation);
	c.m = (float)pow(c.radius, 2) * 4.0f * 3.14f;
	circles.emplace_back(c);
	return circles;
}
inline std::vector<circle_t> delete_circle(vector<circle_t> circles) {
	circles.pop_back();
	circles.shrink_to_fit();
	return circles;
}
inline void circle_t::checkBoundary() {
	if (center.x + radius > 1.77f) {
		velocity.x *= -1;
		center.x -= center.x + radius - 1.77f;
	}
	if (center.x - radius < -1.77f) {
		velocity.x *= -1;
		center.x -= center.x - radius + 1.77f;
	}
	if (center.y + radius > 1.00f) {
		velocity.y *= -1;
		center.y -= center.y + radius - 1.00f;
	}
	if (center.y - radius < -1.00f) {
		velocity.y *= -1;
		center.y -= center.y - radius + 1.00f;
	}
}

inline void circle_t::update( float t )
{
	//checkBoundary();

	float distance = (float)sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	float c	= velocity.x / distance, s = velocity.y / distance;


	checkBoundary();

	center.x += velocity.x * 0.01f;
	center.y += velocity.y * 0.01f;
	// these transformations will be explained in later transformation lecture
	mat4 scale_matrix =
	{
		radius, 0, 0, 0,
		0, radius, 0, 0,
		0, 0, 1, 0,
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
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	model_matrix = translate_matrix*scale_matrix;
}
#endif
