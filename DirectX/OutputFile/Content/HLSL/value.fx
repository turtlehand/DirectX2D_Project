#ifndef _VALUE
#define _VALUE

#include "struct.fx"
#define PI 3.1415926535f

// 상수 레지스터
// HLSL 5.0 : 4096
cbuffer TRASNFORM : register(b0)
{
	row_major matrix g_matWorld;        // 월드 행렬 (위치, 회전, 크기) 행렬
	row_major matrix g_matView;         // 카메라 (위치, 회전 역행렬)
	row_major matrix g_matProj;         // 카메라 투영 행렬
	
	
    row_major matrix g_matWV;           // World * View
    row_major matrix g_matWVP;          // World * View * projection
}

// 메테리얼용 상수 버퍼
cbuffer Material : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
	
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
	
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
	
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
	
    row_major Matrix g_mat_0;
    row_major Matrix g_mat_1;
    row_major Matrix g_mat_2;
    row_major Matrix g_mat_3;
    
    int g_bool_tex_0;
    int g_bool_tex_1;
    int g_bool_tex_2;
    int g_bool_tex_3;
    int g_bool_tex_4;
    int g_bool_tex_5;

    int g_padding_0;
    int g_padding_1;

}

cbuffer GLOBAL : register(b2)
{
    float2 g_Resolution;
    float g_DT;
    float g_EngineDT;
    
    float g_AccTime;
    float g_EnginAccTime;
    int g_Light2DCount;
    int g_Light3DCount;
}



// 이미지 텍스쳐 샘플
SamplerState g_sam_0 : register(s0);        // 이방성 필터링
SamplerState g_sam_1 : register(s1);        // 2D

// 텍스쳐 레지스터
// 크기 제한 없음
// 속도가 레지스터에 비해 느림
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

// 2D 광원 정보
StructuredBuffer<tLight2DInfo> g_Light2D : register(t12);

// PostProcess Texture Register
Texture2D PostProcessTarget : register(t14);
#endif