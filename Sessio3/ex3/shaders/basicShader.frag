#version 330 core

out vec4 FragColor;

const float MAX = 580.0;

void main() {
	float posx = gl_FragCoord.x;
	float posy = gl_FragCoord.y;
	float dist1 = sqrt(posx*posx + posy*posy);
	float dist2 = sqrt((posx-708.)*(posx-708.) + posy*posy);
	float dist3 = sqrt((posx-354.)*(posx-354.) + (posy-708.)*(posy-708.));
    FragColor = vec4(1-(dist1/MAX)*(dist1/MAX), 1-(dist2/MAX)*(dist2/MAX), 1-(dist3/MAX)*(dist3/MAX), 1);
}

