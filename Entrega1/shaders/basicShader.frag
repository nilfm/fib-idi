#version 330 core

in vec3 fcolor;

uniform int ratlles;

out vec4 FragColor;

void main() {
	if (ratlles != 0 && int(gl_FragCoord.y)%4 != 0) discard;
    FragColor = vec4(fcolor, 1);
}

