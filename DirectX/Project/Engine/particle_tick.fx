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
	
	//��Ȱ��ȭ�Ǿ��ٸ� �Ѿ
	if (!g_Buffer[_ID.x].Active)
	{
		while (g_Spawn[0].SpawnCount)
		{
			int SpawnCount = g_Spawn[0].SpawnCount;         // ����
			int Input = g_Spawn[0].SpawnCount - 1;
			int Origin = 0;
		
			// Atomic �Լ�, �����尡 �ϳ��� ���������� ����
			InterlockedCompareExchange(g_Spawn[0].SpawnCount, SpawnCount, Input, Origin);
			
			// ���� ������ �����常 Orgin ���� ���� ���� SpawnCount�� ���� �� �ִ�.
			if (Origin == SpawnCount)
			{
				g_Buffer[_ID.x].Active = true;
				break;
			}
		}
		
		// Ȱ��ȭ�� ��ƼŬ �ʱ�ȭ
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
		// ���
		
		// Scale ���
		if (Module.Module[3])
		{
			float CurScale = (Module.StartScale * (1 - Particle.NormalizedAge) + Module.EndScale * Particle.NormalizedAge);
			Particle.WorldScale = Particle.InitWorldScale * CurScale;
		}
		
		// Drag ���� ���
		if (g_Module[0].Module[4])
		{
			if (Particle.NormalizedAge < g_Module[0].DestNormalizedAge)
			{
				// ����
				// (���� �ӷ� - ���� �ӷ�) / (���� NA - ���� NA)
				float Gradient = (g_Module[0].LimitSpeed - length(Particle.Velocity)) / (g_Module[0].DestNormalizedAge - Particle.NormalizedAge);
				float NADT = g_EngineDT / Particle.Life;
			
				float NewSpeed = length(Particle.Velocity) + (Gradient * NADT);
				Particle.Velocity = normalize(Particle.Velocity) * NewSpeed;
			}
		}
		
		// Noise Force ���
		if (g_Module[0].Module[5])
		{
			// ���� �ð����� Noise Force �� ������ �����ϰ� ����
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
		
		// Render ���
		if (g_Module[0].Module[6])
		{
			float4 CurColor = (Module.StartColor * (1 - Particle.NormalizedAge) + Module.EndColor * Particle.NormalizedAge);
		
            Particle.Color = CurColor;
			
			// FadeOut�� True�� ������ ���� �ð��� �ٰ����� ��
            if (Module.FadeOut &&  Module.FadeStartRatio <= Particle.NormalizedAge)
			{
                float CurAlpha = saturate(1 - (Particle.NormalizedAge - Module.FadeStartRatio) / (1 - Module.FadeStartRatio));
                Particle.Color.a = CurAlpha;

            }
		}
		
		// ====================
		
		// �ӵ��� ���� �̵�
		// Particle �� �־��� ���� ���� ���ӵ� ���
		float3 vAccel = Particle.Force / Particle.Mass;
		Particle.Force = 0;
		
		// ���ӵ��� ���� �ӵ��� ��ȭ
		Particle.Velocity.xyz += vAccel * g_EngineDT;
		Particle.Velocity.y += g_Module[0].Module[2] ? - Module.GravityScale * g_EngineDT : 0;
		
		// ����
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
		
		// ��ƼŬ�� ������ ���ϸ�
		Particle.Age += g_EngineDT;
		if (Particle.Life < Particle.Age)
		{
			Particle.Active = false;
		}
		
		g_Buffer[_ID.x] = Particle;

	}

}

#endif