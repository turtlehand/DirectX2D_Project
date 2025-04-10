#ifndef _SPRITE
#define _SPRITE

#include "value.fx"
#include "func.fx"

#define ATLAS_TE    g_tex_0
#define LeftTop     g_vec2_0
#define Slice		g_vec2_1
#define AColor		g_vec4_0;


// VertexShader
struct VS_IN
{
	float3 vPos : POSITION; // Semantic, InputLayout
	float2 vUV : TEXCOORD;
};

struct VS_OUT
{
	// System Value Semantic
	// SV_Position : 레스터라이저에 NDC 좌표를 전달할 때 사용하는 Semantic
	float4 vPosistion : SV_Position;
	float2 vUV : TEXCOORD;
    float2 vWorldPos : POSITION;
};

VS_OUT VS_SPRITE(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	output.vPosistion = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
	
	return output;
}

// PixelShader
float4 PS_SPRITE(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
	
    // 해당 UV에 정보를 샘플링 하여 가져온다.
    vColor = g_tex_0.Sample(g_sam_1, _in.vUV * Slice + LeftTop) * AColor;
    if (vColor.a == 0.f)
	{
		discard;
	}
	
    float3 vLightColor = float3(0.f, 0.f, 0.f);
    CalcLight2D(_in.vWorldPos.xy, vLightColor);
    vColor.rgb *= vLightColor;
	
    return vColor;
}

#endif