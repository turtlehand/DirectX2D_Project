#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"

#define OBJECT_POS g_vec4_0

StructuredBuffer<tParticle> m_Buffer : register(t20);
StructuredBuffer<tParticleModule> m_Module : register(t21);

struct VS_IN
{
	//float3 vPos : POSITION;         // ������ 0,0,0 �̹Ƿ� �ǹ� ����
	//float2 vUV : TEXCOORD;
	
	uint iInstId : SV_InstanceID;
};

struct VS_OUT
{
	//float3 vPosistion : SV_Position;    // �����Ͷ��������� ������ ��, NDC
	//float2 vUV : TEXCOORD;
	
	float InstID : FOG;
};

VS_OUT VS_Particle(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	output.InstID = _in.iInstId;           // ���� ȸ���� �Ѱ��ֱ⸸ �ϸ� ��
	
	return output;
}

// ��ȯ��
struct GS_OUT
{
	float4 vPosition : SV_Position;        // �����Ͷ��������� ������ ��, NDC
	float2 vUV : TEXCOORD;
	uint InstID : SV_InstanceID;
};

// �����Ͷ��������� ���� ������ �߰��ϹǷ� ��ȯ Ÿ���� Void
// ������ �ϳ��̹Ƿ� _in�� 1
[maxvertexcount(12)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _Stream)
{
	uint InstID = (uint) _in[0].InstID;
	
	// ���� ȸ���� Active�� False��� ��
	if (false == m_Buffer[InstID].Active)
		return;
	
	// m_Buffer[InstID].WorldPos;   �߽���
	// �߽��� * View
	float3 vWorldPos = m_Buffer[InstID].WorldPos;
	float3 vViewPos = mul(float4(vWorldPos, 1.f), g_matView).xyz;
	float3 vScale = m_Buffer[InstID].WorldScale.xyz;
	
	// �����Ͷ����� ����
	GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
	GS_OUT output_cross[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
	
	// �簢�� ���� ����
	output[0].vPosition.xyz = float3(- vScale.x / 2,  + vScale.y / 2, 0.f);
	output[1].vPosition.xyz = float3(+ vScale.x / 2,  + vScale.y / 2, 0.f);
	output[2].vPosition.xyz = float3(+ vScale.x / 2,  - vScale.y / 2, 0.f);
	output[3].vPosition.xyz = float3(- vScale.x / 2,  - vScale.y / 2, 0.f);
	
	// ���� �޽� ���� ����
	if (m_Module[0].CrossMesh)
	{
		output_cross[0].vPosition.xyz = float3(-vScale.x / 2, 0.f, +vScale.y / 2);
		output_cross[1].vPosition.xyz = float3(+vScale.x / 2, 0.f, +vScale.y / 2);
		output_cross[2].vPosition.xyz = float3(+vScale.x / 2, 0.f, -vScale.y / 2);
		output_cross[3].vPosition.xyz = float3(-vScale.x / 2, 0.f, -vScale.y / 2);
	}
	
	// �ӵ� ����(���� �������� ȸ����Ű��, ������ X)
	if (m_Module[0].Module[6])
    {
        if (m_Module[0].VelocityAlignment)
        {
            float3 vR = normalize(mul(float4(m_Buffer[InstID].Velocity.xyz, 0.f), g_matView).xyz);
            float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
            float3 vU = normalize(cross(vF, vR));

            float3x3 vRot = { vR, vU, vF, };
		
            for (int i = 0; i < 4; ++i)
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
			
                if (m_Module[0].CrossMesh)
                {
                    output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
                }
            }
        }
    }

	
	// ���� ��� �ݿ�
	for (int i = 0; i < 4; ++i)
	{
		output[i].vPosition.xyz += vViewPos.xyz;
		output[i].vPosition = mul(float4(output[i].vPosition.xyz, 1.f), g_matProj);
		output[i].InstID = InstID;
		
        if (m_Module[0].Module[6])
        {
			// ���� �޽� ��� ���ϱ�
            if (m_Module[0].CrossMesh)
            {
                output_cross[i].vPosition.xyz += vViewPos.xyz;
                output_cross[i].vPosition = mul(float4(output_cross[i].vPosition.xyz, 1.f), g_matProj);
                output_cross[i].InstID = InstID;

            }
        }
	}
	
	// UV ��ǥ ����
	output[0].vUV = float2(0.f, 0.f);
	output[1].vUV = float2(1.f, 0.f);
	output[2].vUV = float2(1.f, 1.f);
	output[3].vUV = float2(0.f, 1.f);
	
	// ������ ���� �߰�
	_Stream.Append(output[0]);
	_Stream.Append(output[1]);
	_Stream.Append(output[2]);
	_Stream.RestartStrip();
	
	_Stream.Append(output[0]);
	_Stream.Append(output[2]);
	_Stream.Append(output[3]);
	_Stream.RestartStrip();
	
	// ���� �޽� �ѱ��
    if (m_Module[0].Module[6])
    {
        if (m_Module[0].CrossMesh)
        {
            output_cross[0].vUV = float2(0.f, 0.f);
            output_cross[1].vUV = float2(1.f, 0.f);
            output_cross[2].vUV = float2(1.f, 1.f);
            output_cross[3].vUV = float2(0.f, 1.f);
		
		// ������ ���� �߰�
            _Stream.Append(output_cross[0]);
            _Stream.Append(output_cross[1]);
            _Stream.Append(output_cross[2]);
            _Stream.RestartStrip();
	
            _Stream.Append(output_cross[0]);
            _Stream.Append(output_cross[2]);
            _Stream.Append(output_cross[3]);
            _Stream.RestartStrip();
        }
    }


}

float4 PS_Particle(GS_OUT _in) : SV_Target
{
	if(!g_bool_tex_0)
	{
		return float4(0.0f, 1.0f, 1.f, 1.f);
	}
	
	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	
	vColor = vColor * m_Buffer[_in.InstID].Color;
	
	return vColor;
}


float4 PS_Particle_Fire(GS_OUT _in) : SV_Target
{
	float2 LeftTop = float2(1.f / 3.f * (_in.InstID % 3), 1.f / 3.f * (_in.InstID % 3));
	float2 Slice = float2(1.f / 3.f, 1.f / 3.f);
	
	float4 vColor = g_tex_0.Sample(g_sam_1, LeftTop + Slice * _in.vUV);
	
	if (!(vColor.r == 1.f && vColor.g == 1.f && vColor.b == 1.f))
		discard;
	
	vColor = m_Buffer[_in.InstID].Color;
	
	return vColor;
}

#endif