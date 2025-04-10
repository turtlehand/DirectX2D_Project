#ifndef _SET_COLOR
#define _SET_COLOR

#include "value.fx"

// RW : Read / Write
RWTexture2D<float4> g_TargertTex : register(u0);


// SV_GroupID               : �����尡 ���� �׷��� �ε��� (��)
// SV_GroupThreadID         : �����尡 ���� �׷� ������ �ش� �������� �ε��� (�� -> ��ȣ)
// SV_GroupIndex            : �����尡 ���� �׷� ������ �ش� �������� �ε����� 1�������� ����
// SV_DispatchThreadID      : ��� �����带 ��Ʋ� �ش� �������� �ε��� (�й�)

// HLSL 5.0 ���� �׷�� ������ ���� : 1024
[numthreads(32,32,1)]   /// 1 Group �� ������ �� 240
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{
    if (g_int_0 <= _ID.x || g_int_1 <= _ID.y)
		return;
	g_TargertTex[_ID.xy] = g_vec4_0;
}

#endif