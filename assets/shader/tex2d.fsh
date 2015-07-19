varying vec4 v_color;
precision mediump float;
varying vec2 v_coord;
uniform sampler2D s_texture;
void main() {
  gl_FragColor = v_color * texture2D(s_texture, v_coord);
}