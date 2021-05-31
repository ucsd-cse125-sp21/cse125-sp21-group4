#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

uniform sampler2D tex;
uniform vec3 viewPos;        //just the eye pose
uniform vec3 diffuseFactor; //kd
uniform vec3 ambientColor; //ka
uniform vec3 specColor;  //ks
uniform float specHighlight; //ns, the larger this value is, the more apparent the light dot on the surface
uniform float dissolve; //d

// You can output many things. The first vec4 type output determines the color of the fragment
in vec3 normalOutput;
in vec3 posOutput;
in vec2 texCoord;
out vec4 fragColor;

void main()
{
	vec4 texColor = texture(tex, texCoord);  //get the base object color from texture
	vec3 lightPos = vec3(viewPos.x, 0.0, viewPos.z - 6);	
	vec3 lightDir = normalize(lightPos - posOutput);
	vec3 norm = normalize(normalOutput);
	
	//diffuse lighting
	float diff = max(dot(norm, lightDir), 0.0); // this calculates diffuse intensity based on angle
	vec3 diffuseColor = vec3( diffuseFactor[0],diffuseFactor[1],  diffuseFactor[2]); //this calculates diffuse
	vec3 diffuse = diff * diffuseColor;   //color based on texture color value and input diffuse factor kd

	float specularStrength = 0.1;           //4 might be too high
	vec3 viewDir = normalize(viewPos - posOutput);	
	vec3 reflectDir = reflect(-lightDir, normalOutput);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specHighlight);
	vec3 specular = specularStrength * spec * specColor; 
	
	vec3 result = ambientColor;
	fragColor = vec4(result, dissolve);
}
