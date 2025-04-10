#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

#define AColor g_vec4_0

// VertexShader
struct VS_IN
{
	float3 vPos : POSITION;     // Semantic, InputLayout
	float4 vColor : COLOR;
	float2 vUV : TEXCOORD;
};

struct VS_OUT
{
	// System Value Semantic
	// SV_Position : 레스터라이저에 NDC 좌표를 전달할 때 사용하는 Semantic
	float4 vPosistion : SV_Position;
	float4 vColor : COLOR;
	float2 vUV : TEXCOORD;
	float2 vWorldPos : POSITION;
};

VS_OUT VS_Std2D(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	output.vPosistion = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vColor = _in.vColor;
	output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
	return output;
}

// PixelShader
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
	// 해당 UV에 정보를 샘플링 하여 가져온다.
    float4 vSampleColor = float4(1.0f, 0.0f, 1.0f, 1.f);
	
	if (g_bool_tex_0)
		vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    vSampleColor *= AColor;
	
	/*
	if (g_bool_tex_1)
	{
		float NoiseR = g_tex_1.Sample(g_sam_1, _in.vUV).g;
		
        if (1.f < NoiseR + g_float_0)
            discard;

    }
	*/
	
    float3 vLightColor = float3(0.f,0.f,0.f);
    CalcLight2D(_in.vWorldPos.xy, vLightColor);
    vSampleColor.rgb *= vLightColor;
	
	return vSampleColor;
}

float4 PS_Std2D_AlphaBlend(VS_OUT _in) : SV_Target
{
	// 해당 UV에 정보를 샘플링 하여 가져온다.
    float4 vSampleColor = float4(1.0f, 0.0f, 1.0f, 1.f);

    if (g_bool_tex_0)
        vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);
	
    vSampleColor *= AColor;
	
    float3 vLightColor = float3(0.f, 0.f, 0.f);
    CalcLight2D(_in.vWorldPos.xy, vLightColor);
    vSampleColor.rgb *= vLightColor;
	
	return vSampleColor;
}

#endif