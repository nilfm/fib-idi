#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec3 vertSCO;
out vec3 NM;
out vec3 matambF;
out vec3 matspecF;
out float matshinF;
out vec3 matdiffF;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform mat3 normalMatrix;

void main()
{	
	vertSCO = (view*TG*vec4(vertex, 1.0)).xyz;
	NM = normalMatrix*normal;
	NM = normalize(NM);
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    matambF = matamb;
    matspecF = matspec;
    matshinF = matshin;
    matdiffF = matdiff;
}
