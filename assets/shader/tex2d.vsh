attribute vec4 a_color;
varying vec4 v_color;

attribute vec4 a_position;
attribute vec2 a_coord;
varying vec2 v_coord;
void main() {
  v_color = a_color;
  gl_Position = vec4(a_position.xy,0,1);
  v_coord = a_coord;
}