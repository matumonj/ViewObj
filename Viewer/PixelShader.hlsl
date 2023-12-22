
#include "Header.hlsli"

float4 BasicPS(VSOutput input) : SV_TARGET
{
	//光源の向き
	float3 light = normalize(float3(1,-1,1));
	//diffuse
	float diffuse = saturate(dot(-light, input.normal));
	//光源へのベクトルと法線ベクトルの内積
	float brightness = diffuse+0.3f;//真っ暗を避ける

	return float4(brightness, brightness, brightness, 1);
	return float4(input.normal,1);
	//return color;
}
