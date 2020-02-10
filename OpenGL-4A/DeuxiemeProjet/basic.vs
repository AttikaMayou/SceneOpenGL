attribute vec3 a_position;
attribute vec3 a_color;
uniform float u_time;
uniform mat4 u_matrix;

varying vec4 v_color;

void main(void) {
	gl_Position = u_matrix * vec4(a_position, 1.0);
	v_color = vec4(a_color, 1.0);
}