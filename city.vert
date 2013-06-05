#version 110

attribute vec3 position;
attribute vec3 normal;
//attribute vec3 in_Color;
attribute vec3 tex_coord;
varying   vec3 color;
varying   vec3 out_tex;
uniform   mat4 transform;
uniform   vec3 light_pos;

void main(void)
{
    vec4 transnorm = transform*vec4(normal, 1.0);
    vec4 transvert = transform*vec4(position, 1.0);
    vec4 l = normalize(vec4(light_pos, 1.0) - transvert);
    float dist = length(l);
    float diffuse = max(dot(transnorm, l), 0.0);
    diffuse = diffuse * (1.0 / (1.0 + (0.25 * dist * dist)));
    color = vec3(0.2, 0.3, 0.7) * diffuse;
    gl_Position = transform*vec4(position, 1.0);
    //light = vec3(transform*vec4(0.5, 0.5,     -0.3, 1.0));
    //color = dot(light, vec3(0.4, 0.3, 0.7));
}


