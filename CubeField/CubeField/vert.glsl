#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

layout(location = 2) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

//Forward the normals to the fragment shader
out vec3 Normal;

//Forward the Fragment positions in the world space
out vec3 FragPos;

// Values that stay constant for the whole mesh.
uniform mat4 MVP,trans;

void main()
{
	gl_Position =   MVP *trans* vec4(vertexPosition_modelspace,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
	//Normal array to be forwarded to the fragment shader
	Normal = vertexNormal_modelspace;

	//Fragment positions  to be forwarded to the fragment shader
	FragPos =vertexPosition_modelspace;
}