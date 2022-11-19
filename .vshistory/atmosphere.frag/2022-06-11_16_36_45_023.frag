#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

varying vec2 texCoord;
uniform sampler2D texture;

void main()
{
    vec2 st = gl_FragCoord.st;
    vec4 sample =  texture2D(texture, st);
    gl_FragColor = vec4(sample[0], sample[1], sample[2], 1);
}