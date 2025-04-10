#ifndef _FUNC
#define _FUNC

#include "value.fx"

int IsBind(Texture2D _tex)
{
	uint width = 0;
	uint height = 0;
	
	g_tex_0.GetDimensions(width, height);
	
	if (width || height)
	{
		return true;
	}
   
	return false;
}

void CalcLight2D(int _LightIDx, float2 _PixelWorldPos, inout float3 vLightColor)
{
	float DistRatio = 1.f;
	
	// ����ó��
	// ��ü�� ���� * ��������
	// Directional
	if (g_Light2D[_LightIDx].Type == 0)
	{
		vLightColor += g_Light2D[_LightIDx].Color;
	}
	// Point
	else if (g_Light2D[_LightIDx].Type == 1)
	{
		float fDist = distance(g_Light2D[_LightIDx].WorldPos.xy, _PixelWorldPos);
		
		// �߽ɿ� �������� ���� ���� Ŀ���� 1
		// �߽ɿ��� �ּ��� ���� ���� �۾��� 0
		// saturate 1 ~ 0���� ����
        if (g_Light2D[_LightIDx].Radius < fDist)
            return;
		
		DistRatio = saturate(cos(fDist / g_Light2D[_LightIDx].Radius * 0.5 * PI));
		
		vLightColor += DistRatio * g_Light2D[_LightIDx].Color;

	}
	// Spot
	else if(g_Light2D[_LightIDx].Type == 2)
	{
		// ���� �ȼ��� �Ÿ�
        float fDist = length(g_Light2D[_LightIDx].WorldPos.xy - _PixelWorldPos);
		
		// ========
		// ���� ���
		
		// ������ ���ߴ� ����
        float Angle = g_Light2D[_LightIDx].Angle / 2;
		
		// ������ ���� ����
        float2 lDir = normalize(g_Light2D[_LightIDx].Dir);
	
		// �������� �ȼ��� ����
        float2 pDir = normalize(_PixelWorldPos - g_Light2D[_LightIDx].WorldPos.xy);
		
		// �������� �ȼ��� ���Ⱒ
        float Theta = acos(dot(lDir, pDir));
        
		
		// ������ ���ߴ� �������� �������� �ȼ��� ���Ⱒ�� ũ�ٸ� ���� ���� �ʴ� ���̴�.
        if (Angle < Theta || g_Light2D[_LightIDx].Radius < fDist)
        {
            return;
        }
	
		
		
        
		
		// �߽ɿ� �������� ���� ���� Ŀ���� 1
		// �߽ɿ��� �ּ��� ���� ���� �۾��� 0
		// saturate 1 ~ 0���� ����
		DistRatio = saturate(cos(fDist / g_Light2D[_LightIDx].Radius * 0.5 * PI));
		
		vLightColor += DistRatio * g_Light2D[_LightIDx].Color;
		
	}
	
}

void CalcLight2D(float2 _PixelWorldPos, inout float3 vLightColor)
{
	for (int i = 0; i < g_Light2DCount ; ++i)
	{
		CalcLight2D(i, _PixelWorldPos, vLightColor);
	}
	
}

#endif