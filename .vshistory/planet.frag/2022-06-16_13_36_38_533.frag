#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;

void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (st.x + offset.x);
	st.y = (st.y - offset.y);
	float r = 0.5 + 0.1 * sin(8.0 * PI * u);

	float g = 0.5 + 0.1 * sin(8.0 * PI * v);

	float b = 0.5 + 0.1 * sin(8.0 * PI * (u + v));

	gl_FragColor = vec4(r, g, b, 1);
}