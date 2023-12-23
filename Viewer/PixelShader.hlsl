
#include "Header.hlsli"

float4 BasicPS(VSOutput input) : SV_TARGET
{
	//�����̌���
	float3 light = normalize(float3(1,-1,1));
	//diffuse
	float diffuse = saturate(dot(-light, input.normal.xyz));
	//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	float brightness = diffuse+0.3f;//�^���Â������

	return float4(brightness, brightness, brightness, 1);

	//return color;
}
