#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform float saturation;

void main()
{    
    FragColor = texture(tex, texCoord);
    FragColor.r = FragColor.r * (saturation);
    FragColor.g = FragColor.g * (saturation);
    FragColor.b = FragColor.b * (saturation);
    if (FragColor.a < 0.1)
    {
        //FragColor = vec4(1.0,0,0,1.0);
        discard;
    }
}