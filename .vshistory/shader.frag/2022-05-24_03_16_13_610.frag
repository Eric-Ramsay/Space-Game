#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec3 u_mouse;
uniform float u_time;

void main() {
	vec2 st = gl_FragCoord.st / u_resolution;
	if (st.x % 50 == 0) {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}