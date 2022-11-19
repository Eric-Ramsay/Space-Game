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
	//gl_FragColor = vec4(0.0, st.y, 0.0, 1.0);
	int key = randC(st.x, st.y) % 1000;
	if (key == 1) {
		gl_FragColor = vec4(.5 + ((int)st.x % 2) * .5, .5 + ((int)st.y % 2) * .5, .9, 1.0);
	}
	else {
		int x = ((int)st.x % 32000);
		int y = ((int)st.y % 32000);
		gl_FragColor = vec4(x % 2, y % 2, 0.0, 1.0);
	}
}