#version 150

in vec3 ObjectPos;
in vec3 Normal;
in vec3 CameraPos;

out vec4 out_Color;

in vec2 frag_texcoord;

uniform sampler2D exampletexture;

void main(void)
{
	// Calculate light and normalize vectors
	vec3 lightPos = vec3(1.0, 1.0, 1.0);
	vec3 lightDir = vec3(normalize(lightPos - ObjectPos));
	vec3 normal = normalize(Normal);

	// camera
	vec3 cameraPos = vec3(0, 0, 1.5);
	vec3 cameraDir = vec3(normalize(cameraPos - ObjectPos));

	// reflection vector
	vec3 reflection = 2 * normal * (dot(lightDir, normal)) - lightDir;
	reflection = normalize(reflection);

	// Variables for surface
	//vec3 kd = vec3(1.0, 1.0, 1.0);
	vec4 kd = texture(exampletexture,frag_texcoord);
	float is = 0.9;
	float ia = 0.1;
	vec4 kspec = vec4(1.0, 1.0, 1.0, 1.0);
	float alpha = 50.0;

	vec4 shade = (kd * ia) + (kd * is * max(dot(lightDir, normal), 0.0)) + (kspec * is * pow(max(0, dot(reflection, cameraDir)), alpha));

	//out_Color=vec4(shade, 1.0);
	out_Color = shade;
	//out_Color=texture(exampletexture,frag_texcoord) * vec4(shade, 1.0);
}
