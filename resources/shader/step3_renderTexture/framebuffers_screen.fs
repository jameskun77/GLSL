#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offsetX = 1.0 /1280.0;
const float offsetY = 1.0 /720.0;

void main()
{
    //vec3 col = texture(screenTexture, TexCoords).rgb;
    //FragColor = vec4(col, 1.0);
	
	//FragColor = vec4(vec3(1.0 - texture(screenTexture,TexCoords)),1.0);
	
	//FragColor = texture(screenTexture,TexCoords);
	//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g +  0.0722 * FragColor.b;
	//FragColor = vec4(average,average,average,1.0);
	
	//FragColor = texture(screenTexture,TexCoords);
	
	//FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	
	vec2 offsets[9] = vec2[](
		vec2(-offsetX,  offsetY), 
        vec2( 0.0f,    offsetY), 
        vec2( offsetX,  offsetY), 
        vec2(-offsetX,  0.0f),   
        vec2( 0.0f,    0.0f),   
        vec2( offsetX,  0.0f),  
        vec2(-offsetX, -offsetY), 
        vec2( 0.0f,   -offsetY), 
        vec2( offsetX, -offsetY)  
		);
		
	float kernel[9] = float[](
		-1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
	);
	
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
} 