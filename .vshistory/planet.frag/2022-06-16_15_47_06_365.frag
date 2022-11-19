#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;
uniform float height;
float PI = 3.14159265358979323846;

void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (st.x);
	st.y = (st.y);
	
	int band = 10 * (st.y/height);

	gl_FragColor = vec4(.5 + band * .05, 1 - band * .1, 0, 1);
}