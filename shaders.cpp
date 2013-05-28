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