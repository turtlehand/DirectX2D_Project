#ifndef _DEBUG_SHAPE
#define _DEBUG_SHAPE

#include "value.fx"

struct VS_IN
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float2 vUV : TEXCOORD;
};

VS_OUT VS_DebugShape(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;
	
	return output;
}

VS_OUT VS_DebugLine(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = float4(_in.vPos, 1.f);
    output.vUV = float2(0.0f, 0.0f);
	
    return output;
}

// ��ȯ��
struct GS_OUT
{
    float4 vPosition : SV_Position; // �����Ͷ��������� ������ ��, NDC
    float2 vUV : TEXCOORD;
};

// �����Ͷ��������� ���� ������ �߰��ϹǷ� ��ȯ Ÿ���� Void
// ������ �ϳ��̹Ƿ� _in�� 1
[maxvertexcount(2)]
void GS_DebugLine(point VS_OUT _in[1], inout LineStream<GS_OUT> _Stream)
{
    // �����Ͷ����� ����
    GS_OUT output[2];
	
	// �簢�� ���� ����
    output[0].vPosition.xyz = _in[0].vPosition.xyz;
    output[1].vPosition.xyz = g_vec4_1.xyz + _in[0].vPosition.xyz;
    
    
    for (int i = 0; i < 2; ++i)
    {
        output[i].vPosition = mul(float4(output[i].vPosition.xyz, 1.f), g_matWVP);
    }
    
	
	// UV ��ǥ ����
    output[0].vUV = float2(0.5f, 0.5f);
    output[1].vUV = float2(1.0f, 0.5f);
	
	// ������ ���� �߰�
    _Stream.Append(output[0]);
    _Stream.Append(output[1]);
    _Stream.RestartStrip();
}

float4 PS_DebugShape(VS_OUT _in) :SV_Target
{
	return g_vec4_0;
}


#endif