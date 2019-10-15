#version 330 core

out vec4 FragColor;
uniform ivec2 vp;

float MAX = sqrt(float(vp[0]*vp[0] + vp[1]*vp[1]));

void main() {
	float posx = gl_FragCoord.x;
	float posy = gl_FragCoord.y;
	float dist1 = sqrt(posx*posx + posy*posy);
	float dist2 = sqrt((posx-vp[0])*(posx-vp[0]) + posy*posy);
	float dist3 = sqrt((posx-vp[0]/2)*(posx-vp[0]/2) + (posy-vp[1])*(posy-vp[1]));
    FragColor = vec4(1-(dist1/MAX), 1-(dist2/MAX), 1-(dist3/MAX), 1);
}

