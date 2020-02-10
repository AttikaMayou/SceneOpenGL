varying vec4 v_color;
varying vec3 v_normal;
varying vec2 v_text_coord;

varying float v_ambient_intensity;

void main(void) {
	gl_FragColor = v_color	* v_ambient_intensity;

	v_normal = normalize(v_normal);
}

vec3 diffuse(vec3 N, vec3 L) {
	vec3 direction = vec3(1.0, 1.0, 1.0);

	return direction;
}