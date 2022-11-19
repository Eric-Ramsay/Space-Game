#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

varying vec2 texCoord;
uniform sampler2D texture;

void main()
{
    vec4 sample =  texture2D(texture, texCoord);
    float brightness = ((gl_Color[0] + gl_Color[1] + gl_Color[2]) * gl_Color[3])/3.0;
    gl_FragColor = vec4(gl_Color[0], gl_Color[1], gl_Color[2], 1);
}