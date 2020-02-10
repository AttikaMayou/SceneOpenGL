attribute vec3 a_position;
attribute vec3 a_color;
attribute vec3 a_normal;
attribute vec2 a_text_coord;

attribute vec3 a_diffuse_position;
attribute vec3 a_diffuse_color;

uniform float u_ambient_intensity;

uniform float u_time;
uniform mat4 u_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

varying vec3 v_color;
varying vec3 v_normal;
varying vec2 v_text_coord;

varying float v_ambient_intensity;
varying vec3 v_diffuse_light_position;
varying vec3 v_diffuse_light_color;
varying vec3 v_frag_pos;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

mat4 translationMatrix(vec3 axis, float value) 
{
    axis = normalize(axis);

	return mat4(1.0, 0.0, 0.0, axis.x * value,
				0.0, 1.0, 0.0, axis.y * value,
				0.0, 0.0, 1.0, axis.z * value,
				0.0, 0.0, 0.0, 1.0);
}

void main(void) {
	vec3 axisTrans = vec3(0.0, 0.0, 1.0);
	vec3 axisRot = vec3(1.0, 0.0, 0.0);
	float angleByTime = (int)(u_time * 100) % (360 * 100);

	mat4 matrixRot = rotationMatrix(axisRot, angleByTime/100);
	mat4 newMatrix = u_matrix * matrixRot;

	gl_Position = u_projection_matrix * u_view_matrix * newMatrix * vec4(a_position, 1.0);
	v_color = a_color;

	v_normal = mat3(transpose(inverse(u_matrix))) * a_normal;
	v_ambient_intensity = u_ambient_intensity;
	v_text_coord = a_text_coord;

	v_diffuse_light_color = vec3(1.0, 0.0, 0.0);
	// Nous sommes sincèrement désolé mais on avait pas vu
	v_diffuse_light_position = vec3(0.0, 0.0, -cos(u_time));

	v_frag_pos = vec3(u_matrix * vec4(a_position, 1.0));
}