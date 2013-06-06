#version 110

varying vec4 color;
varying vec4 light;
varying vec2 texin;
uniform sampler2D texsampl;
uniform int texon;
void main(void)	
{
	//float d = abs(dot(color,light));
    //gl_FragColor = vec4(0.0,d,d,1.0);
	//gl_FragColor = vec4(0.1, 0.5, 0.2, 1.0);
    if (texon == 1)
        gl_FragColor = texture2D(texsampl, texin);
    else
        gl_FragColor = vec4(0.1, 0.5, 0.2, 1.0);
}
