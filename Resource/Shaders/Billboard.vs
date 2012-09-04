attribute vec3 a_position;
attribute vec2 a_uv;
varying vec2 v_uv;
uniform mat4 u_mvpMatrix;
void main()
{
vec4 posL = vec4(a_position, 1.0);
gl_Position = u_mvpMatrix * posL;
v_uv = a_uv;
}
   