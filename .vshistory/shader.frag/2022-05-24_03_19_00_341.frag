#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;

int randC(int x, int y) {
	int h = 7272753535 + x * 374761393 + y * 668265263;
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}	

void main() {
	vec2 st = gl_FragCoord.st;
	if ((int)(st.x) % 50 == 0) {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	else {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}