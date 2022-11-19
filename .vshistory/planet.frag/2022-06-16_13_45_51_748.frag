#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;
float PI = 3.14159265358979323846;

void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (st.x + offset.x);
	st.y = (st.y - offset.y);
	float r = 0.5 + 0.1 * sin(8.0 * PI * st.x);

	float g = 0.5 + 0.1 * sin(8.0 * PI * st.y);

	float b = 0.5 + 0.1 * sin(8.0 * PI * (st.x + st.y));

	gl_FragColor = vec4(r, g, b, 1);
}