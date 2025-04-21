#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "particle_func.fx"

RWStructuredBuffer<tParticle> g_Buffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_Spawn : register(u1);
StructuredBuffer<tParticleModule> g_Module : register(t20);

#define MAX_PARTICLE g_int_0



[numthreads(256, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
	if (MAX_PARTICLE <= _ID.x)
		return;
	
	//비활성화되었다면 넘어감
	if (!g_Buffer[_ID.x].Active)
	{
		while (g_Spawn[0].SpawnCount)
		{
			int SpawnCount = g_Spawn[0].SpawnCount;         // 예상값
			int Input = g_Spawn[0].SpawnCount - 1;
			int Origin = 0;
		
			// Atomic 함수, 스레드가 하나씩 순차적으로 실행
			InterlockedCompareExchange(g_Spawn[0].SpawnCount, SpawnCount, Input, Origin);
			
			// 최초 진입한 스레드만 Orgin 값을 원래 예상값 SpawnCount로 가질 수 있다.
			if (Origin == SpawnCount)
			{
				g_Buffer[_ID.x].Active = true;
				break;
			}
		}
		
		// 활성화된 파티클 초기화
		if (g_Buffer[_ID.x].Active)
		{
			float NormalizedID = (float) _ID.x / (float) MAX_PARTICLE;
			ParticleInit(g_Buffer[_ID.x], g_Module[0], g_tex_0, g_bool_tex_0, NormalizedID);
		}

	}
	else
	{
		tParticle Particle = g_Buffer[_ID.x];
		tParticleModule Module = g_Module[0];
		
		Particle.NormalizedAge = Particle.Age / Particle.Life;
		
		// ====================
		// 모듈
		
		// Scale 모듈
		if (Module.Module[3])
		{
			float CurScale = (Module.StartScale * (1 - Particle.NormalizedAge) + Module.EndScale * Particle.NormalizedAge);
			Particle.WorldScale = Particle.InitWorldScale * CurScale;
		}
		
		// Drag 감속 모듈
		if (g_Module[0].Module[4])
		{
			if (Particle.NormalizedAge < g_Module[0].DestNormalizedAge)
			{
				// 기울기
				// (목적 속력 - 현재 속력) / (목적 NA - 현재 NA)
				float Gradient = (g_Module[0].LimitSpeed - length(Particle.Velocity)) / (g_Module[0].DestNormalizedAge - Particle.NormalizedAge);
				float NADT = g_EngineDT / Particle.Life;
			
				float NewSpeed = length(Particle.Velocity) + (Gradient * NADT);
				Particle.Velocity = normalize(Particle.Velocity) * NewSpeed;
			}
		}
		
		// Noise Force 모듈
		if (g_Module[0].Module[5])
		{
			// 일정 시간마다 Noise Force 의 방향을 랜덤하게 구함
			if (g_Module[0].NoiseForceTerm <= Particle.NoiseForceAccTime)
			{
				Particle.NoiseForceAccTime -= g_Module[0].NoiseForceTerm;
				
				float NormalizedThreadID = (float) _ID.x / (float) MAX_PARTICLE;
				
				float3 vRandom = GetRandom(NormalizedThreadID, g_tex_0);
				float3 vNoiseForce = normalize(vRandom.xyz - 0.5f);

				Particle.NoiseForceDir = vNoiseForce;
			}
						
			Particle.Force.xyz += Particle.NoiseForceDir * g_Module[0].NoiseForceScale;
			Particle.NoiseForceAccTime += g_EngineDT;
		}
		
		// Render 모듈
		if (g_Module[0].Module[6])
		{
			float4 CurColor = (Module.StartColor * (1 - Particle.NormalizedAge) + Module.EndColor * Particle.NormalizedAge);
		
            Particle.Color = CurColor;
			
			// FadeOut이 True고 수명이 시작 시간에 다가왔을 때
            if (Module.FadeOut &&  Module.FadeStartRatio <= Particle.NormalizedAge)
			{
                float CurAlpha = saturate(1 - (Particle.NormalizedAge - Module.FadeStartRatio) / (1 - Module.FadeStartRatio));
                Particle.Color.a = CurAlpha;

            }
		}
		
		// ====================
		
		// 속도에 따른 이동
		// Particle 에 주어진 힘에 따른 가속도 계산
		float3 vAccel = Particle.Force / Particle.Mass;
		Particle.Force = 0;
		
		// 가속도에 따른 속도의 변화
		Particle.Velocity.xyz += vAccel * g_EngineDT;
		Particle.Velocity.y += g_Module[0].Module[2] ? - Module.GravityScale * g_EngineDT : 0;
		
		// 로컬
		if (0 == Module.SpaceType)
		{
			Particle.LocalPos.xyz += g_Buffer[_ID.x].Velocity.xyz * g_EngineDT;
			Particle.WorldPos.xyz = Module.ObjectWorldPos.xyz + Particle.LocalPos.xyz;
		}
		else
		{
			Particle.LocalPos.xyz += g_Buffer[_ID.x].Velocity.xyz * g_EngineDT;
			Particle.WorldPos.xyz += g_Buffer[_ID.x].Velocity.xyz * g_EngineDT;
		}
		
		// 파티클의 수명이 다하면
		Particle.Age += g_EngineDT;
		if (Particle.Life < Particle.Age)
		{
			Particle.Active = false;
		}
		
		g_Buffer[_ID.x] = Particle;

	}

}

#endif