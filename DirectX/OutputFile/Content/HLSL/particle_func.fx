#ifndef _PARTICLE_FUNC
#define _PARTICLE_FUNC

#include "value.fx"

// �� float ������ 0 ~ 1
float3 GetRandom(float _NormalizedThreadID, in Texture2D _NoiseTex)
{
	float2 vUV = (float2) 0.f;
	vUV.x = _NormalizedThreadID + g_EnginAccTime * 0.1f;
	vUV.y = 0.5f + sin(vUV.x * 100.f * PI) * 0.5f;
		
	float3 vRandom = _NoiseTex.SampleLevel(g_sam_0, vUV, 0);
	return vRandom;
}

// in �б����� ����
// out ���� ���� ����
// input �б� ���� ���� ����
void ParticleInit(inout tParticle _Particle, in tParticleModule _Module
					, in Texture2D _NoiseTex, int _IsNoiseBind, float _NomalizedThreadID)
{
	// Box
	if(0 == _Module.SpawnShape)
	{
		// NoiseTexture�� ������ �������� ���� 0���� ����.
		float3 vRandom = GetRandom(_NomalizedThreadID, _NoiseTex);
		
		// ���� ������ -0.5 ~ 0.5�� ���� �� ���j���� ũ�⸦ ���ؼ� ��ġ���� ���Ѵ�.
		float3 vSpawnPos = (vRandom - 0.5f) * _Module.SpawnShapeScale;
		
		_Particle.LocalPos.xyz = vSpawnPos;
	}
	// Sphere
	else if(1 == _Module.SpawnShape)
	{
		
	}
	
	// ��ƼŬ Life ���� ����
	_Particle.Age = 0.f;
	float3 vRandom = GetRandom(_NomalizedThreadID + 0.1f, _NoiseTex);
    _Particle.Life = _Module.MinLife + (_Module.MaxLife - _Module.MinLife) * vRandom.r;
	
	// ��ƼŬ �ʱ� �� ����
	_Particle.Color = _Module.StartColor;
	
	// ��ƼŬ �ʱ� ũ�� ����
    vRandom = GetRandom(_NomalizedThreadID + 0.2f, _NoiseTex);
    _Particle.InitWorldScale.xyz = _Particle.WorldScale.xyz = _Module.MinScale + (_Module.MaxScale - _Module.MinScale) * vRandom.r;
	
	// ��ƼŬ �߷� ����
    _Particle.Mass = 1.f;
	
    _Particle.Velocity = 0.f;
	
	// �ӵ� ���
	if(_Module.Module[2])
    {
        float3 vRandom = GetRandom(_NomalizedThreadID + 0.2f, _NoiseTex);
        
        float fSpeed = _Module.AddMinSpeed + (_Module.AddMaxSpeed - _Module.AddMinSpeed) * vRandom.x;
                        
        // Random		������
        if (0 == _Module.AddVelocityType)                        
            _Particle.Velocity.xyz = normalize(vRandom - 0.5f) * fSpeed;
        // FromCenter	�߽ɿ���
        else if (1 == _Module.AddVelocityType)                        
            _Particle.Velocity.xyz = normalize(_Particle.LocalPos) * fSpeed;
        // ToCenter		�߽�����
        else if (2 == _Module.AddVelocityType)
            _Particle.Velocity.xyz = -normalize(_Particle.LocalPos) * fSpeed;
        // Fixed		���� ����
        else
            _Particle.Velocity.xyz = _Module.AddVelocityFixedDir;

    }
	
    _Particle.WorldPos.xyz = _Particle.LocalPos.xyz + _Module.ObjectWorldPos;
    

}

#endif