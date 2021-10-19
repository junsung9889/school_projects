
#include "cgmath.h"         // slee's simple math library
#include "cgut.h"      // slee's OpenGL utility
#include "cgut2.h"         // slee's OpenGL utility
#include "trackball.h"
#include "assimp_loader.h"
#include "skybox.h"
#include "CrossHair.h"
#include "item.h"
#include "irrKlang\irrKlang.h"
#include "particle.h"
#include "BackgroundSaturn.h"
#include "BackgroundSaturnRing.h"
#include "BackgroundSaturnRingTrans.h"
#pragma comment(lib, "irrKlang.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define random(n) rand()%n
#define MAX_DEATH_COUNT 5
#define MAX_ENEMY_COUNT 8

bool init_text();
void render_text(std::string text, GLint x, GLint y, GLfloat scale, vec4 color, GLfloat dpi_scale = 1.0f);

//*************************************
// global constants
static const char* window_name = "cgmodel - assimp for loading {obj|3ds} files";
static const char* vert_shader_path = "../bin/shaders/model.vert";
static const char* frag_shader_path = "../bin/shaders/model.frag";
static const char* mp3_path = "../bin/sounds/blaster.mp3";
static const char* background_mp3_path = "../bin/sounds/BackgroundMusic.mp3";
static const char* enemy_mp3_path = "../bin/sounds/enemydeath.mp3";
static const char* get_skill_sound_path = "../bin/sounds/getskill.mp3";

static const char* image_path = "../bin/images/fire.png";

//*************************************
// common structures
struct camera
{
    vec3   eye = vec3(0, 10, -55);
    vec3   at = vec3(0, 5, -30);
    vec3   up = vec3(0, 1, 0);
    mat4   view_matrix = mat4::look_at(eye, at, up);
    float   fovy = PI / 4.0f; // must be in radian
    float   aspect_ratio;
    float   dNear = 0.01f;
    float   dFar = 100000.0f;
    mat4   projection_matrix;
};
struct light_t
{
    vec4   position = vec4(10.0f, 10.0f, -30.0f, 1.0f);   // directional light
    vec4   ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    vec4   diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4   specular = vec4(10.0f, 10.0f, 10.0f, 10.0f);
};

//*************************************
// window objects
GLFWwindow* window = nullptr;
//ivec2      window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint   program = 0;   // ID holder for GPU program
GLuint		vertex_array = 0;	// ID holder for vertex array object
GLuint		TEX = 0;			// sprite texture

//*************************************
// global variables
int     frame = 0;  
int     MouseButton = 0;
int     MouseAction = 0;
int     MouseMod = 0;
bool    show_texcoord = false;
bool    isTitle = true;
bool    isMode = true;
bool    isEasy = false;
bool    isNormal = false;
bool    isHard = false;
bool    isMain = true;
bool    isEnd = true;
bool    isCompleted = true;
bool    isContinue = true;
bool    shot_animation = false;
bool    shot = false;
bool    isHelp = false;
uint    current_enemy_count = 0;
uint    kill_count = 0;
uint    death_count = 0;
uint    skill_count = 0;
float   _time = 0.0f;
float   item_time = 0.0f;
float   a = 0.0f;
float   level_easy_spead_factor = 0.0025f;
float   level_normal_spead_factor = 0.0035f;
float   level_hard_spead_factor = 0.05f;
float   item_spead_factor = 0.0001f;
float   sound_start_time = 0.0f;
float   enemy_sound_start_time = 0.0f;
float   skill_sound_start_time = 0.0f;


//*************************************
std::vector<mesh2*>   pMeshes;
std::vector<mesh_enemy*> pEnemys;
std::vector<Item*> items;
std::vector<particle_t> particles;
camera      cam;
light_t      light;
struct universe Universe;
Skycube* skycube;
CrossHair* crossHair;
Saturn* saturn;
SaturnRing* saturnRing;
SaturnRingTrans* saturnRingTrans;

irrklang::ISoundEngine* engine;
irrklang::ISoundSource* wave_src = nullptr;
irrklang::ISoundSource* mp3_src = nullptr;

irrklang::ISoundEngine* BackgroundEngine;
irrklang::ISoundSource* BackgroundSource = nullptr;

