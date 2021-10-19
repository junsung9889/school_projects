#pragma once
#ifndef __CGUT2_H__
#define __CGUT2_H__

#include "cgut.h"
#include "particle.h"

struct Texture
{
	std::string path;
	GLuint id = 0;

	~Texture(void){ glDeleteTextures(1, &id); }
};
struct material
{
	std::string name;

	vec4	ambient;
	vec4	diffuse;
	vec4	specular;
	vec4	emissive;

	float	shininess;			// the power of specular in phong.
	float	opacity;			// alpha OR opacity
	float	refractive_index;		// index of refraction (IOR)

	struct maps {
		Texture* ambient	= nullptr;
		Texture* diffuse	= nullptr;
		Texture* specular	= nullptr;
		Texture* bump		= nullptr;
	} textures;
};
struct geometry
{
	material*	mat = nullptr;		// material of this geometry
	size_t		index_start = 0;	// the starting index of index buffer.
	size_t		index_count = 0;	// the number of indicies of index buffer. 

	geometry(void) {}
	geometry(material* mat, size_t index_start, size_t index_count): mat(mat), index_start(index_start), index_count(index_count) {}
	size_t get_num_faces(void) { return index_count / 3; }
};
struct mesh2
{
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	GLuint				vertex_buffer = 0;
	GLuint				index_buffer = 0;
	GLuint				vertex_array = 0;

	std::vector<material*>	material_list;
	std::vector<geometry>	geometry_list;
	
	vec3	center = vec3(0, 0, 0);
	mat4	model_matrix;
	mat4	rotation_matrix = mat4::identity();

	void	update(bool animation);

	~mesh2(void)
	{
		const size_t k_end = material_list.size();
		for (size_t k = 0; k < k_end; k++) {
			delete material_list[k];
		}

		if(vertex_buffer) glDeleteBuffers(1, &vertex_buffer);
		if(index_buffer) glDeleteBuffers(1, &index_buffer);
		if(vertex_array) glDeleteVertexArrays(1,&vertex_array);
	}
};
inline void mesh2::update(bool animation)
{
	if (animation) {
		if (center.z == -30.0f) {
			center.z -= 0.5f;
			center.y -= 0.5;
		}
		else if (center.z == -30.5f) {
			center.z += 0.5f;
			center.y += 0.5f;
		}
		printf("shot animation run\n");
	}

	mat4 scale_matrix =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
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

	model_matrix = translate_matrix *rotation_matrix* scale_matrix;
}
struct mesh_enemy
{
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	GLuint				vertex_buffer = 0;
	GLuint				index_buffer = 0;
	GLuint				vertex_array = 0;

	std::vector<material*>	material_list;
	std::vector<geometry>	geometry_list;

	vec3	center = vec3(0, 0, 0);
	mat4	model_matrix;
	mat4	rotation_matrix = mat4::identity();
	float	speed = 0.005f;

	bool enemy_hit_with_spaceship = false;
	bool enemy_death = false;
	float hit_count = 0.0f;

	void	update();
	float   CalcDistance(vec3 enemyPos, vec3 playerPos);
	bool   CheckHitted(vec3 CameraPos);
	~mesh_enemy(void)
	{
		const size_t k_end = material_list.size();
		for (size_t k = 0; k < k_end; k++) {
			delete material_list[k];
		}

		if (vertex_buffer) glDeleteBuffers(1, &vertex_buffer);
		if (index_buffer) glDeleteBuffers(1, &index_buffer);
		if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
	}
};
inline void mesh_enemy::update()
{
	vec3 space_ship_position = vec3(0, 0, -30);

	if ((vec3(0, 0, -30) - center).length() < 5.0f) {
		enemy_hit_with_spaceship = true;
	}
	else {
		center = center + (space_ship_position - center).normalize() * speed;
	}

	mat4 scale_matrix =
	{
	   1, 0, 0, 0,
	   0, 1, 0, 0,
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
inline float mesh_enemy::CalcDistance(vec3 enemyPos, vec3 playerPos)
{
	return sqrt(powf(enemyPos.x - playerPos.x, 2.0f) + powf(enemyPos.y - playerPos.y, 2.0f));
}


#endif // __CGUT2_H__
