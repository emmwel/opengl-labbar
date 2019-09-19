#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;
uniform mat4 b1_rot;
uniform mat4 b1_neg1;
uniform mat4 b1_pos1;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning p� GPU
//
// Flytta �ver din implementation av soft skinning fr�n CPU-sidan
// till vertexshadern. Mer info finns p� hemsidan.

void main(void)
{
	// transformera resultatet med ModelView- och Projection-matriserna
	// vec4 res_bones = in_TexCoord.x * vec4(in_Position, 1.0) + in_TexCoord.y * b1_pos1 * b1_rot * b1_neg1 * vec4(in_Position, 1.0);
	// gl_Position = matrix * res_bones;
	gl_Position = matrix * vec4(in_Position, 1.0);

	// s�tt r�d+gr�n f�rgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// L�gg p� en enkel ljuss�ttning p� vertexarna
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}