irrklang::ISoundEngine* EnemyDeathEngine;
irrklang::ISoundSource* EnemyDeathSoundSource = nullptr;

irrklang::ISoundEngine* SkillSoundEngine;
irrklang::ISoundSource* SkillSoundSource = nullptr;

//*************************************

bool mesh_enemy::CheckHitted(vec3 CameraPos)
{
    vec3 u_vector = cam.at - cam.eye;

    float dist = (center - cam.eye).cross(u_vector).length() / u_vector.length();
    bool hit = false;
    if (abs(dist) < 2.0f) {
        hit_count += abs(1 / dist) * 2.0f;
        hit = true;
    }

    if (hit_count >= 3.0f) {
        printf("enemy death!!\n");
        enemy_death = true;
    }
    return hit;
}
bool Item::GetItem(vec3 CameraPos) {
    vec3 u_vector = cam.at - cam.eye;

    float dist = (center - cam.eye).cross(u_vector).length() / u_vector.length();
    bool hit = false;
    if (abs(dist) < 5.0f) {
        hit_count += abs(1 / dist) * 2.0f;
        hit = true;
    }
    if (hit_count >= 8.0f) {
        get_item = true;
    }
    return hit;
}
void use_skill() {
    if (skill_count == 0) {
        printf("no skill count\n");
        return;
    }
    skill_count -= 1;
    if (SkillSoundEngine->isCurrentlyPlaying(SkillSoundSource)) { SkillSoundEngine->stopAllSounds(); }
    SkillSoundEngine->play2D(SkillSoundSource, true);
    skill_sound_start_time = float(glfwGetTime());

    for (auto& a : pEnemys) {
        a->enemy_death = true;
    }
}
void create_enemy() {
    mesh_enemy* new_enemy = enemy_load_model("../bin/mesh/rock/rock.obj");
    new_enemy->center = vec3(float(random(150) - 75), float(random(150) - 75), float(random(30) + 30));
    if (isEasy)
        new_enemy->speed = float(random(100) + 10) * level_easy_spead_factor;
    else if (isNormal)
        new_enemy->speed = float(random(100) + 10) * level_normal_spead_factor;
    else if (isHard)
        new_enemy->speed = float(random(100) + 10) * level_hard_spead_factor;
    pEnemys.emplace_back(new_enemy);
    // printf("new enemy created center = %f %f %f speed = %f\n", new_enemy->center.x, new_enemy->center.y, new_enemy->center.z, new_enemy->speed);
}
void create_particle(vec3 c) {
    for (int i = 0; i < particle_t::MAX_PARTICLES; i++)
    {
        particle_t p;
        p.center = c;
        p.reset();
        particles.emplace_back(p);
    }
}
void enemy_clear() {
    std::vector<mesh_enemy*>::iterator it;
    for (it = pEnemys.begin(); it != pEnemys.end();) {
        if ((*it)->enemy_hit_with_spaceship || (*it)->enemy_death) {
            if ((*it)->enemy_hit_with_spaceship) {
                death_count += 1;
                current_enemy_count -= 1;
            }
            else {
                kill_count += 1;
                current_enemy_count -= 1;
                create_particle((*it)->center);
                if (EnemyDeathEngine->isCurrentlyPlaying(EnemyDeathSoundSource)) { EnemyDeathEngine->stopAllSounds(); }
                EnemyDeathEngine->play2D(EnemyDeathSoundSource, true);
                enemy_sound_start_time = float(glfwGetTime());
                // printf("start Enemy Sound\n");
            }
            it = pEnemys.erase(it);
        }
        else {
            it++;
        }
    }
}
void item_clear() {
    std::vector<Item*>::iterator it;
    for (it = items.begin(); it != items.end();) {
        if ((*it)->getItemGet() || (*it)->getItemGone()) {
            if ((*it)->getItemGet()) {
                skill_count += 1;
                printf("skill count increase skill count=%d\n", skill_count);
            }
            it = items.erase(it);
        }
        else {
            it++;
        }
    }
}
void reset() {
    pEnemys.clear();
    items.clear();
    particles.clear();
    cam.eye = vec3(0, 10, -55);
    cam.at = vec3(0, 5, -30);
    cam.up = vec3(0, 1, 0);
    cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);

    current_enemy_count = 0;
    kill_count = 0;
    death_count = 0;
    skill_count = 0;

    shot = false;
    shot_animation = false;

    if (engine->isCurrentlyPlaying(mp3_src)) {
        engine->stopAllSounds();
    }
    if (EnemyDeathEngine->isCurrentlyPlaying(EnemyDeathSoundSource)) {
        EnemyDeathEngine->stopAllSounds();
    }
    if (SkillSoundEngine->isCurrentlyPlaying(SkillSoundSource)) {
        SkillSoundEngine->stopAllSounds();
    }
}
void title()
{
    // clear screen (with background color) and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // notify GL that we use our own program
    glUseProgram(program);
    GLint uloc;
    uloc = glGetUniformLocation(program, "view_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
    uloc = glGetUniformLocation(program, "projection_matrix");   if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);

    skycube->render(program);
    float dpi_scale = cg_get_dpi_scale();
    render_text("@@@Star Wars@@@", window_size.x * 17 / 72, window_size.y / 3, 1.7f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    render_text("Press Enter", window_size.x * 21 / 54, window_size.y * 3 / 4, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    glfwSwapBuffers(window);
}
void selectMode()
{
    // clear screen (with background color) and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // notify GL that we use our own program
    glUseProgram(program);

    GLint uloc;
    uloc = glGetUniformLocation(program, "view_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
    uloc = glGetUniformLocation(program, "projection_matrix");   if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);

    skycube->render(program);
    float dpi_scale = cg_get_dpi_scale();
    render_text("Press Number to select Mode", window_size.x * 11 / 40, window_size.y / 4, 0.8f, vec4(1.0f, 1.0f, 1.0f, 1.0f), dpi_scale);
    render_text("1. Easy", window_size.x * 2 / 5, window_size.y * 4 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    render_text("2. Normal", window_size.x * 2 / 5, window_size.y * 5 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    render_text("3. Hard", window_size.x * 2 / 5, window_size.y * 6 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    reset();
    glfwSwapBuffers(window);
}
void completeMode()
{
    // clear screen (with background color) and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // notify GL that we use our own program
    glUseProgram(program);
    GLint uloc;
    uloc = glGetUniformLocation(program, "view_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
    uloc = glGetUniformLocation(program, "projection_matrix");   if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);

    skycube->render(program);
    float dpi_scale = cg_get_dpi_scale();
    render_text("!!!Completed!!!", window_size.x * 7 / 30, window_size.y * 3 / 8, 1.7f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    render_text("Press 'i' to insert coin", window_size.x * 12 / 40, window_size.y * 5 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    render_text("Press 'esc' to exit", window_size.x * 14 / 40, window_size.y * 6 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    glfwSwapBuffers(window);
}
void end()
{
    // clear screen (with background color) and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // notify GL that we use our own program
    glUseProgram(program);

    GLint uloc;
    uloc = glGetUniformLocation(program, "view_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
    uloc = glGetUniformLocation(program, "projection_matrix");   if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
    skycube->render(program);
    float dpi_scale = cg_get_dpi_scale();
    render_text("...Game End...", window_size.x * 17 / 72, window_size.y * 3 / 8, 1.7f, vec4(1.0f, 0.0f, 0.0f, 1.0f), dpi_scale);
    render_text("Press 'i' to insert coin", window_size.x * 12 / 40, window_size.y * 5 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    render_text("Press 'esc' to exit", window_size.x * 14 / 40, window_size.y * 6 / 8, 0.8f, vec4(0.5f, 0.7f, 0.7f, a), dpi_scale);
    glfwSwapBuffers(window);
}
void update()
{
    glUseProgram(program);
    for (auto& p : particles) p.update();
    // update projection matrix
    cam.aspect_ratio = window_size.x / float(window_size.y);
    cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect_ratio, cam.dNear, cam.dFar);
    // build the model matrix for oscillating scale
    float t = float(glfwGetTime());

    if (float(glfwGetTime() - _time) > 0.5f && current_enemy_count < MAX_ENEMY_COUNT) {
        create_enemy();
        current_enemy_count += 1;
        _time = float(glfwGetTime());
    }
    if (float(glfwGetTime() - item_time) > 10.0f && items.size() < 1) {
        Item* new_item = new Item();
        new_item->setCenter(vec3(float(random(150) - 75), float(random(150) - 75), float(random(30) + 30)));
        new_item->setSpeed(150 * item_spead_factor);
        items.push_back(new_item);
        item_time = float(glfwGetTime());
    }
    // update uniform variables in vertex/fragment shaders

    a = abs(sin(float(glfwGetTime()) * 2.5f));

    // setup light properties
    glUniform4fv(glGetUniformLocation(program, "light_position"), 1, light.position);
    glUniform4fv(glGetUniformLocation(program, "Ia"), 1, light.ambient);
    glUniform4fv(glGetUniformLocation(program, "Id"), 1, light.diffuse);
    glUniform4fv(glGetUniformLocation(program, "Is"), 1, light.specular);
    glUniform1i(glGetUniformLocation(program, "TEX0"), 0);
    glUniform1i(glGetUniformLocation(program, "TEX1"), 1);
    glUniform1i(glGetUniformLocation(program, "TEX2"), 2);
    glUniform1i(glGetUniformLocation(program, "TEX3"), 3);
    glUniform1i(glGetUniformLocation(program, "TEX4"), 4);
    glUniform1i(glGetUniformLocation(program, "TEX5"), 5);
    glUniform1i(glGetUniformLocation(program, "TEX6"), 6);
    glUniform1i(glGetUniformLocation(program, "TEX7"), 7);
    glUniform1i(glGetUniformLocation(program, "TEX8"), 8);
}
void render()
{
    // clear screen (with background color) and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // notify GL that we use our own program
    glUseProgram(program);

    if (engine->isCurrentlyPlaying(mp3_src) && float(glfwGetTime()) - sound_start_time > 0.7f) {
        engine->stopAllSounds();
    }
    if (EnemyDeathEngine->isCurrentlyPlaying(EnemyDeathSoundSource) && float(glfwGetTime()) - enemy_sound_start_time > 0.3f) {
        EnemyDeathEngine->stopAllSounds();
        // printf("stop Enemy Sound\n");
    }
    if (SkillSoundEngine->isCurrentlyPlaying(SkillSoundSource) && float(glfwGetTime()) - skill_sound_start_time > 1.4f) {
        SkillSoundEngine->stopAllSounds();
    }
    for (auto& pMesh : pMeshes) {
        // bind vertex array object
        glBindVertexArray(pMesh->vertex_array);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);

        pMesh->update(shot_animation);
        if (shot_animation && pMeshes[0]->center.z == -30.0f) {
            shot_animation = false;
            printf("animation set false.....%d\n", pMeshes.size());
        }

        GLint uloc;
        uloc = glGetUniformLocation(program, "view_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
        uloc = glGetUniformLocation(program, "projection_matrix");   if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
        uloc = glGetUniformLocation(program, "model_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, pMesh->model_matrix);
        for (size_t k = 0, kn = pMesh->geometry_list.size(); k < kn; k++) {
            geometry& g = pMesh->geometry_list[k];
            material* m = g.mat;
            glActiveTexture(GL_TEXTURE3);
            glUniform1i(glGetUniformLocation(program, "mode"), 3);
            glBindTexture(GL_TEXTURE_2D, m->textures.diffuse->id);

            // setup material properties
            glUniform4fv(glGetUniformLocation(program, "Ka"), 1, m->ambient);
            glUniform4fv(glGetUniformLocation(program, "Kd"), 1, m->diffuse);
            glUniform4fv(glGetUniformLocation(program, "Ks"), 1, m->specular);
            glUniform1f(glGetUniformLocation(program, "shininess"), m->shininess);

            // render vertices: trigger shader programs to process vertex data
            glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
        }
    }
    enemy_clear();
    item_clear();
    for (auto& pEnemy : pEnemys) {
        bool hit_flag = false;
        if (shot) {
            hit_flag = pEnemy->CheckHitted(crossHair->getCenter());
            if (hit_flag) { printf("enemy hit!!!!\n");  shot = false; }
        }

        glBindVertexArray(pEnemy->vertex_array);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEnemy->index_buffer);
        pEnemy->update();
        GLint uloc;
        uloc = glGetUniformLocation(program, "view_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
        uloc = glGetUniformLocation(program, "projection_matrix");   if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
        uloc = glGetUniformLocation(program, "model_matrix");         if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, pEnemy->model_matrix);

        if (pEnemy->hit_count > 0) { glUniform1i(glGetUniformLocation(program, "hit"), 1); }
        else glUniform1i(glGetUniformLocation(program, "hit"), 0);
        for (size_t k = 0, kn = pEnemy->geometry_list.size(); k < kn; k++) {
            geometry& g = pEnemy->geometry_list[k];
            material* m = g.mat;
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(program, "mode"), 0);
            glBindTexture(GL_TEXTURE_2D, m->textures.diffuse->id);

            // setup material properties
            glUniform4fv(glGetUniformLocation(program, "Ka"), 1, m->ambient);
            glUniform4fv(glGetUniformLocation(program, "Kd"), 1, m->diffuse);
            glUniform4fv(glGetUniformLocation(program, "Ks"), 1, m->specular);
            glUniform1f(glGetUniformLocation(program, "shininess"), m->shininess);

            // render vertices: trigger shader programs to process vertex data
            glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
        }
    }
    skycube->render(program);
    crossHair->render(program);
    saturn->render(program);
    saturnRing->render(program);
    saturnRingTrans->render(program);

    glBindVertexArray(vertex_array);
    glActiveTexture(GL_TEXTURE5);
    glUniform1i(glGetUniformLocation(program, "mode"), 5);
    glBindTexture(GL_TEXTURE_2D, TEX);
    for (auto& p : particles)
    {
        p.update2();
        GLint uloc;
        uloc = glGetUniformLocation(program, "color");			if (uloc > -1) glUniform4fv(uloc, 1, p.color);
        uloc = glGetUniformLocation(program, "model_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, p.model_matrix);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    for (auto& item : items) { if (shot) { bool item_hit = item->GetItem(crossHair->getCenter()); if (item_hit) { shot = false; } } }
    for (auto& item : items) { item->render(program); }
    float dpi_scale = cg_get_dpi_scale();
    std::string text = "Total Life : ";
    text += std::to_string(MAX_DEATH_COUNT - death_count);
    render_text(text, window_size.x / 20, window_size.y * 6 / 7, 0.5f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    text = "Total Skill : ";
    text += std::to_string(skill_count);
    render_text(text, window_size.x * 8 / 20, window_size.y * 6 / 7, 0.5f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    text = "Total Kill : ";
    text += std::to_string(kill_count);
    render_text(text, window_size.x * 16 / 20, window_size.y * 6 / 7, 0.5f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    glfwSwapBuffers(window);
}
void reshape(GLFWwindow* window, int width, int height)
{
    // set current viewport in pixels (win_x, win_y, win_width, win_height)
    // viewport: the window area that are affected by rendering 
    window_size = ivec2(width, height);
    glViewport(0, 0, width, height);
}
void print_help()
{
    // clear screen (with background color) and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // notify GL that we use our own program
    glUseProgram(program);
    float dpi_scale = cg_get_dpi_scale();
    render_text("/HELP/", window_size.x * 1 / 8, window_size.y / 7, 1.3f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    render_text("Shoot enemy by press 'SPACE' button", window_size.x * 1 / 8, window_size.y * 2 / 7, 0.8f, vec4(1.0f, 1.0f, 1.0f, 1.0f), dpi_scale);
    render_text("Use skill by press 'A' button", window_size.x * 1 / 8, window_size.y * 3 / 7, 0.8f, vec4(1.0f, 1.0f, 1.0f, 1.0f), dpi_scale);
    render_text("When total life becomes 0, game is over", window_size.x * 1 / 8, window_size.y * 4 / 7, 0.8f, vec4(1.0f, 1.0f, 1.0f, 1.0f), dpi_scale);
    render_text("When total kill becomes 30, the mode is completed", window_size.x * 1 / 8, window_size.y * 5 / 7, 0.8f, vec4(1.0f, 1.0f, 1.0f, 1.0f), dpi_scale);
    render_text("<Press 'F1' to go back>", window_size.x * 1 / 8, window_size.y * 6 / 7, 0.8f, vec4(1.0f, 1.0f, 0.2f, 1.0f), dpi_scale);
    glfwSwapBuffers(window);
}
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_H || key == GLFW_KEY_F1) {
            isHelp = !isHelp;
        }

        else if (key == GLFW_KEY_SPACE)
        {
            if (engine->isCurrentlyPlaying(mp3_src)) { engine->stopAllSounds(); }
            engine->play2D(mp3_src, true);
            sound_start_time = float(glfwGetTime());
            shot = true;
            shot_animation = true;
        }
        else if (key == GLFW_KEY_ENTER)
        {
            isTitle = false;
            printf("%d %d\n", window_size.x, window_size.y);
        }
        else if (key == GLFW_KEY_ESCAPE)
        {
            isContinue = false;
            isCompleted = false;
            isEnd = false;
        }
        else if (key == GLFW_KEY_1)
        {
            isMode = false;
            isEasy = true;
        }
        else if (key == GLFW_KEY_2)
        {
            isMode = false;
            isNormal = true;
        }
        else if (key == GLFW_KEY_3)
        {
            isMode = false;
            isHard = true;
        }
        else if (key == GLFW_KEY_I)
        {
            isCompleted = false;
            isEnd = false;
            isContinue = true;
        }
        else if (key == GLFW_KEY_R) {
            reset();
        }
        else if (key == GLFW_KEY_A)
        {
            use_skill();
        }
    }
}
void mouse(GLFWwindow* window, int button, int action, int mods)
{
    dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
    vec2 npos = cursor_to_ndc(pos, window_size);

    MouseButton = button;
    MouseAction = action;
    MouseMod = mods;

    if (MouseButton == GLFW_MOUSE_BUTTON_LEFT && MouseMod == 0)
    {
        if (MouseAction == GLFW_PRESS)         Universe.begin(cam.view_matrix, npos, cam.eye, cam.at, cam.up);
        else if (MouseAction == GLFW_RELEASE) {
            Universe.end();
            cam.eye = vec3(0, 10, -60);
            cam.at = vec3(0, 5, -30);
            cam.up = vec3(0, 1, 0);
            cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);
            pMeshes[0]->rotation_matrix = mat4::identity();
            crossHair->setRotationMatrix(mat4::identity());
        }
    }
    else if (MouseButton == GLFW_MOUSE_BUTTON_MIDDLE || (MouseButton == GLFW_MOUSE_BUTTON_LEFT && (MouseMod & GLFW_MOD_CONTROL)))
    {
        if (MouseAction == GLFW_PRESS)         Universe.panning_begin(cam.view_matrix, npos, cam.eye, cam.at, cam.up);
        else if (MouseAction == GLFW_RELEASE) {
            Universe.end();
        }
    }
    else if (MouseButton == GLFW_MOUSE_BUTTON_RIGHT || (MouseButton == GLFW_MOUSE_BUTTON_LEFT && (MouseMod & GLFW_MOD_SHIFT)))
    {
        if (MouseAction == GLFW_PRESS)         Universe.zooming_begin(cam.view_matrix, npos, cam.eye, cam.at, cam.up);
        else if (MouseAction == GLFW_RELEASE) {
            Universe.end();
        }
    }
}
void motion(GLFWwindow* window, double x, double y)
{
    dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
    vec2 npos = cursor_to_ndc(pos, window_size);

    if (Universe.b_tracking && MouseButton == GLFW_MOUSE_BUTTON_LEFT && MouseMod == 0) {
        vec3 original_eye = cam.eye;
        cam.eye = Universe.update_eye(npos, Universe.eye0, Universe.at0, Universe.up0);
        //cam.at = Universe.update_at(npos, Universe.eye0, Universe.at0, Universe.up0);
        cam.up = Universe.update_up(npos, Universe.eye0, Universe.at0, Universe.up0);
        cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);
        pMeshes[0]->rotation_matrix = Universe.r_matrix;
        crossHair->setRotationMatrix(Universe.r_matrix);
        //crossHair->setCenter(cam.at);
    }
    else if (Universe.b_panning && (MouseButton == GLFW_MOUSE_BUTTON_MIDDLE || (MouseButton == GLFW_MOUSE_BUTTON_LEFT && (MouseMod & GLFW_MOD_CONTROL))))
    {
        vec3 diff = Universe.update_pan(npos, Universe.eye0, Universe.at0, Universe.up0);
        //cam.eye = Universe.eye0 - diff;
        //cam.at = Universe.at0 - diff;
        vec3 original_eye = cam.eye;
        cam.eye = Universe.eye0 - diff;
        cam.at = Universe.at0 - diff;
        pMeshes[0]->center += original_eye - cam.eye;
        //cam.view_matrix = mat4::look_at((cam.eye), (cam.at), cam.up);
    }
    else if (Universe.b_zooming && (MouseButton == GLFW_MOUSE_BUTTON_RIGHT || (MouseButton == GLFW_MOUSE_BUTTON_LEFT && (MouseMod & GLFW_MOD_SHIFT))))
    {
        vec3 zooming_factor = Universe.update_zoom(npos, Universe.eye0, Universe.at0, Universe.up0);
        vec3 n_to_eye0 = cam.eye - Universe.at0;

        if ((n_to_eye0 + zooming_factor).normalize().operator==((n_to_eye0).normalize())) {
            cam.eye = Universe.eye0 + zooming_factor;
        }
        cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);
    }
}
bool user_init()
{
    // init GL states
    glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);   // set clear color
    // glEnable(GL_CULL_FACE);                        // turn on backface culling
    glEnable(GL_DEPTH_TEST);                        // turn on depth tests
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    skycube = new Skycube();
    crossHair = new CrossHair();
    saturn = new Saturn();
    saturnRing = new SaturnRing();
    saturnRingTrans = new SaturnRingTrans();

    // animation = new Animation();

    // setup freetype
    if (!init_text()) return false;

    // load the mesh
    pMeshes.clear();
    pMeshes.emplace_back(load_model("../bin/mesh/Intergalactic_Spaceship-(Wavefront)/Intergalactic_Spaceship-(Wavefront).obj"));
    pMeshes[0]->center = vec3(0, 0, -30);

    pEnemys.clear();
    items.clear();

    engine = irrklang::createIrrKlangDevice();
    if (!engine) return false;

    engine->setSoundVolume(0.2f);
    mp3_src = engine->addSoundSourceFromFile(mp3_path);

    BackgroundEngine = irrklang::createIrrKlangDevice();
    if (!BackgroundEngine) return false;
    BackgroundEngine->setSoundVolume(0.15f);
    BackgroundSource = BackgroundEngine->addSoundSourceFromFile(background_mp3_path);
    BackgroundEngine->play2D(background_mp3_path, true);

    EnemyDeathEngine = irrklang::createIrrKlangDevice();
    if (!EnemyDeathEngine) return false;
    EnemyDeathEngine->setSoundVolume(0.4f);
    EnemyDeathSoundSource = EnemyDeathEngine->addSoundSourceFromFile(enemy_mp3_path);


    SkillSoundEngine = irrklang::createIrrKlangDevice();
    if (!SkillSoundEngine) return false;
    SkillSoundEngine->setSoundVolume(0.7f);
    SkillSoundSource = SkillSoundEngine->addSoundSourceFromFile(get_skill_sound_path);


    static vertex vertices[] = { {vec3(-10,-10,0),vec3(-1,-1,0).normalize(),vec2(0,0)}, {vec3(10,-10,0),vec3(1,-1,0).normalize(),vec2(1,0)}, {vec3(-10,10,0),vec3(-1,1,0).normalize(),vec2(0,1)}, {vec3(10,10,0),vec3(1,1,0).normalize(),vec2(1,1)} }; // strip ordering [0, 1, 3, 2]

    // generation of vertex buffer: use vertices as it is
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 4, &vertices[0], GL_STATIC_DRAW);

    // generate vertex array object, which is mandatory for OpenGL 3.3 and higher
    if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
    vertex_array = cg_create_vertex_array(vertex_buffer);
    if (!vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return false; }

    // create a particle texture from image with alpha
    TEX = cg_create_texture(image_path, false); if (!TEX) return false; // disable mipmapping

    // initialize particles
    // particles.resize(particle_t::MAX_PARTICLES);

    return true;
}
void user_finalize()
{
    delete_texture_cache();
    engine->drop();
    BackgroundEngine->drop();
    EnemyDeathEngine->drop();
    SkillSoundEngine->drop();
}
bool isWindowClose() {
    if (glfwWindowShouldClose(window)) {
        isMain = false;
        isContinue = false;
        isCompleted = false;
        isEnd = false;
        return true;
    }
    return false;
}
int main(int argc, char* argv[])
{
    // create window and initialize OpenGL extensions
    if (!(window = cg_create_window(window_name, window_size.x, window_size.y))) { glfwTerminate(); return 1; }
    if (!cg_init_extensions(window)) { glfwTerminate(); return 1; }   // version and extensions

    // initializations and validations
    if (!(program = cg_create_program(vert_shader_path, frag_shader_path))) { glfwTerminate(); return 1; }   // create and compile shaders/program
    if (!user_init()) { printf("Failed to user_init()\n"); glfwTerminate(); return 1; }               // user initialization

    glfwSetWindowTitle(window, "Team Billionair OpenGl Game Project");

    // register event callbacks

    glfwSetWindowSizeCallback(window, reshape);   // callback for window resizing events
    glfwSetKeyCallback(window, keyboard);         // callback for keyboard events
    glfwSetMouseButtonCallback(window, mouse);   // callback for mouse click inputs
    glfwSetCursorPosCallback(window, motion);      // callback for mouse movement

    while (isTitle) {
        if (isWindowClose()) break;
        glfwPollEvents();
        update();
        title();
        while (isHelp) {
            if (isWindowClose()) break;
            glfwPollEvents();
            print_help();
         }
    }
    while (isContinue) {
        isMode = true;
        isEasy = false;
        isNormal = false;
        isHard = false;
        while (isMode)
        {
            if (isWindowClose()) break;
            engine->stopAllSounds();
            EnemyDeathEngine->stopAllSounds();
            SkillSoundEngine->stopAllSounds();
            // if (skill_sound_start_time - float(glfwGetTime()) > 1.4f && SkillSoundEngine->isCurrentlyPlaying(SkillSoundSource)) { SkillSoundEngine->stopAllSounds(); }
            glfwPollEvents();
            update();
            selectMode();
            item_time = float(glfwGetTime());
            _time = float(glfwGetTime());
            while (isHelp) {
                if (isWindowClose()) break;
                glfwPollEvents();
                print_help();
            }
        }
        // enters rendering/event loop 
        float t1 = (float)glfwGetTime();
        while(isMain)
        {
            float t2 = (float)glfwGetTime();
            if (t2 - t1 >= 0.016f) {
                if (isWindowClose()) break;
                if (death_count >= 5) {
                    isContinue = false;
                    isCompleted = false;
                    break;
                }
                if (kill_count >= 40) {
                    break;
                }
                glfwPollEvents();   // polling and processing of events
                update();         // per-frame update
                render();         // per-frame render
                while (isHelp) {
                    if (isWindowClose()) break;
                    glfwPollEvents();
                    print_help();
                }
            t1 = t2;
            }
        }
        death_count = 0;
        kill_count = 0;
        while (isCompleted)
        {
            if (isWindowClose()) break;
            engine->stopAllSounds();
            EnemyDeathEngine->stopAllSounds();
            SkillSoundEngine->stopAllSounds();
            glfwPollEvents();
            update();
            completeMode();
            item_time = float(glfwGetTime());
            _time = float(glfwGetTime());
            while (isHelp) {
                if (isWindowClose()) break;
                glfwPollEvents();
                print_help();
            }
        }
        isCompleted = true;
        while (isEnd)
        {
            if (isWindowClose()) break;
            engine->stopAllSounds();
            EnemyDeathEngine->stopAllSounds();
            SkillSoundEngine->stopAllSounds();
            glfwPollEvents();
            update();
            end();
            while (isHelp) {
                if (isWindowClose()) break;
                glfwPollEvents();
                print_help();
            };
        }
        isEnd = true;
    }
    user_finalize();
    // normal termination
    cg_destroy_window(window);
    return 0;
}
