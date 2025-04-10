#ifndef _TILE
#define _TILE

#include "value.fx"
#include "func.fx"

struct TileInfo
{
	int iTP;
	float2	vLeftTop;
	float2	vSlice;
	float3 Padding;
};

StructuredBuffer<TileInfo> g_TileInfo : register(t20);

#define ATLAS_TEX_0		g_tex_0
#define ATLAS_TEX_1		g_tex_1
#define ATLAS_TEX_2		g_tex_2
#define ATLAS_TEX_3		g_tex_3

#define MAX_COL			g_int_1
#define MAX_ROW			g_int_2

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
	float3 vWorldPos : Position;
};

VS_OUT VS_TileRender(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	output.vPosistion = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV * float2(MAX_COL, MAX_ROW);
	output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
	return output;
}

// PixelShader
float4 PS_TileRender(VS_OUT _in) : SV_Target
{
	// 해당 UV에 정보를 샘플링 하여 가져온다.
	float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
	
	int2 ColRow = (int2) floor(_in.vUV);		// UV값을 행렬 크기만큼 곱하였다. 이 UV는 1보다 크며 정수로변환
	float2 vSampleUV = frac(_in.vUV);			// 다시 소수로 변환
	
	// ColRow 정보를 사용하여 인덱스를 구한다.
	int idx = ColRow.y * MAX_COL + ColRow.x;
	
	// 타일 팔레이트가 4라면 없는 취급
	if (g_TileInfo[idx].iTP == 4)
		vColor = float4(0.f, 0.f, 0.f, 0.f);
	// 각 타일 정보의 Slice, LeftTop을 가져와 스프라이트를 설정한다.
	else
    {
        if (g_TileInfo[idx].iTP == 0 && g_bool_tex_0)
        {
            vColor = ATLAS_TEX_0.Sample(g_sam_1, (g_TileInfo[idx].vSlice * frac(_in.vUV)) + g_TileInfo[idx].vLeftTop);
        }
        else if (g_TileInfo[idx].iTP == 1 && g_bool_tex_1)
        {
            vColor = ATLAS_TEX_1.Sample(g_sam_1, (g_TileInfo[idx].vSlice * frac(_in.vUV)) + g_TileInfo[idx].vLeftTop);
        }
        else if (g_TileInfo[idx].iTP == 2 && g_bool_tex_2)
        {
            vColor = ATLAS_TEX_2.Sample(g_sam_1, (g_TileInfo[idx].vSlice * frac(_in.vUV)) + g_TileInfo[idx].vLeftTop);
        }
        else if (g_TileInfo[idx].iTP == 3 && g_bool_tex_3)
        {
            vColor = ATLAS_TEX_3.Sample(g_sam_1, (g_TileInfo[idx].vSlice * frac(_in.vUV)) + g_TileInfo[idx].vLeftTop);
        }
    }
		
	
	if (0.f == vColor.a)
		discard;
	
	float3 vLightColor = float3(0.f, 0.f, 0.f);
	CalcLight2D(_in.vWorldPos.xy, vLightColor);
	vColor.rgb *= vLightColor;
	
	return vColor;
}

#endif