#ifndef _PARTICLE_FUNC
#define _PARTICLE_FUNC

#include "value.fx"

// 각 float 범위는 0 ~ 1
float3 GetRandom(float _NormalizedThreadID, in Texture2D _NoiseTex)
{
	float2 vUV = (float2) 0.f;
	vUV.x = _NormalizedThreadID + g_EnginAccTime * 0.1f;
	vUV.y = 0.5f + sin(vUV.x * 100.f * PI) * 0.5f;
		
	float3 vRandom = _NoiseTex.SampleLevel(g_sam_0, vUV, 0);
	return vRandom;
}

// in 읽기전용 참조
// out 쓰기 전용 참조
// input 읽기 쓰기 가능 참조
void ParticleInit(inout tParticle _Particle, in tParticleModule _Module
					, in Texture2D _NoiseTex, int _IsNoiseBind, float _NomalizedThreadID)
{
	// Box
	if(0 == _Module.SpawnShape)
	{
		// NoiseTexture가 없으면 랜덤값은 전부 0으로 본다.
		float3 vRandom = GetRandom(_NomalizedThreadID, _NoiseTex);
		
		// 랜덤 범위를 -0.5 ~ 0.5로 변경 후 스퐅영역 크기를 곱해서 위치값을 구한다.
		float3 vSpawnPos = (vRandom - 0.5f) * _Module.SpawnShapeScale;
		
		_Particle.LocalPos.xyz = vSpawnPos;
	}
	// Sphere
	else if(1 == _Module.SpawnShape)
	{
		
	}
	
	// 파티클 Life 랜덤 설정
	_Particle.Age = 0.f;
	float3 vRandom = GetRandom(_NomalizedThreadID + 0.1f, _NoiseTex);
    _Particle.Life = _Module.MinLife + (_Module.MaxLife - _Module.MinLife) * vRandom.r;
	
	// 파티클 초기 색 설정
	_Particle.Color = _Module.StartColor;
	
	// 파티클 초기 크기 설정
    vRandom = GetRandom(_NomalizedThreadID + 0.2f, _NoiseTex);
    _Particle.InitWorldScale.xyz = _Particle.WorldScale.xyz = _Module.MinScale + (_Module.MaxScale - _Module.MinScale) * vRandom.r;
	
	// 파티클 중량 설정
    _Particle.Mass = 1.f;
	
    _Particle.Velocity = 0.f;
	
	// 속도 모듈
	if(_Module.Module[2])
    {
        float3 vRandom = GetRandom(_NomalizedThreadID + 0.2f, _NoiseTex);
        
        float fSpeed = _Module.AddMinSpeed + (_Module.AddMaxSpeed - _Module.AddMinSpeed) * vRandom.x;
                        
        // Random		무작위
        if (0 == _Module.AddVelocityType)                        
            _Particle.Velocity.xyz = normalize(vRandom - 0.5f) * fSpeed;
        // FromCenter	중심에서
        else if (1 == _Module.AddVelocityType)                        
            _Particle.Velocity.xyz = normalize(_Particle.LocalPos) * fSpeed;
        // ToCenter		중심으로
        else if (2 == _Module.AddVelocityType)
            _Particle.Velocity.xyz = -normalize(_Particle.LocalPos) * fSpeed;
        // Fixed		고정 방향
        else
            _Particle.Velocity.xyz = _Module.AddVelocityFixedDir;

    }
	
    _Particle.WorldPos.xyz = _Particle.LocalPos.xyz + _Module.ObjectWorldPos;
    

}

#endif