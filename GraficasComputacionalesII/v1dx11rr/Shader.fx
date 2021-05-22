Texture2D colorMap : register(t0);			
Texture2D colorMap2 : register(t1);		
Texture2D colorMap5 : register(t2);
Texture2D colorMap6 : register(t3);
Texture2D blendMap : register(t4);			
Texture2D colorMap3 : register(t5);			
Texture2D colorMap4 : register(t6);			
Texture2D rgbMap : register(t7);

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
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangente : NORMAL1;
	float3 binormal : NORMAL2;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangent : NORMAL1;
	float3 binorm : NORMAL2;
	float3 FAA_temp: TEXCOORD2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	vsOut.blendTex = vertex.blendTex;
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangent = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.binorm = normalize(mul(vertex.binormal, worldMatrix));

	vsOut.FAA_temp = FAA.xyz;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 fColor = float4(1,0,0,1);

	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	//Elemento ambiental
	float4 LuzAmbiental = float4(1, 1, 1, 1);
	float FAA = pix.FAA_temp.x; //Factor de antenuacion ambiental
	float4 AportAmbiental = LuzAmbiental * FAA;

	//Texturas 
	float4 text = colorMap.Sample(colorSampler, pix.tex0);		//dirt
	float4 text2 = colorMap2.Sample(colorSampler, pix.tex0);	//dirt nm
	float4 text3 = colorMap3.Sample(colorSampler, pix.tex0);	//rocks

	//Texturas normales
	float4 text4 = colorMap4.Sample(colorSampler, pix.tex0);	//snow
	float4 text5 = colorMap5.Sample(colorSampler, pix.tex0);	//rocks nm
	float4 text6 = colorMap6.Sample(colorSampler, pix.tex0);	//snow nm

	float4 rgbMap2 = rgbMap.Sample(colorSampler, pix.blendTex);	//mapaRGB

	float4 textf = 0;
	textf = text * (rgbMap2.g) + textf * (1.0 - rgbMap2.g);		//dirt
	textf = text3 * (rgbMap2.r) + textf * (1.0 - rgbMap2.r);	//rocks
	textf = text4 * (rgbMap2.b) + textf * (1.0 - rgbMap2.b);	//snow

	float4 normalf = 0;
	normalf = text2 * (rgbMap2.g) + normalf * (1.0 - rgbMap2.g); //dirt nm
	normalf = text5 * (rgbMap2.r) + normalf * (1.0 - rgbMap2.r); //rocks nm
	normalf = text6 * (rgbMap2.b) + normalf * (1.0 - rgbMap2.b); //snow nm

	float3 bump = 2.0 * normalf - 1.0;
	float3x3 TBN = { {pix.tangent}, {pix.binorm}, {pix.normal} };
	float3 newnormal = mul(TBN, bump);

	float3 DiffuseDirection = float3(0.0f, -1.0f, 2.5f);
	float4 DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3 diffuse = dot(-DiffuseDirection, newnormal);
	diffuse = saturate(diffuse * DiffuseColor.rgb);
	diffuse = saturate(diffuse + ambient);

	fColor = float4(textf.rgb * diffuse, 1.0f);

	return fColor * AportAmbiental;
}