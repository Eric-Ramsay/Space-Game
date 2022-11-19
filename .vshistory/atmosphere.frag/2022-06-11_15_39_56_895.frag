#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

varying vec2 texCoord;
uniform sampler2D texture;

void main()
{
    vec4 sample = texture2D(texture, texCoord);
    float grey = 0.33 * sample[0] + 0.33 * sample[1] + 0.33 * sample[2];
    gl_FragColor = vec4(grey, grey, grey, 1.0);
}