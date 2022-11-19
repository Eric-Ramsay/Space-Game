#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;
uniform float size;
float PI = 3.14159265358979323846;

void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (st.x + offset.x);
	st.y = (st.y - offset.y);
	
	float band = abs(st.y/size);

	gl_FragColor = vec4(band, 0, 0, 1);
}