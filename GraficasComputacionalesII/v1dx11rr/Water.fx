#pragma once
Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);


SamplerState colorSampler : register(s0);

cbuffer parametros : register(b0)
{
	matrix m_World;
	matrix m_View;
	matrix m_Perspective;

	float4 v_campos;

	float4 v_lightPos;

	float3 c_light;
	float f_shininess;

	float f_height_value;
	float f_map_displace;
	float f_displacement_factor;
}

Texture2D    t_diffuse     : register(t0);
Texture2D    t_normal      : register(t1);
Texture2D    t_specular    : register(t2);
Texture2D    t_height      : register(t3);
Texture2D    t_displace    : register(t4);

SamplerState s_sampler     : register(s0);

struct VS_In
{
	float4 pos      : POSITION;
	float3 normal   : NORMAL;
	float2 texcoord : TEXCOORD;
};

struct PS_In
{
	float4 pos           : SV_POSITION;
	float4 worldPos      : POSITION0;
	float3 normal        : NORMAL;
	float2 texcoord      : TEXCOORD0;
	float2 texcoord2     : TEXCOORD1;

	float4 v_campos      : POSITION1;
	float4 v_lightPos    : POSITION2;
	float f_displacement_factor : VALUE;
};
/*
float nrand(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}     */

PS_In vert(VS_In IN)
{
	PS_In OUT;


	float t = 2.5f * (f_map_displace % 31415.f);

	matrix m_MVP = mul(mul(m_World, m_View), m_Perspective);

	float f_new_u = IN.texcoord.x + f_map_displace;

	float f_new_v = IN.texcoord.y + f_map_displace;

	float2 v_displaced_texcoord = float2(f_new_u, f_new_v);

	float f_new_u2 = (IN.texcoord.x - f_map_displace) / 5;

	float f_new_v2 = (IN.texcoord.y + f_map_displace) / 5;

	OUT.texcoord2 = float2(f_new_u2, f_new_v2);

	float height_sample = t_height.SampleLevel(s_sampler, v_displaced_texcoord, 0).r * 2 - 1;

	float displaced_y = IN.pos.y + (height_sample * f_height_value);

	float4 v_result_coord = float4(IN.pos.x, displaced_y, IN.pos.z, 1); //float4(IN.pos.x, displaced_y, IN.pos.z, 1);

	OUT.worldPos = mul(v_result_coord, m_World);

	OUT.texcoord = v_displaced_texcoord;

	OUT.normal = IN.normal; //normalize(mul(float4(IN.normal, 1), m_World)).xyz;

	OUT.v_campos = v_campos;

	OUT.v_lightPos = v_lightPos;

	OUT.f_displacement_factor = f_displacement_factor;

	OUT.pos = mul(v_result_coord, m_MVP);

	return OUT;
}

float4 pixel(PS_In IN) : SV_TARGET
{
	float4 OUT;

	float4 up = float4(0, 1, 0, 1);

	float3 v_lightDir = normalize(IN.v_lightPos.xyz);

	float f_displacement_map_value = t_displace.Sample(s_sampler, IN.texcoord2);

	f_displacement_map_value = f_displacement_map_value * 2 - 1;


	float2 v_displaced_coords = IN.texcoord;//+float2(f_displacement_map_value * IN.f_displacement_factor, f_displacement_map_value * IN.f_displacement_factor);

	/////////////////////////////////////
	//            AMBIENT              //
	/////////////////////////////////////

	float4 c_ambient = float4(.1, .1, .1, 1);

	/////////////////////////////////////
	//            DIFFUSE              //
	/////////////////////////////////////

	float4 c_diffuse = float4(1,1,1, 1);


	float f_lambert_vertex_normal = saturate(dot(IN.normal, v_lightDir));

	c_diffuse = c_diffuse * f_lambert_vertex_normal;


	float4 c_texture = t_diffuse.Sample(s_sampler, v_displaced_coords);

	OUT = float4(c_texture.rgb /**mbient.rgb + c_diffuse.rgb)*/, .7);

	return OUT;
}

float2 displaceUV(float2 uv, Texture2D displacement_map, SamplerState ss, float f_disp)
{
	float displacement = (displacement_map.Sample(ss, uv).r * 2 - 1) * f_disp;
	return float2(displacement, displacement);
}