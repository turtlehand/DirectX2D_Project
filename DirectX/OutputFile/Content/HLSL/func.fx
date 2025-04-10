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
	
	// 광원처리
	// 물체의 색상 * 광원색상
	// Directional
	if (g_Light2D[_LightIDx].Type == 0)
	{
		vLightColor += g_Light2D[_LightIDx].Color;
	}
	// Point
	else if (g_Light2D[_LightIDx].Type == 1)
	{
		float fDist = distance(g_Light2D[_LightIDx].WorldPos.xy, _PixelWorldPos);
		
		// 중심에 가까울수록 빛의 양이 커지고 1
		// 중심에서 멀수록 빛의 양이 작아짐 0
		// saturate 1 ~ 0으로 제한
        if (g_Light2D[_LightIDx].Radius < fDist)
            return;
		
		DistRatio = saturate(cos(fDist / g_Light2D[_LightIDx].Radius * 0.5 * PI));
		
		vLightColor += DistRatio * g_Light2D[_LightIDx].Color;

	}
	// Spot
	else if(g_Light2D[_LightIDx].Type == 2)
	{
		// 빛과 픽셀의 거리
        float fDist = length(g_Light2D[_LightIDx].WorldPos.xy - _PixelWorldPos);
		
		// ========
		// 각도 계산
		
		// 광원이 빛추는 각도
        float Angle = g_Light2D[_LightIDx].Angle / 2;
		
		// 광원의 방향 벡터
        float2 lDir = normalize(g_Light2D[_LightIDx].Dir);
	
		// 광원에서 픽셀의 방향
        float2 pDir = normalize(_PixelWorldPos - g_Light2D[_LightIDx].WorldPos.xy);
		
		// 광원에서 픽셀의 방향각
        float Theta = acos(dot(lDir, pDir));
        
		
		// 광원이 빛추는 각도보다 광원에서 픽셀의 방향각이 크다면 빛이 닿지 않는 곳이다.
        if (Angle < Theta || g_Light2D[_LightIDx].Radius < fDist)
        {
            return;
        }
	
		
		
        
		
		// 중심에 가까울수록 빛의 양이 커지고 1
		// 중심에서 멀수록 빛의 양이 작아짐 0
		// saturate 1 ~ 0으로 제한
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