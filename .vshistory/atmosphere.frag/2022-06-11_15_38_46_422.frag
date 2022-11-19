#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

varying vec2 texCoord;
uniform sampler2D texture;

void main()
{
    vec4 sample = texture2D(texture, texCoord);
    float grey = 0.21 * sample.r + 0.71 * sample.g + 0.07 * sample.b;
    gl_FragColor = vec4(brightness, brightness, brightness, 1);
}