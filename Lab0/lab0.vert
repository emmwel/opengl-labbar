#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;

uniform mat4 projectionMatrix;
// uniform mat4 modelToWorldToView;
uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform float u_time;

out vec3 ObjectPos;
out vec3 Normal;
out vec2 frag_texcoord;

void main(void)
{

	gl_Position = projectionMatrix * worldToView *  modelToWorld * vec4(in_Position, 1.0);
	ObjectPos = vec3(worldToView *  modelToWorld * vec4(in_Position, 1.0));
	mat3 withoutTranslation = mat3(worldToView *  modelToWorld);
	Normal = withoutTranslation * in_Normal;
	frag_texcoord = in_TexCoord;
}
