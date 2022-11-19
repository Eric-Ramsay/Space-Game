#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel;
}