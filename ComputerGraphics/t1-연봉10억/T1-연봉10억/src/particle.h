#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#pragma once

#include "cgmath.h"
#include "cgut.h"

#define Random(n) rand()%n
inline float random_range(float min, float max) { return mix(min, max, rand() / float(RAND_MAX)); }


struct particle_t
{
	static constexpr int MAX_PARTICLES = 20;
	vec3 center = vec3(0);
	vec3 pos;
	vec4 color;
	vec3 velocity;
	float scale;
	float life;
	mat4 model_matrix;
	//optional
	float elapsed_time;
	float time_interval;

	bool is_alive = true;

	int count = 0;

	particle_t() { };
	void reset();
	void update();
	void update2();
};

inline void particle_t::update2()
{
	mat4 translate_matrix =
	{
		1, 0, 0, pos.x,
		0, 1, 0, pos.y,
		0, 0, 1, pos.z,
		0, 0, 0, 1
	};
	mat4 scale_matrix = mat4::scale(scale);
	model_matrix = translate_matrix * scale_matrix;
}

inline void particle_t::reset()
{
	pos = vec3(random_range(center.x - 5.0f, center.x + 5.0f), random_range(center.y - 5.0f, center.y + 5.0f), center.z);
	color = vec4(1.0f, 1.0f, 1.0f, 1);
	scale = random_range(0.005f, 0.08f);
	life = random_range(0.01f, 1.0f);
	velocity = vec3(random_range(-1.0f, 1.0f), random_range(-1.0f, 1.0f), random_range(-1.0f, 1.0f)) * 0.003f;
	elapsed_time = 0.0f;
	time_interval = random_range(200.0f, 600.0f);
}

inline void particle_t::update()
{
	const float dwTime = (float)glfwGetTime();
	elapsed_time += dwTime;

	if (elapsed_time > time_interval)
	{
		const float theta = random_range(0, 1.0f) * PI * 2.0f;
		constexpr float velocity_factor = 0.003f;
		velocity = vec3(cos(theta), sin(theta), 0) * velocity_factor;

		elapsed_time = 0.0f;
	}

	pos += velocity;

	constexpr float life_factor = 0.01f;
	life -= life_factor * dwTime;

	// disappear
	if (life < 0.0f)
	{
		constexpr float alpha_factor = 0.01f;
		color.a -= alpha_factor * dwTime;
	}

	// dead
	if (count < 2) {
		if (color.a < 0.0f) {
			count++;
			reset();
		}
	}
	else {
		is_alive = false;
	}
}

#endif