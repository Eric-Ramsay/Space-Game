#ifdef GL_ES
precision mediump float;
#endif

void main() {
	vec2 st = gl_FragCoord.st;
	gl_FragColor = vec4((int)st.x % 2, st.y % 2, 0, 1);
}