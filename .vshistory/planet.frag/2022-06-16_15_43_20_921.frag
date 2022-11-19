#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;
float PI = 3.14159265358979323846;

void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (st.x + offset.x);
	st.y = (st.y - offset.y);
	
	int num = 10;

	for (int band = 0; band < num; band++) {
		gl_FragColor = vec4(r, g, 0, 1);
	}
}