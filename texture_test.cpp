#include <GL/glew.h>
#include "OpenGLGeneral.h"
#include "readBMP.h"
#include "shaders.h"
#include <iostream>
#include <stdexcept>
#include <cassert>
using namespace std;

unsigned int buffer;
unsigned int normals_buffer;
unsigned int textures_buffer;
unsigned int texture;
// Vertex shader variable references.
unsigned int in_Position;
unsigned int in_Normal;
unsigned int in_Tex;
unsigned int texsampl;
unsigned int texon;
// Shader handles and program
unsigned int program;
unsigned int vertex_shader;
unsigned int fragment_shader;  

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
    glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
    
    glDeleteProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
    
    exit(0);
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

void init_shaders(char* vert, char* frag)
{
		program = glCreateProgram();
		int goodv, goods;
		string v, f;
		try
		{
			v = read_shader(vert);
			f = read_shader(frag);
		}
		catch (std::runtime_error re)
		{
			std::cerr << re.what() << endl;
			exit(1);
		}
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		const char* verts = v.data();	// This way we can use the C++ string class; no danger of memory leaks.
		glShaderSource(vertex_shader, 1, (const GLcharARB **)&verts, NULL);
		glCompileShader(vertex_shader);
		glAttachShader(program, vertex_shader);
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &goodv);
		print_info_log(vertex_shader);
		assert(goodv == GL_TRUE);
		
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* frags = f.data();
		glShaderSource(fragment_shader, 1, (const GLcharARB **)&frags, NULL);
		glCompileShader(fragment_shader);
		glAttachShader(program, fragment_shader);
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &goods);
		print_info_log(fragment_shader);
		assert(goods == GL_TRUE);

		glLinkProgram(program);
		/* Get handles to all needed attributes here */
		glUseProgram(program);
		in_Position = glGetAttribLocation(program, "in_Position");
		in_Normal = glGetAttribLocation(program, "in_Normal");
        in_Tex = glGetAttribLocation(program, "in_Tex");
        texsampl = glGetUniformLocation(program, "texsampl");
        texon = glGetUniformLocation(program, "texon");
        cout << texsampl << " " << texon << endl;
		glUseProgram(0);
}

void start()
{
    glutMainLoop();
}

void update()
{
    glutPostRedisplay();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    /*glEnableVertexAttribArray(in_Position);
    glEnableVertexAttribArray(in_Tex);*/
    //cout << texsampl << " " << texon << endl;
    glUniform1i(texsampl, 0);
    glUniform1i(texon, 1);
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glUseProgram(0);
    glutSwapBuffers();
	glFlush();
}

int main(int argc, char** argv)
{
    glutInitWindowSize(400, 400);
    glutInit(&argc,argv);
    glutInitDisplayString("samples rgb double depth");
    glutCreateWindow("Texture Test");
    glutDisplayFunc(render);
    glutIdleFunc(update);

	init_OpenGL();
	init_shaders("vase.vert", "vase.frag");

    float data[] = {
    //vertices
    //Bottom face
    0,0,0,
    1,0,0,
    1,0,1,
    0,0,0,
    1,0,1,
    0,0,1,
	
    // Top 
    0,1,0,
    0,1,1,
    1,1,1,
    0,1,0,
    1,1,1,
    1,1,0,
	
    // Front 
    0,0,1,
    1,0,1,
    1,1,1,
    0,0,1,
    1,1,1,
    0,1,1,
	
    // Back 
    0,0,0,
    0,1,0,
    1,1,0,
    0,0,0,
    1,1,0,
    1,0,0,
    
    // Right 
    1,0,0,
    1,1,0,
    1,1,1,
    1,0,0,
    1,1,1,
    1,0,1,
	
    // Left 
    0,0,0,
    0,0,1,
    0,1,1,
    0,0,0,
    0,1,1,
    0,1,0,
        
    //normals
    //bottom face
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
        
    // Top 
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
        
    // Front 
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
        
    // Back 
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,
    
    // Right 
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,
        
    // Left 
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0};
    /* For some unknown reason it insists that the buffer data be
     * in local scope when you bind the buffer, even if it's actually
     * visible from whatever scope it's at.
     */
     
    GLfloat cubedat[2*4*2] = {  
     // front  
     0.0, 0.0,  
     1.0, 0.0,  
     1.0, 1.0,  
     0.0, 1.0, 
     // Texture coordinates
     0.0, 0.0,  
     1.0, 0.0,  
     1.0, 1.0,  
     0.0, 1.0,
   }; 
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), 
        cubedat, GL_STATIC_DRAW);
	glEnableVertexAttribArray(in_Position);
	glVertexAttribPointer(in_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);/*
	glEnableVertexAttribArray(in_Normal);
	glVertexAttribPointer(in_Normal, 3, GL_FLOAT, GL_FALSE, 0,
        (void*)(108*sizeof(float)));*/
    glEnableVertexAttribArray(in_Tex);
    glVertexAttribPointer(in_Tex, 2, GL_FLOAT, GL_FALSE, 0, (void*)(8*sizeof(float)));
    
    Image cubetex;
    assert(ImageLoad("post-office-small.bmp", &cubetex));
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, // the current 2D texture
	       0, // Mipmap level
	       GL_RGB, //internal format for texture
	       cubetex.sizeX, // size in s coord
	       cubetex.sizeY, // size in t coord
	       0, // should always be 0
	       GL_RGB, // incoming data format; should match internal
	       GL_UNSIGNED_BYTE, // type of incoming data
	       cubetex.data // pointer to the data
	       );
    /*
    glGenBuffers(1, &textures_buffer);  
    glBindBuffer(GL_ARRAY_BUFFER, textures_buffer);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), cubedat, GL_STATIC_DRAW);*/

    
    
	start();
	return 0;
}
