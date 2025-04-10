#pragma once
#include "GRenderComponent.h"
#include "GParticleTickCS.h"

class GStructuredBuffer;

class GParticleRender :
	public GRenderComponent
{
private:
	GStructuredBuffer*      m_ParticleBuffer;	// 기본 입자 정보
	GStructuredBuffer*		m_SpawnBuffer;		// 생성 관련 버퍼
	GStructuredBuffer*		m_ModuleBuffer;		// 모듈 정보 버퍼

	Ptr<GParticleTickCS>    m_TickCS;
	Ptr<GTexture>			m_ParticleTexture;

	UINT                    m_MaxParticle;		// 파티클 최대 개수

	tParticleModule			m_Module;			// 파티클 제어 설정값
	bool					m_ModuleChanged;	// 해당 Tick에서 모듈값이 변경되었다면 정보를 다시 넘겨줌
	float					m_AccTime;			// 누적시간 체크

	

public:
	void SetMaxParticle(UINT _Max) { m_MaxParticle = _Max; }
	UINT GetMaxParticle() { return m_MaxParticle; }

	void SetParticleTex(Ptr<GTexture> _Tex) { m_ParticleTexture = _Tex; m_ModuleChanged = true; }
	Ptr<GTexture> GetParticleTex() { return m_ParticleTexture; }

	void SetModule(PARTICLE_MODULE _Module, bool _On) { m_Module.Module[(int)_Module] = _On; m_ModuleChanged = true;}
	bool GetModule(PARTICLE_MODULE _Module) { return m_Module.Module[(int)_Module]; }

	// =============================
	// SpawnModule

	// 초당 파티클 생성량 설정
	void SetSpawnRate(float _SpawnRate) { m_Module.SpawnRate = _SpawnRate; }
	float GetSpawnRate() { return m_Module.SpawnRate; }

	void SetStartColor(Vector4 _Color) { m_Module.StartColor = _Color; m_ModuleChanged = true; }
	const Vector4& GetStartColor() { return m_Module.StartColor; }

	void SetMinScale(Vector3 _Scale) { m_Module.MinScale = _Scale; m_ModuleChanged = true; }
	const Vector3& GetMinScale() { return m_Module.MinScale; }

	void SetMaxScale(Vector3 _Scale) { m_Module.MaxScale = _Scale; m_ModuleChanged = true; }
	const Vector3& GetMaxScale() { return m_Module.MaxScale; }

	void SetMinLife(float _MinLife) { m_Module.MinLife = _MinLife; m_ModuleChanged = true; }
	float GetMinLife() { return m_Module.MinLife; }

	void SetMaxLife(float _MaxLife) { m_Module.MaxLife = _MaxLife; m_ModuleChanged = true; }
	float GetMaxLife() { return m_Module.MaxLife; }

	void SetSpawnShape(int _Shape) { m_Module.SpawnShape = _Shape; m_ModuleChanged = true; }	// 0 : Box, 1 : Sphere
	int GetSpawnShape() { return m_Module.SpawnShape; }

	void SetSpawnShapeScale(Vector3 _Scale) { m_Module.SpawnShapeScale = _Scale; m_ModuleChanged = true; }
	const Vector3& GetSpawnShapeScale() { return m_Module.SpawnShapeScale; }

	void SetSpaceType(int _Type) { m_Module.SpaceType = _Type; m_ModuleChanged = true; }		// 0 : Local, 1 : World;
	int GetSpaceType() { return m_Module.SpaceType; }

	// =============================
	// AddVeolcityModule
	// 속도
	// 0 : Random Velocity(무작위), 1 : FromCenter(밖으로), 2: ToCenter(안으로), 3 : Fixed(고정)
	void SetAddVelocityType(int _Type) { m_Module.AddVelocityType = _Type; m_ModuleChanged = true; }
	int GetAddVelocityType() { return m_Module.AddVelocityType; }

	// 무작위 시 최소 속도
	void SetMinSpeed(float _MinSpeed) { m_Module.AddMinSpeed = _MinSpeed; m_ModuleChanged = true; }
	float GetMinSpeed() { return m_Module.AddMinSpeed; }

	// 무작위 시 최대 속도
	void SetMaxSpeed(float _MaxSpeed) { m_Module.AddMaxSpeed = _MaxSpeed; m_ModuleChanged = true;}
	float GetMaxSpeed() { return m_Module.AddMaxSpeed; }

	// 고정 속도
	void SetFixedVelocity(Vector3 _FixedVelocityDir) { m_Module.AddVelocityFixedDir = _FixedVelocityDir; m_ModuleChanged = true;}
	const Vector3& GetFixedVelocity() { return m_Module.AddVelocityFixedDir; }

	//=====================
	// SpawnBurst
	// 특정 Term마다 여러개의 파티클 생성
	void SetBurstParticleCount(int _Count) { m_Module.SpawnBurstCount = _Count; m_ModuleChanged = true;}
	int GetBurstParticleCount() { return m_Module.SpawnBurstCount; }

	void SetBurstRepeatCount(int _Count) { m_Module.SpawnBurstRepeat = _Count; m_ModuleChanged = true;}
	int GetBurstRepeatCount() { return m_Module.SpawnBurstRepeat; }

	void SetBurstTerm(float _Term) { m_Module.SpawnBurstRepeatTime = _Term; m_ModuleChanged = true;}
	float GetBurstTerm() { return m_Module.SpawnBurstRepeatTime; }

	// ==========================
	// ScaleModule
	// 크기
	void SetStartScale(float _Start) { m_Module.StartScale = _Start; m_ModuleChanged = true;}
	float GetStartScale() {return m_Module.StartScale; }

	void SetEndScale(float _End) { m_Module.EndScale = _End; m_ModuleChanged = true;}
	float GetEndScale() { return m_Module.EndScale; }

	// ==========================
	// DragModule
	// 감속

	// 감속되는 시간
	void SetDragDestNormalizeAge(float _Age) { m_Module.DestNormalizedAge = _Age; m_ModuleChanged = true;}
	float GetDragDestNormalizeAge() { return m_Module.DestNormalizedAge; }
	
	// 감속 시 최소 속도
	void SetDragLimitSpeed(float _LimitSpeed) { m_Module.LimitSpeed = _LimitSpeed; m_ModuleChanged = true;}
	float GetDragLimitSpeed() {return m_Module.LimitSpeed; }

	// ==========================
	// NoiseForce
	// 특정 때 마다 무작위로 힘을 줌
	void SetNoiseForceTerm(float _Term) { m_Module.NoiseForceTerm = _Term; m_ModuleChanged = true;}
	float GetNoiseForceTerm() { return m_Module.NoiseForceTerm; }

	void SetNoiseForceScale(float _Scale) { m_Module.NoiseForceScale = _Scale; m_ModuleChanged = true;}
	float GetNoiseForceScale() { return m_Module.NoiseForceScale; }

	// ==========================
	// Render Module
	void SetEndColor(Vector4 _Color) { m_Module.EndColor = _Color; m_ModuleChanged = true; }
	Vector4 GetEndColor() { return m_Module.EndColor; }

	// 사라짐
	void SetFadeOut(bool _FadeOut) { m_Module.FadeOut = _FadeOut; }
	bool GetFadeOut() { return m_Module.FadeOut; }

	// 사라지는 시간
	void SetFadeOutStartRatio(float _Ratio) { m_Module.FadeStartRatio = _Ratio; }
	float GetFadeOutStartRatio() { return m_Module.FadeStartRatio; }

	void SetVelocityAlignment(bool _Use, bool _CrossMesh) { m_Module.VelocityAlignment = _Use; m_Module.CrossMesh = _CrossMesh; }
	bool GetVelocityAlignment() { return m_Module.VelocityAlignment; }
	bool GetCrossMesh() { return m_Module.CrossMesh; }


public:
	virtual void FinalUpdate() override;
	virtual void Render() override;

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

private:
	void CreateMtrl();
	void CalcSpawnCount();		// 생성할 입자 개수 

public:
	CLONE(GParticleRender);
	GParticleRender();
	GParticleRender(const GParticleRender& _Origin);
	~GParticleRender();
};

