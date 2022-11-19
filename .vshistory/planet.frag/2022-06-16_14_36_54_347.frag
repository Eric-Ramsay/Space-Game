#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 offset;
float PI = 3.14159265358979323846;

float gasGiant(vec2 position, float time) {
	float x = position.x;
	float y = position.y;
	float theta = atan(x, y);
	float r = sqrt(x*x + y*y);
	float bands = 10.0;
	float width = 1.0 / bands;
	float alpha = 1.0;

	for (float i = 0.0; i < bands; i++)	{
		float bandThickness = mix(0.2, 1.0, i / (bands - 1.0));
		float bandPosition = sin(theta * bands + time * (i + 1.0)) * width * 0.5 - i*width;
		float bandAlpha = 1.0 - abs(bandPosition);
		alpha *= bandAlpha;
	}

	return alpha;
}


void main() {
	vec2 st = gl_FragCoord.st;
	st.x = (st.x + offset.x);
	st.y = (st.y - offset.y);
	float r = 0.5 + 0.1 * sin(8.0 * PI * st.x);

	float g = 0.5 + 0.1 * sin(8.0 * PI * st.y);

	float b = 0.5 + 0.1 * sin(8.0 * PI * (st.x + st.y));

	gl_FragColor = vec4(r, g, 0, 1);
}