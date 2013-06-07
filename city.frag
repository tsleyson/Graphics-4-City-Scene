#version 110

varying vec3 out_tex;
varying vec3 color;
varying vec4 light;
uniform sampler2D samp;

void main(void)
{
    float d = abs(dot(vec4(color, 1.0),light));
    gl_FragColor = vec4(0.0,d,d,1.0);
    //gl_FragColor = vec4(color, 1.0);
}
