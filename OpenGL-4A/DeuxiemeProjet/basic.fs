varying vec3 v_color;
varying vec3 v_normal;
varying vec2 v_text_coord;

varying float v_ambient_intensity;
varying vec3 v_diffuse_light_position;
varying vec3 v_diffuse_light_color;
varying vec3 v_frag_pos;

uniform sampler2D u_texture_sampler;

void main(void) {
	// With ambien intensity
	//gl_FragColor = v_color * v_ambient_intensity;

	vec4 tex_color = texture2D(u_texture_sampler, v_text_coord);

	vec3 ambient_color = vec3(1.0, 1.0, 1.0);
	vec3 ambient = ambient_color * v_ambient_intensity;

	vec3 newNormal = normalize(v_normal);
	vec3 lightDirection = normalize(v_diffuse_light_position - v_frag_pos);

	float diff = max(dot(newNormal, lightDirection), 0.0);
	vec3 diffuse = diff * v_diffuse_light_color;
	
	// With diffuseColor
	//gl_FragColor = vec4((ambient + diffuse) * v_color, 1.0);

	vec3 eyes = vec3(0.0, 0.0, 1.0);
	vec3 eyeDirection = normalize(eyes - v_frag_pos);
	vec3 reflexionDirection = reflect(-lightDirection, newNormal);
	
	vec3 specColor = vec3(1.0, 1.0, 1.0);
	float specular_intensity = 0.5;
	float spec = pow(max(dot(eyeDirection, reflexionDirection), 0.0), 32);
	vec3 specular = spec * specular_intensity * specColor;

	gl_FragColor = (vec4((ambient + diffuse + specular) * v_color, 1.0)) * tex_color;
}

vec3 diffuse(vec3 N, vec3 L) {
	vec3 direction = vec3(1.0, 1.0, 1.0);

	return direction;
}