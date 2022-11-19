#ifdef GL_ES
precision mediump float;
#endif
#extension GL_EXT_gpu_shader4 : require

uniform vec2 offset;
uniform vec2 position;
uniform vec2 resolution;

int randC(int x, int y) {
	int h = 727253535 + (x % 32000) * 374761393 + (y % 32000) * 668265263;
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}
void main() {
	vec2 st = gl_FragCoord.st;
	int x = int(st.x + offset.x) % 32000;
	int y = int(st.y - offset.y) % 32000;
	
	gl_FragColor = vec4(((float)x)/resolution.x, ((float)y/resolution.y, 0.0, 1.0);
}