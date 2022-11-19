#ifdef GL_ES 
#define LOWP lowp
#define MED mediump
#define HIGH highp
precision highp float;
#else
#define MED
#define LOWP
#define HIGH
#endif
 
uniform float u_time;
uniform vec3 u_planetColor0;
uniform vec3 u_planetColor1;
uniform vec3 u_planetColor2;
 
uniform float u_random0;
uniform float u_random1;
uniform float u_random2;
uniform float u_random3;
uniform float u_random4;
uniform float u_random5;
uniform float u_random6;
uniform float u_random7;
uniform float u_random8;
uniform float u_random9;
 
varying vec2 v_texCoords0;
 
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+10.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
 
float pnoise2(vec2 P, float period) {
    return pnoise(P*period, vec2(period, period));
}
 
float pnoise1(float x, float period) {
    return pnoise2(vec2(x, 0.0), period);
}
 
vec3 toColor(float value) {
    float r = clamp(-value, 0.0, 1.0);
    float g = clamp(value, 0.0, 1.0);
    float b = 0.0;
    return vec3(r, g, b);
}
 
float planetNoise(vec2 P) {
    vec2 rv1 = vec2(u_random0, u_random1);
    vec2 rv2 = vec2(u_random2, u_random3);
    vec2 rv3 = vec2(u_random4, u_random5);
    vec2 rv4 = vec2(u_random6, u_random7);
    vec2 rv5 = vec2(u_random8, u_random9);
 
    float r1 = u_random0 + u_random2;
    float r2 = u_random1 + u_random2;
    float r3 = u_random2 + u_random2;
    float r4 = u_random3 + u_random2;
    float r5 = u_random4 + u_random2;
 
    float noise = 0.0; 
    noise += pnoise2(P+rv1, 10.0) * (0.2 + r1 * 0.4);
    noise += pnoise2(P+rv2, 50.0) * (0.2 + r2 * 0.4);
    noise += pnoise2(P+rv3, 100.0) * (0.3 + r3 * 0.2);
    noise += pnoise2(P+rv4, 200.0) * (0.05 + r4 * 0.1);
    noise += pnoise2(P+rv5, 500.0) * (0.02 + r4 * 0.15);
 
    return noise;
}
 
float jupiterNoise(vec2 texCoords) {
    float r1 = u_random0;
    float r2 = u_random1;
    float r3 = u_random2;
    float r4 = u_random3;
    float r5 = u_random4;
    float r6 = u_random5;
    float r7 = u_random6;
    float distEquator = abs(texCoords.t - 0.5) * 2.0;
    float noise = planetNoise(vec2(texCoords.x+distEquator*0.6, texCoords.y));
 
    float distPol = 1.0 - distEquator;
    float disturbance = 0.0;
    disturbance += pnoise1(distPol+r1, 3.0+r4*3.0) * 1.0;
    disturbance += pnoise1(distPol+r2, 9.0+r5*5.0) * 0.5;
    disturbance += pnoise1(distPol+r3, 20.0+r6*10.0) * 0.1;
    disturbance = disturbance*disturbance*2.0;
    float noiseFactor = r7 * 0.3;
    float noiseDistEquator = distEquator + noise * noiseFactor * disturbance;
    return noiseDistEquator;
}
 
float jupiterHeight(float noise) {
    return noise * 5.0;
}
 
vec3 planetColor(float distEquator) {
    float r1 = u_random0 + u_random3;
    float r2 = u_random1 + u_random3;
    float r3 = u_random2 + u_random3;
    float r4 = u_random3 + u_random3;
    float r5 = u_random4 + u_random3;
    float r6 = u_random5 + u_random3;
 
    float r7 = u_random6 + u_random3;
    float r8 = u_random7 + u_random3;
 
    vec3 color1 = u_planetColor0; 
    vec3 color2 = u_planetColor1; 
    vec3 color3 = u_planetColor2; 
 
    float v1 = pnoise1(distEquator+r1, 2.0 + r4*15.0) * r7;
    float v2 = pnoise1(distEquator+r2, 2.0 + r5*15.0) * r8;
 
    vec3 mix1 = mix(color1, color2, v1);
    vec3 mix2 = mix(mix1, color3, v2);
    return mix2;
}
 
void main() {
    float noise = jupiterNoise(v_texCoords0);
    vec3 color = planetColor(noise);
 
    gl_FragColor.rgb = color;
}