// My own versions of the provided shader utilities. I didn't like the idea
// of having to call delete on the strings so I'm using the C++ string class.
#include "shaders.h"
#include <GL/glew.h>
#include "OpenGLGeneral.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ostringstream;

string read_shader(const char * filename)
{
	ifstream file(filename);
	string s;
	ostringstream content;
	if(file.good())
	{
		while (getline(file, s))
			content << s << endl;
		return content.str();
	}
	else
	{
		throw std::runtime_error("Shader does not exist");
	}
}


void print_info_log(unsigned int obj)
{
	int infoll = 0;
	glGetShaderiv(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoll);
    
	if(infoll > 0)
	{
		char *infoLog = new char[infoll];
        int charsWritten = 0;
		glGetShaderInfoLog(obj, infoll, &charsWritten, infoLog);
        cout <<  infoLog << endl;
		delete[] infoLog;
	}
}

void init_shaders(string vert, string frag, unsigned int& program)
{
		program = glCreateProgram();
		int goodv, goods;
		string v, f;
		try
		{
			v = read_shader(vert.data());
			f = read_shader(frag.data());
		}
		catch (std::runtime_error re)
		{
			std::cerr << re.what() << endl;
			exit(1);
		}
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		const char* verts = v.data();	// This way we can use the C++ string class; no danger of memory leaks.
		glShaderSource(vertex_shader, 1, (const GLcharARB **)&verts, NULL);
		glCompileShader(vertex_shader);
		glAttachShader(program, vertex_shader);
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &goodv);
		print_info_log(vertex_shader);
		assert(goodv == GL_TRUE);
		
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* frags = f.data();
		glShaderSource(fragment_shader, 1, (const GLcharARB **)&frags, NULL);
		glCompileShader(fragment_shader);
		glAttachShader(program, fragment_shader);
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &goods);
		print_info_log(fragment_shader);
		assert(goods == GL_TRUE);

		glLinkProgram(program);
}
