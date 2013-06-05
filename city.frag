#version 110

varying vec3 out_tex;
varying vec3 color;
uniform sampler2D samp;

void main(void)
{
    gl_FragColor = vec4(color, 1.0);
}
