#pragma once
#include "cgmath.h"         // slee's simple math library
#include "cgut.h"         // slee's OpenGL utility
#include "Custom_Function.h"

class SaturnRing {
private:
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint texture_buffer = { 0 };
    GLuint vertex_array = { 0 };

    vec3   center = vec3(300, -100, 300);

    std::vector<vertex>   vertex_list;
    std::vector<uint>   index_list;
    mat4 model_matrix;
    mat4 rotation_matrix = mat4::identity();
    const char* path = { "../bin/images/saturnringcolor.jpg" };

public:
    void render(GLuint program) {
        glActiveTexture(GL_TEXTURE7);
        glUniform1i(glGetUniformLocation(program, "mode"), 7);
        glBindVertexArray(vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBindTexture(GL_TEXTURE_2D, texture_buffer);
        update();
        glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
        cg_bind_vertex_attributes(program);
        glDrawElements(GL_TRIANGLES, (uint)index_list.size(), GL_UNSIGNED_INT, nullptr);
    }
    SaturnRing() {
        float inner = 110.0f;
        float outer = 250.0f;
        for (uint k = 0; k <= 72; k++) {
            float t = PI * 2.0f * k / float(72), c = cos(t), s = sin(t);
            float tc_alpha = k / float(72);

            vertex_list.push_back({ vec3(inner * c, inner * s, 0.0f), vec3(c, s, 0.0f), vec2(1, tc_alpha) });
            vertex_list.push_back({ vec3(outer * c, outer * s, 0.0f), vec3(c, s, 0.0f), vec2(0, tc_alpha) }); 
        }
        for (uint k = 0; k <= 144; k += 1)
        {
            //ring_indices.push_back(0);
            index_list.push_back(k);
            index_list.push_back(k + 1);
            index_list.push_back(k + 2);
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
            0, cos(-30.0f), -sin(-30.0f), 0,
            0, sin(-30.0f), cos(-30.0f), 0,
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
};