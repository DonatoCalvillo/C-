Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specMap : register(t2);
Texture2D displaceMap : register(t3);

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

cbuffer gener : register(b3)
{
	float4 generales;
};

cbuffer Faa : register(b4)
{
	float4 FAA;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;	
	float3 normal : NORMAL0;
	float3 tangente : TANGENT0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 tangente : TEXCOORD2;
	float3 binormal : TEXCOORD3;
	float3 generais : TEXCOORD4;
	float3 posicion : TEXCOORD5;
	float3 FAA_temp: TEXCOORD6;
};

SamplerState demo
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	float lee = displaceMap.SampleLevel(colorSampler, vertex.tex0, 0).r;
	vsOut.pos = vertex.pos + float4(vertex.normal * lee * 0.5,0);
	
	vsOut.pos = mul(vsOut.pos, worldMatrix);
	vsOut.posicion = vsOut.pos;
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;	

	vsOut.tangente = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangente = normalize(vsOut.tangente - vsOut.normal * dot(vsOut.normal, vsOut.tangente));
	vsOut.binormal = normalize(cross(vsOut.normal, vsOut.tangente));
	
	vsOut.generais = generales;

	vsOut.FAA_temp = FAA.xyz;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 textColor = colorMap.Sample(colorSampler, pix.tex0);

	//Elemento ambiental
	float4 LuzAmbiental = float4(1, 1, 1, 1);
	float FAA = pix.FAA_temp.x; //Factor de antenuacion ambiental
	float4 AportAmbiental = LuzAmbiental * FAA;

	///////////////////////////////////////
	// elemento difuso
	///////////////////////////////////////
	float4 LuzDifusa = float4(1, 1, 1, 1);
	float3 DirLuz = normalize(float3(0, 10, 0));
	float FAD = .5;	
	float4 bump = normalMap.Sample(colorSampler, pix.tex0);
	float3 bumpNormal = 2 * bump - 1;
	bumpNormal.x *= -1;
	float3x3 TBN = float3x3(pix.tangente, pix.binormal, pix.normal);
	
	float3 bumpTBN = normalize(mul(bumpNormal, TBN));
	
	float FALLMN = dot(bumpTBN, DirLuz);
	float4 AportDif = LuzDifusa * FAD  * FALLMN;

	//////////////////////////////////////////////
	//elemento especular
	/////////////////////////////////////////////
	float Shininess = 40;
	float4 LuzEspecular = float4(1, 1, 1, 1);
	float  FAS = 3.5;
	float3 PosView = float3(0,30,220);
	float3 ViewDir = normalize(PosView - pix.posicion.xyz);
	
	float3 Reflect = normalize(2 * bumpTBN - DirLuz);
	float specular = pow(saturate(dot(Reflect, ViewDir)), Shininess);
	
	float4 specPix = specMap.Sample(colorSampler, pix.tex0);

	float4 AportEspecular = LuzEspecular * specPix * specular * FAS;


	return textColor * (AportAmbiental + AportDif + AportEspecular);
}