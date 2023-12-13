cbuffer cbuff0:register(b0)
{
	float4 color;
}

float4 BasicPS() : SV_TARGET
{
	return color;
}