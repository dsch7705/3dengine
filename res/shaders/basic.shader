#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

out vec4 fragColor;

void main()
{
	fragColor = vec4(176.0 / 255.0, 224.0 / 255.0, 230.0 / 255.0, 1.0);
}