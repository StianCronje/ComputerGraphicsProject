#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

//Normals 
in vec3 Normal;  

//recieve the fragement positions
in vec3 FragPos; 

// Ouput data
out vec3 color;



// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

//Ambient
uniform vec3 ambientColor;

//Diffuse color position
uniform vec3 lightPos; 
uniform vec3 lightPos1; 
// uniform vec3 lightPos2; 
//Diffuse color 
uniform vec3 lightColor;
uniform vec3 lightColor1;
// uniform vec3 lightColor2;

//Specular staff
uniform vec3 specColor;
uniform vec3 speclightPos;
uniform vec3 vertexPosition_cameraspace;


void main()
{


// Ambient Light strength is really weak (Should be)
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * ambientColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir1 = normalize(lightPos1 - FragPos);
    // vec3 lightDir2 = normalize(lightPos2 - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    // float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 diffuse = (diff * lightColor) + (diff1 * lightColor1);// + (diff2 * lightColor2);

	// Specular
	float specularStrength = 0.4f;
    vec3 viewDir = normalize(vertexPosition_cameraspace - FragPos);
	vec3 speclightDir = normalize(speclightPos - FragPos);
    vec3 reflectDir = reflect(-speclightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * specColor;  

	// The final color calculation 
    vec3 result = texture(myTextureSampler, UV).rgb * (ambient + diffuse + specular);
	color = result;
}