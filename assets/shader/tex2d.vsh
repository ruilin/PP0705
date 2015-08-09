uniform mat4 u_mvp;
attribute vec4 a_color;
attribute vec4 a_position;
attribute vec2 a_coord;
varying vec4 v_color;
varying vec2 v_coord;

void main() {
  v_color = a_color;
  gl_Position = vec4(a_position.xy, 0, 1) * u_mvp;
  v_coord = a_coord;
}