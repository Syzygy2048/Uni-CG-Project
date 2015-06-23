#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D blurredTexture;
uniform sampler2D depthBuffer;
uniform float focusDistance;

uniform mat4 projectionMatrixInverse;

float focalLength = focusDistance / 2;

void main(){
	vec3 basePixel = texture(renderedTexture, UV).rgb;
	vec3 blurPixel = 1-texture(blurredTexture, UV).rgb;
	
	vec2 mappedUV = UV * 2 - 1;
	
	float depth = texture(depthBuffer, UV).r;

	vec4 screenCoordinate = vec4(mappedUV, 1-depth, 1);

	vec4 unprojectedCoordinate = screenCoordinate * projectionMatrixInverse;
	
	
	float focus = abs(focusDistance - depth);
	clamp(focus, 0, 1);
	//focus = focus / focalLength;
	//color = mix(basePixel, blurPixel, focus);

	float scaleTest = length((unprojectedCoordinate.xyz/unprojectedCoordinate.w).xyz);
	scaleTest = (scaleTest - 0.1) / (5.0-0.1);
	color = vec3(scaleTest);
}
