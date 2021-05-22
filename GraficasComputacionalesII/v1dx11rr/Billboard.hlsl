Texture2D colorMap : register(t0);

SamplerState colorSampler : register(s0);

cbuffer cbChangerEveryFrame : register(b0)
{
	matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
	matrix projMatrix;
};

cbuffer Faa : register(b3)
{
	float4 FAA;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 FAA_temp: TEXCOORD1;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;

	vsOut.FAA_temp = FAA.xyz;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	//Elemento ambiental
	float4 LuzAmbiental = float4(1, 1, 1, 1);
	float FAA = pix.FAA_temp.x; //Factor de antenuacion ambiental
	float4 AportAmbiental = LuzAmbiental * FAA;

	float4 text = colorMap.Sample(colorSampler, pix.tex0);

	if (text.a < 0.8)
	{
		clip(-1);
		return text;
	}
	else
		return text * AportAmbiental;

}