#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;

void main() {
	function gasGiantBands(u, v) {

		var r = 0.5 + 0.1 * Math.sin(8.0 * Math.PI * u);

		var g = 0.5 + 0.1 * Math.sin(8.0 * Math.PI * v);

		var b = 0.5 + 0.1 * Math.sin(8.0 * Math.PI * (u + v));

		return [r, g, b, 1.0];

	}
}