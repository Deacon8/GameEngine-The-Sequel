#version 450 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
}