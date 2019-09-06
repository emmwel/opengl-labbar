#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;
uniform float u_time;

out float shade;
// out vec3 shade;
//out vec2 texcoord;

void main(void)
{
	// shade = (mat3(modelToWorldToView)*in_Normal).z; // Fake shading
	// shade = (mat3(modelToWorldToView)*in_Normal); // Gouraud shading
	//texcoord = in_TexCoord;

	// move like snake
	vec3 pos = in_Position;
	// pos.x = pos.x + sin(10 * pos.y + 0.001 * u_time);

	vec3 light = vec3(1.0, 1.0, 1.0);
	light = normalize(light);
	vec3 normal = normalize(in_Normal);
	float kd = 0.5;
	float is = 0.9;
	shade = kd * is * (dot(light, normal));

	gl_Position=projectionMatrix*modelToWorldToView*vec4(pos, 1.0);
}
