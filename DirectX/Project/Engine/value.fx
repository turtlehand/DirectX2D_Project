#ifndef _VALUE
#define _VALUE

#include "struct.fx"
#define PI 3.1415926535f

// ��� ��������
// HLSL 5.0 : 4096
cbuffer TRASNFORM : register(b0)
{
	row_major matrix g_matWorld;        // ���� ��� (��ġ, ȸ��, ũ��) ���
	row_major matrix g_matView;         // ī�޶� (��ġ, ȸ�� �����)
	row_major matrix g_matProj;         // ī�޶� ���� ���
	
	
    row_major matrix g_matWV;           // World * View
    row_major matrix g_matWVP;          // World * View * projection
}

// ���׸���� ��� ����
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



// �̹��� �ؽ��� ����
SamplerState g_sam_0 : register(s0);        // �̹漺 ���͸�
SamplerState g_sam_1 : register(s1);        // 2D

// �ؽ��� ��������
// ũ�� ���� ����
// �ӵ��� �������Ϳ� ���� ����
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

// 2D ���� ����
StructuredBuffer<tLight2DInfo> g_Light2D : register(t12);

// PostProcess Texture Register
Texture2D PostProcessTarget : register(t14);
#endif