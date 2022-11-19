#ifdef GL_ES
precision mediump float;
#endif

void main() {
	vec2 st = gl_FragCoord.st;
	gl_FragColor = vec4(st.x % 2, 0, 0, 1);
}