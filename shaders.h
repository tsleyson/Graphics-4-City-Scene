#include <string>

std::string read_shader(const char * filename);
void print_info_log(unsigned int obj);
void init_shaders(std::string vert, std::string frag, unsigned int& program);
