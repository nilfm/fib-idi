#version 330 core

out vec4 FragColor;
uniform ivec2 vp;

void main() {
	if (gl_FragCoord.x < vp[0]/2) {
		if (gl_FragCoord.y < vp[1]/2) {
			FragColor = vec4(1, 1, 0, 1);
		}
		else {
			FragColor = vec4(1, 0, 0, 1);
		}
	}
	else {
		if (gl_FragCoord.y < vp[1]/2) {
			FragColor = vec4(0, 1, 0, 1);
		}
		else {
			FragColor = vec4(0, 0, 1, 1);
		}
	}
}

