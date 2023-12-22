#include"Header.hlsli"

VSOutput main(float4 pos:POSITION,float3 normal:NORMAL)
{
	VSOutput output;
	output.svpos = mul(mat,pos);
	output.normal = normal;
	return output;
}