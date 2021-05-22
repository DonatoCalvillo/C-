Texture2D textures;
SamplerState colorSampler : register(s0);

cbuffer MatrixBuffer:register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};

cbuffer time : register(b1)
{
	float4 Timer;
};

cbuffer PaddingBuffer:register(b2)
{
	float4 padding;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;

};

struct PS_Input
{
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float4 pad  : TEXCOORD1;
	float4 tiempoLuz  : TEXCOORD2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;

	vsOut.pad = valores;
	vsOut.tiempoLuz = Timer;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 textureColor;
	float4 textureColor2;
	float2 nuevo = pix.tex0;
	float4 finalColor;
	

	if (pix.tiempoLuz.x >= 5)
	{
		textureColor = textures.Sample(colorSampler, nuevo); 
		finalColor = textureColor;
	}
	else if (pix.tiempoLuz.x >= 4)
	{
		textureColor = textures.Sample(colorSampler, nuevo);
		textureColor2 = textures.Sample(colorSampler, nuevo + float2(0.66, 0)); 
		finalColor = textureColor * pix.pad.x + textureColor2 * (1 - pix.pad.x);
	}
	else if (pix.tiempoLuz.x >= 3)
	{
		textureColor = textures.Sample(colorSampler, nuevo + float2(0.66, 0)); 
		finalColor = textureColor;
	}
	else if (pix.tiempoLuz.x >= 2)
	{
		textureColor = textures.Sample(colorSampler, nuevo + float2(0.66, 0));
		textureColor2 = textures.Sample(colorSampler, nuevo + float2(0.33, 0)); 
		finalColor = textureColor * pix.pad.x + textureColor2 * (1 - pix.pad.x);
	}
	else if (pix.tiempoLuz.x >= 1)
	{
		textureColor = textures.Sample(colorSampler, nuevo + float2(0.33, 0)); 
		finalColor = textureColor;
	}
	else
	{
		textureColor = textures.Sample(colorSampler, nuevo + float2(0.33, 0));
		textureColor2 = textures.Sample(colorSampler, nuevo);                        
		finalColor = textureColor * pix.pad.x + textureColor2 * (1 - pix.pad.x);
	}

	return finalColor;
	
}