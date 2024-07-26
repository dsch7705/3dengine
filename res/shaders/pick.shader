#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

uniform int objId;

out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}