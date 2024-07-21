#shader vertex
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}

#shader fragment
#version 460 core

out vec4 fragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float weight;

void main()
{
	//fragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	fragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), weight) + vec4(ourColor, 1.0);
}