#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

uniform sampler2D texture;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float brightness = (gl_Color[0] + gl_Color[1] + gl_Color[2])
    gl_FragColor = vec4(gl_Color[0] ;
}