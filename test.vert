#version 110

attribute  vec3 in_Position;
varying    vec4 color;
attribute  vec3 in_Normal;
varying    vec4 light;
attribute  vec2 in_Tex;
varying    vec2 texin;
mat4 transform;
void main(void)
{
    transform = mat4(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
	gl_Position = gl_ModelViewMatrix*gl_ProjectionMatrix*transform*vec4(in_Position, 1.0);
	color = vec4(in_Normal, 1.0);
	light = vec4(0.1, 0.1, -0.8, 1.0);
    texin = in_Tex;
}
