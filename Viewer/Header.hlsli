cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix mat;
	//float3 normal;
}

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float3 normal:NORMAL;
};