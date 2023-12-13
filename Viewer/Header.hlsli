cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix mat;
}

struct VSOutput
{
	float4 svpos:SV_POSITION;
};