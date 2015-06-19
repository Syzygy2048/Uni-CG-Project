#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform bool horizontal;

const vec2 gaussFilter[7] = vec2[](
	vec2(-3.0, 0.015625),
	vec2(-2.0, 0.09375),
	vec2(-1.0, 0.234375),
	vec2(0.0, 0.3125),
	vec2(1.0, 0.234375),
	vec2(2.0, 0.09375),
	vec2(3.0, 0.015625));


 
void main(){
	vec2 pixelSize = vec2(1.0/1280, 1.0/720);
	vec3 pixel = texture(renderedTexture, UV).xyz;
	color = pixel;
	//if(length(pixel) > 1)
	//{
	//	vec3 tempColor = vec3(0.0, 0.0, 0.0);
	//	for(int i = 0; i < 7; i++)
	//	{
	//		vec2 sampleUV;
	//		if(horizontal)
	//		{
	//			sampleUV = vec2(UV.x + gaussFilter[i].x * pixelSize.x, UV.y);
	//		}
	//		else
	//		{
	//			sampleUV = vec2(UV.x, UV.y + gaussFilter[i].x * pixelSize.y);
	//		}			 
	//		tempColor += texture2D(renderedTexture, sampleUV).xyz * gaussFilter[i].y * length(pixel);
	//	}
	//	color = tempColor;
	////}
	//else
	//{
	//	color = pixel;
	//}
}
