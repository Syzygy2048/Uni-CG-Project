#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D blurredTexture;
uniform sampler2D depthBuffer;
uniform float focusDistance;

float focalLength = focusDistance / 2;

void main(){
	vec3 basePixel = texture(renderedTexture, UV).rgb;
	vec3 blurPixel = texture(blurredTexture, UV).rgb;
	
	
	float depth = texture(depthBuffer, UV).r - 1;


	float focus = abs(focusDistance - depth);
	clamp(focus, 0, 1);
	//focus = focus / focalLength;
	color = mix(basePixel, blurPixel, focus);
	//color = vec3(focus);
}
