#version 450 core
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform mat4 mvp;

out vec3 normal;
out vec3 position;
out vec2 texcoord;

void main(){
	gl_Position = mvp * vec4(in_vertex, 1.0f);
	position = gl_Position.xyz;
	normal = normalize(mat3(mvp) * in_normal);
    position = in_vertex;
	texcoord = in_texcoord;
}