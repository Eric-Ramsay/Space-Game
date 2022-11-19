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
	int key = 0;
	int layer = 0;
	vec2 st = gl_FragCoord.st;
	do {
		st.x = int(st.x + offset.x);
		st.y = int(st.y - offset.y);
		key = randC(int(st.x), int(st.y)) % 10000;
	} while (layer++ < 8 && key != 1);
	if (key == 1) {
		gl_FragColor = vec4(.5 + ((int)st.x % 2) * .5, .5 + ((int)st.y % 2) * .5, .9, 1.0);
	}
	else {
		gl_FragColor = vec4(0, 0, 0, 1);
	}
}