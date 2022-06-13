// "RayMarching starting point"
// by Martijn Steinrucken aka The Art of Code/BigWings - 2020
// The MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// Email: countfrolic@gmail.com
// Twitter: @The_ArtOfCode
// YouTube: youtube.com/TheArtOfCodeIsCool
// Facebook: https://www.facebook.com/groups/theartofcode/
//
// You can use this shader as a template for ray marching shaders

#define MAX_STEPS 1000
#define MAX_DIST 800.
#define SURF_DIST .001

#define S(a, b, t) smoothstep(a, b, t)
#define T iTime

uniform vec2 M;             // mouse pos
uniform vec2 Position;

uniform float Zoom;        // size of gyroid
uniform float ScaleDetail;  // main gyroid detail amount
uniform float Thickness;
uniform float Bias;
uniform float sinBias;
uniform float cosBias;


uniform float bScale;        // size of gyroid
uniform float bScaleDetail;  // main gyroid detail amount
uniform float bThickness;
uniform float bBias;
uniform float bsinBias;
uniform float bcosBias;

//----------------------------------
mat2 Rot(float a) {
    float s=sin(a), c=cos(a);
    return mat2(c, -s, s, c);
}
//----------------------------------
float sdBox(vec3 p, vec3 s) {
    p = abs(p)-s;
	return length(max(p, 0.))+min(max(p.x, max(p.y, p.z)), 0.);
}
//----------------------------------
float sdGyroid(vec3 p, float scale, float thickness, float bias, float sinBias, float cosBias) {
  p *= scale; // scale the gyroid
  return abs(dot(sin(p * sinBias), cos(p.zxy * cosBias))-bias)/(scale*2)-thickness; // a gyroid is the dot product of 2 vectors
}
//----------------------------------
vec3 Transform(vec3 p) {
  p.xy *= Rot(p.z*.0015);
  p.z -= iTime*10;
  p.y -= Position.y;
  p.x -= Position.x;
  return vec3(p);
}
//----------------------------------
vec3 Background(vec3 rd) {
  vec3 col = vec3(0);          // clear the colour
  float y = rd.y*.5+.5;        // remap position
  col += (1-y)*vec3(.01, .1, .8); // add base colour to the background and apply a gradient

  float a = atan(rd.x, rd.z);
  float lightRays = sin(a*10+T*.1)*sin(a*7-T*.2)*sin(a*5);
  lightRays *= S(.8, .1, y); // fade the rays out at the pole
  col += lightRays; // add 10 sin wave stripes
  col = max(col, 0); //prevent going negative
  col += S(0.01, .0, y);

  return col;
}
//----------------------------------
float GetDist(vec3 p) {
    p = Transform(p); // move the world

    float box = sdBox(p, vec3(1));
    float g1 = sdGyroid(p, ScaleDetail, Thickness, Bias, sinBias, cosBias);
    float g2 = sdGyroid(p , bScaleDetail, bThickness, bBias * .3, bsinBias, bcosBias);
    float g3 = sdGyroid(p , bScaleDetail + 5, bThickness, bBias * .3, bsinBias, bcosBias);
    float g4 = sdGyroid(p , bScaleDetail * 15, bThickness, bBias * .3, bsinBias, bcosBias);
    float g5 = sdGyroid(p , bScaleDetail * 30, bThickness, bBias * .3, bsinBias, bcosBias);
    // float g6 = sdGyroid(p , bScaleDetail * 55, bThickness, bBias * .3, bsinBias, bcosBias);
    // float g7 = sdGyroid(p , bScaleDetail * 20.3, bThickness, bBias * .3, bsinBias, bcosBias);

    //float g = min(g1, g2); // create union between 2 gyroids (criss cross)
    //float g = max(g1, -g2); // subtract second gyroid from the first

    g1 -= g2 * .3; // use second gyroid as bump map
    g1 -= g3 * .1;
    g1 += g4 * .1;
    g1 += g5 * .1;
    // g1 += g6 * .2;
    // g1 += g7 * .1;

    //float d = max(box, g1 *.3); // to contain the gyroid use a boolean intersection
    float d = g1 * .3;

    return d;
}
//----------------------------------
float RayMarch(vec3 ro, vec3 rd) {
	float dO=0.;

    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO>MAX_DIST || abs(dS)<SURF_DIST) break;
    }

    return dO;
}

//----------------------------------
vec3 GetNormal(vec3 p) {
	float d = GetDist(p);
    vec2 e = vec2(.01, 0); // spften or sharpen edges

    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));

    return normalize(n);
}
//----------------------------------
vec3 GetRayDir(vec2 uv, vec3 p, vec3 l, float z) {
    vec3 f = normalize(l-p),
        r = normalize(cross(vec3(0,1,0), f)),
        u = cross(f,r),
        c = f*z,
        i = c + uv.x*r + uv.y*u,
        d = normalize(i);
    return d;
}
//----------------------------------

//----------------------------------
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord-.5*iResolution.xy)/iResolution.y;
  	vec2 m = M.xy/iResolution.xy;

    uv += sin(uv*20.+T)*.002; // heat distortion effect

    // camera code
    vec3 ro = vec3(0, 0, -.8)*Zoom;    // camera
    ro.yz *= Rot(-m.y*3.14+1.);  // y rotation
    ro.xz *= Rot(-m.x*6.2831);   // x rotation

    vec3 lookat = vec3(0,0,0);
    vec3 rd = GetRayDir(uv, ro, lookat, 1); // get Ray direction
    vec3 col = vec3(0);
    float d = RayMarch(ro, rd); // ray march

    if(d<MAX_DIST) {
        vec3 p = ro + rd * d;
        vec3 n = GetNormal(p);
        vec3 r = reflect(rd, n);

        float height = p.y;

        p = Transform(p);

        //float dif = dot(n, normalize(vec3(1,2,3)))*.5+.5;
        float dif = n.y *.5 + .5; // place the light directly above, up = 1 / down = -1
        // col = vec3(dif);
        col += dif * dif;  // use the normal for modeeling to check direction

        // add shading in the creases / ambient occlusion / blackening
        float g2 = sdGyroid(p , bScaleDetail + 5, bThickness, bBias, bsinBias, bcosBias);
        col *= S(-.1, .1, g2)*.4; // multiply output colour by the second gyroid to apply shading, use smoothstep to remap

        // add cracks
        float crackWidth = -.02+S(0., -.5, n.y)*.04; // make cracks larger on bottom face
        float cracks = S(crackWidth, -.03, g2);      // add cracks and set thickness
        float g3 = sdGyroid(p+T*.1, bScaleDetail, bThickness, bBias * 0., bsinBias, bcosBias);  // animate cracks
        float g4 = sdGyroid(p-T*.05, bScaleDetail *.5, bThickness, bBias * 0., bsinBias, bcosBias); // animate cracks
        cracks *= g3*g4*10.+.02*S(.2, .0, n.y);          // prevent cracks from going completely black
        col += cracks*vec3(.1, .04, 1.)*40;            // multiply cracks by a colour to add colour


        // add flickering lights
        // float g5 = sdGyroid(p-vec3(0,T*.05,0), ScaleDetail *.9, Thickness, Bias * 0., sinBias, cosBias); // animate cracks
        // col += g5*vec3(0.1,.1,4);
        // col += S(0, -2, height)*vec3(0.1,.1,4);
    }

    col = mix(col, Background(rd), S(0., 4000, d)); // apply the background depending on camrea distance

    col *= 1-dot(uv,uv); // vignette
    //-----------------------------------------------
    col = pow(col, vec3(.8545));	// gamma correction

    fragColor = vec4(col,1.0);    // output
}
