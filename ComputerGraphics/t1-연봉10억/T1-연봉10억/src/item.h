#pragma once
#include "cgmath.h"         // slee's simple math library
#include "cgut.h"         // slee's OpenGL utility
#include "Custom_Function.h"

class Item {
private:
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint texture_buffer = { 0 };
    GLuint vertex_array = { 0 };

    bool item_gone = false;
    bool get_item = false;
    vec3   center = vec3(0, 5, -30);
    float	speed = 0.05f;
    float   hit_count = 0.0f;


    std::vector<vertex>   vertex_list;
    std::vector<uint>   index_list;
    mat4 model_matrix;
    mat4 rotation_matrix = mat4::identity();
    const char* path = { "../bin/mesh/CrossHair/skill.jpg" };

public:
    void render(GLuint program) {
        glActiveTexture(GL_TEXTURE4);
        glUniform1i(glGetUniformLocation(program, "mode"), 4);
        glBindVertexArray(vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBindTexture(GL_TEXTURE_2D, texture_buffer);
        update();
        glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
        cg_bind_vertex_attributes(program);
        glDrawElements(GL_TRIANGLES, (uint)index_list.size(), GL_UNSIGNED_INT, nullptr);
    }
    Item() {
        float delta_phi = 2 * PI / 72;
        float delta_theta = PI / 36;
        float theta, phi;
        
        for (uint i = 0; i <= 36; i++) {
            theta = i * delta_theta;
            for (uint j = 0; j <= 72; j++) {
                phi = j * delta_phi;

                vec3 pos = vec3(5 * sin(theta) * cos(phi), 5 * sin(theta) * sin(phi), 5 * cos(theta));
                vec3 norm = { sin(theta) * cos(phi), sin(theta) * sin(phi) , cos(theta) };
                vec2 texc = { phi / (2 * PI), 1 - (theta / PI) };

                vertex new_vertex = { pos, norm, texc };
                vertex_list.push_back(new_vertex);
            }
        }
        uint index1, index2;
        for (uint i = 0; i < 36; i++)
        {
            index1 = i * (72 + 1);
            index2 = index1 + 72 + 1;

            for (uint j = 0; j < 72; j++, index1++, index2++)
            {
                if (i != 0)
                {
                    index_list.push_back(index1);
                    index_list.push_back(index2);
                    index_list.push_back(index1 + 1);
                }
                if (i != (36 - 1))
                {
                    index_list.push_back(index1 + 1);
                    index_list.push_back(index2);
                    index_list.push_back(index2 + 1);
                }
            }
        }
        bind_texture(&texture_buffer, path, window_size);

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);


        // geneation of index buffer
        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list.size(), &index_list[0], GL_STATIC_DRAW);

        if (vertex_array) { glDeleteVertexArrays(1, &vertex_array); }

        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

        if (index_buffer) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

        static size_t attrib_size[] = { sizeof(vertex().pos), sizeof(vertex().norm), sizeof(vertex().tex) };
        for (size_t k = 0, byte_offset = 0; k < 3; k++, byte_offset += attrib_size[k - 1])
        {
            glEnableVertexAttribArray(GLuint(k));
            glVertexAttribPointer(GLuint(k), attrib_size[k] / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)byte_offset);
        }

        glEnableVertexAttribArray(0);
    }
    void update()
    {
        vec3 space_ship_position = vec3(0, 0, -30);

        if ((vec3(0, 0, -30) - center).length() < 8.0f) {
            item_gone = true;
        }
        else {
            center += (space_ship_position - center) * speed;
        }

        mat4 scale_matrix =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        mat4 rotate_matrix =
        {
            1, 0, 0, 0,
            0, cos(30.0f), -sin(30.0f), 0,
            0, sin(30.0f), cos(30.0f), 0,
            0, 0, 0, 1
        };
        mat4 translate_matrix =
        {
            1, 0, 0, center.x,
            0, 1, 0, center.y,
            0, 0, 1, center.z,
            0, 0, 0, 1
        };
        model_matrix = translate_matrix * rotate_matrix * scale_matrix;
    }

    void setCenter(vec3 at) {
        center = at;
    }
    void setSpeed(float _speed) {
        speed = _speed;
    }
    vec3 getCenter() { return center; }
    bool   GetItem(vec3 CameraPos);
    bool getItemGone() { return item_gone; }
    bool getItemGet() { return get_item; }
};