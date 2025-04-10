#ifndef _FLIPBOOK
#define _FLIPBOOK

#include "value.fx"

#define ATLAS_TE    g_tex_0
#define LeftTop     g_vec2_0
#define Slice		g_vec2_1
#define Offset      g_vec2_2
#define BackGround  g_vec2_3
#define AColor		g_vec4_0

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
};

VS_OUT VS_Flipbook(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	output.vPosistion = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;
	
	return output;
}

// PixelShader
float4 PS_Flipbook(VS_OUT _in) : SV_Target
{
	float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
	
	float2 vUV = (Slice - BackGround) * 0.5f + (BackGround * _in.vUV) + LeftTop - Offset;

    if (vUV.x < LeftTop.x || (LeftTop.x + Slice.x) < vUV.x
            || vUV.y < LeftTop.y || (LeftTop.y + Slice.y) < vUV.y)
    {
        discard;
    }
	
	// 해당 UV에 정보를 샘플링 하여 가져온다.
	vColor = g_tex_0.Sample(g_sam_1, vUV);
    vColor = vColor * AColor;
	if (vColor.a == 0.f)
	{
		discard;
	}
	
	return vColor;
}

#endif