#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


out vec2 texCoord;
out vec3 normalOutput;
out vec3 posOutput;

void main()
{
    	gl_Position = projection * view * model * vec4(position, 1.0);
    	normalOutput = mat3(transpose(inverse(model))) * normal; 
    	posOutput = vec3(model * vec4(position, 1.0));
	texCoord = aTex;
}