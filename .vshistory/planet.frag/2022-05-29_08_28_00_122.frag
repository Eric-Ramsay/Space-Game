#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

uniform vec2 offset;

int randC(int x, int y) {
	int h = 727253535 + (x % 32000) * 374761393 + (y % 32000) * 668265263;
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}
void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (int)(st.x + offset.x);
	st.y = (int)(st.y - offset.y);

	gl_FragColor = vec4(st.x % 2, st.y % 2, 0.0, 1.0);
}