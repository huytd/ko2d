attribute vec3 a_posL;
varying float vAlpha;
uniform float uAlpha;
uniform vec3 uColor;
varying vec3 vColor;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = posL;
	vAlpha = uAlpha;
	vColor = uColor;
}
   