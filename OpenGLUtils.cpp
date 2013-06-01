#include "OpenGLUtils.h"

void init_shaders(string vert, string frag)
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
