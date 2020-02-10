attribute vec3 a_position;
attribute vec3 a_color;
attribute vec3 a_normal;
attribute vec2 a_text_coord;

uniform float u_ambient_intensity;

uniform float u_time;
uniform mat4 u_matrix;

varying vec4 v_color;
varying vec3 v_normal;
varying vec2 v_text_coord;

varying float v_ambient_intensity;

uniform sampler2D u_sampler;

void main(void) {
	gl_Position = u_matrix * vec4(a_position, 1.0);
	vec4 color = texture2D(u_sampler, v_text_coord);
	v_color = vec4(color, 1.0);

	v_normal = a_normal;
	v_ambient_intensity = u_ambient_intensity;
	v_text_coord = a_text_coord;
}