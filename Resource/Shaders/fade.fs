precision mediump float;
varying float vAlpha;
varying vec3 vColor;
void main()
{
	gl_FragColor = vec4(vColor, vAlpha);
}
