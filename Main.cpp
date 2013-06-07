/* Main.cpp
 * The main file for the cityscape.
 * Remember: y = 0 is the ground. z = 0 being the ground means we have
 * to modelview everything straight off the bat to make our initial 
 * viewpoint not be pointing at everything from below, in the sewers. 
 * Too awkward.
 */
#include <iostream>
#include <GL/glew.h>
#include "OpenGLGeneral.h"
#include <vector>
#include "shaders.h"    // Now includes init_shaders.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "CityObject.h"
#include "hw4/array_points_FINAL.hpp"
using namespace std;
using namespace City;
using glm::mat4;
using glm::vec4;
using glm::vec3;

// Glut functions. Note: no update; just use glutPostRedisplay.
void render();
void key_action(int key, int x, int y);
void mouse_action(int button, int x, int y);
void checkExtensions();
void init_OpenGL();
void init_Glut(int argc, char** argv);
void stop();

// Global data.
map<string, unsigned int> vertex_attributes;
map<string, unsigned int> uniforms;
unsigned int program;
mat4 modelview = mat4(1.0);
mat4 projection = mat4(1.0);
vector<CityObject*> objects;
int first = 1;

int main(int argc, char** argv)
{/*
    mat4 rev = mat4(1.0);
    rev[0] = vec4(1.0, 0.0, 0.0, 0.0);
    rev[1] = vec4(0.0, 1.0, 0.0, 0.0);
                    /*vec4(0.0, 0.0, 1.0, 0.0)
                    vec4(0.0, 0.0, 0.0, 1.0)
    cout << rev[1][2] << endl;
    vec3 v1 = vec3(1.0, 0.0, 0.0);
    vec3 v2 = vec3(1.0, 0.0, 0.0);
    vec3 v3 = v2;
    cout << (v1 == v2) << endl;*/
    
    init_Glut(argc, argv);
    init_shaders("city.vert", "city.frag", program);
    // Get locations for all attributes.
    glUseProgram(program);
    vertex_attributes["position"] = glGetAttribLocation(program, "position");
    vertex_attributes["normal"]   = glGetAttribLocation(program, "normal");
    vertex_attributes["tex_coord"] = glGetAttribLocation(program, "tex_coord");
    uniforms["modelview"] = glGetUniformLocation(program, "modelview");
    uniforms["projection"] = glGetUniformLocation(program, "projection");
    uniforms["samp"] = glGetUniformLocation(program, "samp");
    uniforms["light_pos"] = glGetUniformLocation(program, "light_pos");
    glUseProgram(0);
    
    //objects.push_back(new Building(vec3(0.0, 0.0, 0.0), 0.2, 30));
    objects.push_back(new CityObject(building_3, building_3_length));
    objects.push_back(new CityObject(misc_ground, misc_ground_length));
    glutMainLoop();
    
    return 0;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
    modelview = glm::lookAt(vec3(0.0f, -1200.0f, -20.0f), 
        vec3(0.0f, -1200.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f));
    projection = glm::ortho(-1536.0f, 1536.0f, -1536.0f, 1536.0f, 0.0f, 646.4f);
        // debugging
    if (first-- > 0)
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            cout << modelview[i][j] << endl;
    glUniformMatrix4fv(uniforms["modelview"], 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(uniforms["projection"], 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(uniforms["light_pos"], 0.5, 0.5, -0.3);
    for (vector<CityObject*>::iterator it = objects.begin();
         it != objects.end();
         ++it)
    {
        (*it)->render(vertex_attributes, uniforms);
    }
    
    glUseProgram(0);
    glutSwapBuffers();
	glFlush();
}

void key_action(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q':
            stop();
        case 'a':
            modelview *= glm::rotate(modelview, 30.0f, vec3(1.0, 0.0, 0.0));
            break;
        case 'd':
            modelview *= glm::rotate(modelview, 30.0f, vec3(-1.0, 0.0, 0.0));
            break;
        case 'w':
            modelview *= glm::rotate(modelview, 30.0f, vec3(0.0, 1.0, 0.0));
            break;
        case 'x':
            modelview *= glm::rotate(modelview, 30.0f, vec3(0.0, -1.0, 0.0));
            break;
        case 's':
            modelview *= glm::rotate(modelview, 30.0f, vec3(0.0, 0.0, 1.0));
            break;
        case 'c':
            modelview *= glm::rotate(modelview, 30.0f, vec3(0.0, 0.0, -1.0));
            break;
    }
}

void mouse_action(int button, int x, int y)
{
    // Use the mapping from xy coords to a spherical system
    // where the viewer is at the center of the unit sphere.
}

void init_Glut(int argc, char** argv)
{
    /* Change to use cmd line args later. */
    glutInitWindowSize(400, 400);
    glutInit(&argc,argv);
    glutInitDisplayString("samples rgb double depth");
    glutCreateWindow("City");
    glutDisplayFunc(render);
    glutIdleFunc(glutPostRedisplay);
    glutKeyboardUpFunc(key_action);

	init_OpenGL();
}

void init_OpenGL()
{
    glewExperimental = GL_TRUE; 
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::cout << "ERROR: Loading GLEW failed." << std::endl;
        exit(-1);
    }
    checkExtensions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void checkExtensions()
{
    if(glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE)
    {
        std::cout << "ERROR: Shading language extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_vertex_program") != GL_TRUE)
    {
        std::cout << "ERROR: Vertex program extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_vertex_shader") != GL_TRUE)
    {
        std::cout << "ERROR: Vertex shader extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_fragment_program") != GL_TRUE)
    {
        std::cout << "ERROR: Fragment program extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_fragment_shader") != GL_TRUE)
    {
        std::cout << "ERROR: Fragment shader extension not present." << std::endl;
    }
    if(glewGetExtension("GL_ARB_vertex_buffer_object") != GL_TRUE)
    {
        std::cout << "ERROR: VBO extension not present." << std::endl;
    }    
}

void stop()
{   
    glDeleteProgram(program);
    exit(0);
}
