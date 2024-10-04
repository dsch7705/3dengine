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

out uvec3 fragColor;
//out vec4 fragColor;

void main()
{
	fragColor = uvec3(objId + 1, 255, 0);
	//fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}