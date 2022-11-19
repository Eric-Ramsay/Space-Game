#ifdef GL_ES
precision mediump float;
#endif

void main() {
	vec2 st = gl_FragCoord.st;
	gl_FragColor = vec4(0, 0, 0, 1);
}