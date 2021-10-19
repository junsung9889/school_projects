#pragma once
#include "cgmath.h"         // slee's simple math library
#include "cgut.h"         // slee's OpenGL utility
#include "Custom_Function.h"


unsigned int loadCubemap(std::vector<std::string> path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            printf("Cubemap texture failed to load\n");
            // std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

class Skycube {
private:
    GLuint vertex_buffer[6];
    GLuint index_buffer[6];
    GLuint texture_buffer[6] = { 0 };
    GLuint vertex_array[6] = { 0 };

    std::vector<vertex>   vertex_list[6];
    std::vector<uint>   index_list[6];

    const char* path[6] = { "../bin/mesh/Skyboxes/Space/bottom.jpg","../bin/mesh/Skyboxes/Space/left.jpg",
       "../bin/mesh/Skyboxes/Space/right.jpg","../bin/mesh/Skyboxes/Space/top.jpg",
       "../bin/mesh/Skyboxes/Space/front.jpg", "../bin/mesh/Skyboxes/Space/back.jpg" };

    const float size = 1000.0f;

public:
    void render(GLuint program) {

        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(program, "mode"), 1);

        for (int i = 0; i < 6; i++) {
            glBindVertexArray(vertex_array[i]);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer[i]);
            glBindTexture(GL_TEXTURE_2D, texture_buffer[i]);
            mat4 model_matrix = mat4::identity();
            glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
            // glUniform1i(glGetUniformLocation(program, "VertexShaderMode"), SKYBOX_SHADE);
            cg_bind_vertex_attributes(program);
            glDrawElements(GL_TRIANGLES, index_list[i].size(), GL_UNSIGNED_INT, nullptr);
        }
    }
    Skycube() {
        //bottom
        vertex_list[0].push_back({ vec3(-size,  size, -size),vec3(0,0,-1),vec2(0.0f,1.0f) });
        vertex_list[0].push_back({ vec3(-size, -size, -size),vec3(0,0,-1),vec2(0.0f,0.0f) });
        vertex_list[0].push_back({ vec3(size, -size, -size),vec3(0,0,-1),vec2(1.0f,0.0f) });
        vertex_list[0].push_back({ vec3(size, -size, -size),vec3(0,0,-1),vec2(1.0f,0.0f) });
        vertex_list[0].push_back({ vec3(size,  size, -size),vec3(0,0,-1),vec2(1.0f,1.0f) });
        vertex_list[0].push_back({ vec3(-size,  size, -size),vec3(0,0,-1),vec2(0.0f,1.0f) });

        //left
        vertex_list[1].push_back({ vec3(-size, -size,  size),vec3(1,0,0),vec2(0.0f,1.0f) });
        vertex_list[1].push_back({ vec3(-size, -size, -size),vec3(1,0,0),vec2(0.0f,0.0f) });
        vertex_list[1].push_back({ vec3(-size,  size, -size),vec3(1,0,0),vec2(1.0f,0.0f) });
        vertex_list[1].push_back({ vec3(-size,  size, -size),vec3(1,0,0),vec2(1.0f,0.0f) });
        vertex_list[1].push_back({ vec3(-size,  size,  size),vec3(1,0,0),vec2(1.0f,1.0f) });
        vertex_list[1].push_back({ vec3(-size, -size,  size),vec3(1,0,0),vec2(0.0f,1.0f) });

        //right
        vertex_list[2].push_back({ vec3(size, -size, -size),vec3(-1,0,0),vec2(1.0f,0.0f) });
        vertex_list[2].push_back({ vec3(size, -size,  size),vec3(-1,0,0),vec2(1.0f,1.0f) });
        vertex_list[2].push_back({ vec3(size,  size,  size),vec3(-1,0,0),vec2(0.0f,1.0f) });
        vertex_list[2].push_back({ vec3(size,  size,  size),vec3(-1,0,0),vec2(0.0f,1.0f) });
        vertex_list[2].push_back({ vec3(size,  size, -size),vec3(-1,0,0),vec2(0.0f,0.0f) });
        vertex_list[2].push_back({ vec3(size, -size, -size),vec3(-1,0,0),vec2(1.0f,0.0f) });

        //top
        vertex_list[3].push_back({ vec3(-size, -size,  size),vec3(0,0,-1),vec2(0.0f,1.0f) });
        vertex_list[3].push_back({ vec3(-size,  size,  size),vec3(0,0,-1),vec2(0.0f,0.0f) });
        vertex_list[3].push_back({ vec3(size,  size,  size),vec3(0,0,-1),vec2(1.0f,0.0f) });
        vertex_list[3].push_back({ vec3(size,  size,  size),vec3(0,0,-1),vec2(1.0f,0.0f) });
        vertex_list[3].push_back({ vec3(size, -size,  size),vec3(0,0,-1),vec2(1.0f,1.0f) });
        vertex_list[3].push_back({ vec3(-size, -size,  size),vec3(0,0,-1),vec2(0.0f,1.0f) });

        //front
        vertex_list[4].push_back({ vec3(-size,  size, -size),vec3(0,-1,0),vec2(0.0f,0.0f) });
        vertex_list[4].push_back({ vec3(size,  size, -size),vec3(0,-1,0),vec2(1.0f,0.0f) });
        vertex_list[4].push_back({ vec3(size,  size,  size),vec3(0,-1,0),vec2(1.0f,1.0f) });
        vertex_list[4].push_back({ vec3(size,  size,  size),vec3(0,-1,0),vec2(1.0f,1.0f) });
        vertex_list[4].push_back({ vec3(-size,  size,  size),vec3(0,-1,0),vec2(0.0f,1.0f) });
        vertex_list[4].push_back({ vec3(-size,  size, -size),vec3(0,-1,0),vec2(0.0f,0.0f) });

        //back
        vertex_list[5].push_back({ vec3(-size, -size, -size),vec3(0,1,0),vec2(1.0f,0.0f) });
        vertex_list[5].push_back({ vec3(-size, -size,  size),vec3(0,1,0),vec2(1.0f,1.0f) });
        vertex_list[5].push_back({ vec3(size, -size, -size),vec3(0,1,0),vec2(0.0f,0.0f) });
        vertex_list[5].push_back({ vec3(size, -size, -size),vec3(0,1,0),vec2(0.0f,0.0f) });
        vertex_list[5].push_back({ vec3(-size, -size,  size),vec3(0,1,0),vec2(1.0f,1.0f) });
        vertex_list[5].push_back({ vec3(size, -size,  size),vec3(0,1,0),vec2(0.0f,1.0f) });


        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                index_list[i].push_back(j);
            }
            bind_texture(&texture_buffer[i], path[i], window_size);
            glGenBuffers(1, &vertex_buffer[i]);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list[i].size(), &vertex_list[i][0], GL_STATIC_DRAW);
            // geneation of index buffer
            glGenBuffers(1, &index_buffer[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list[i].size(), &index_list[i][0], GL_STATIC_DRAW);
            if (vertex_array[i]) {
                glDeleteVertexArrays(1, &vertex_array[i]);
            }
            vertex_array[i] = cg_create_vertex_array(vertex_buffer[i], index_buffer[i]);
        }
    }
};