#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

uniform vec3 viewPos;
uniform vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment
in vec3 normalOutput;
in vec3 posOutput;
out vec4 fragColor;

void main()
{
	vec3 lightColor = vec3(0.2, 0.2, 0.5);
	vec3 lightPos = vec3(0.0, 10.0, 10.0);
	//vec3 color = vec3(0.0, 0.0, 1.0);

	float ambientStrength = 0.1;
    	vec3 ambient = ambientStrength * lightColor;	

	vec3 norm = normalize(normalOutput);
	vec3 lightDir = normalize(lightPos - posOutput);
	vec3 attenuatedLight = lightColor / distance(lightPos,posOutput) * 10;
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - posOutput);	
	vec3 reflectDir = reflect(-lightDir, normalOutput);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
	vec3 specular = specularStrength * spec * lightColor; 
	
	vec3 result = (ambient + diff + specular) * color;
	fragColor = vec4(result, 1.0);
}
