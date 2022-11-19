#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

void main() {
	gl_FragColor = vec4(1, 1, 1, 1);
}