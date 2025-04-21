#pragma once

// Vertex
// 물체의 형태를 이루는 가장 기본 단위

struct Vertex
{
	Vector3 vPos;		// 정점의 위치
	Vector4 vColor;		// 정점의 색상
	Vector2 vUV;		// 정점에 입혀질 텍스쳐의 위치(좌표)

	/*
	Vector3 vTangent;
	Vector3 vNormal;
	Vector3 vBinormal;

	Vector4 vWeights;
	Vector4 vIndices;
	*/
};

// DebugShapeInfo
struct tDebugShapeInfo
{
	DEBUG_SHAPE		Shape;
	Vector3			WorldPos;
	Vector3			WorldScale;
	Vector3			WorldRotation;
	Vector3			WorldDirction;
	Matrix			MatWorld;
	Vector4			Color;
	float			CurTime;
	float			Duration;
	bool			DepthTest;
};

struct tTask
{
	TASK_TYPE Type;
	DWORD_PTR Param0;
	DWORD_PTR Param1;
	DWORD_PTR Param2;
};

// =================
// 파티클 관련
// =================

// 파티클 하나의 정보
struct tParticle
{
	Vector4			LocalPos;		// 소유 오브젝트로 부터의 상대 좌표
	Vector4			WorldPos;		// 파티클의 월드 좌표
	Vector4			InitWorldScale;	// 파티클의 초기 월드 크기
	Vector4			WorldScale;		// 파티클 월드 크기
	Vector4			Color;			// 파티클 색상

	Vector4			Force;			// 파티클에 주어지고 잇는 힘의 총합
	Vector4			Velocity;		// 파티클 속도
	float			Mass;			// 파티클 질향

	float			NoiseForceAccTime;
	Vector3			NoiseForceDir;      // 적용된 Noise Forec 의 방향

	float			Age;			// 파티클 나이, Age가 Life에 도달하면 수명이 다한 것
	float			Life;			// 파티클 최대 수명
	float			NormalizedAge;	// 전체 수명 대비, 현재 Age 비율, 자신의 Age를 Life 대비 정규화된 값

	int				Active;
	int				padding[3];		// 파키글 구조체 크기를 16배수로 맞추기 위한 패딩 Byte
};

// GPU에게 넘겨줄 정보
struct tSpawnCount
{
	int SpawnCount = 0.f;
	Vector3 vPadding;
};

// Particle Module
// 파티클 생성 설정
struct tParticleModule
{
	// Spawn Module
	float		SpawnRate = 1.f;				// 초당 파티클 생성량
	Vector4		StartColor = Vector4(1.f,1.f,1.f,1.f);				// 초기 파티클 색상
	Vector3		MinScale = Vector3(1.f,1.f,1.f);				// 생성 시 최소 크기
	Vector3		MaxScale = Vector3(1.f, 1.f, 1.f);				// 생성 시 최대 크기
	float		MinLife = 1.f;				// 생성 시 최소 수명
	float		MaxLife = 1.f;				// 생성 시 최대 수명
	int			SpawnShape;				// 0 : Box, 1 : Spear
	Vector3		SpawnShapeScale;
	int			SpaceType;				// 0 : Local, 2 : World

	// Add Velocity
	float		GravityScale;
	UINT		AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vector3		AddVelocityFixedDir;
	float		AddMinSpeed;
	float		AddMaxSpeed;

	// Spawn Burst
	int			SpawnBurstCount;		// 한번에 발생시키는 Particle 수
	int			SpawnBurstRepeat;		// 버스트 횟수
	float		SpawnBurstRepeatTime;	// 버스트 시키는 시간 간격

	// Scale Module
	float		StartScale;				// 시작 시 크기 배율
	float		EndScale;				// 끝에서 크기 배율 

	// Drag Module (감속 모듈)
	float		DestNormalizedAge;
	float		LimitSpeed;

	// Noise Force Module (Term마다 무작위 속도가 반영됨
	float		NoiseForceTerm; // Noise Force 변경 텀
	float		NoiseForceScale; // Noise Force 크기

	// Render Module
	Vector4		EndColor;	// 파티클 최종 색상
	int			FadeOut;	// 0 : Off, 1 : Normalized Age
	float		FadeStartRatio; // FadeOut 효과가 시작되는 Normalized Age 지점
	UINT		VelocityAlignment;  // 속도 정렬 0 : Off, 1 : On
	UINT		CrossMesh;			// 십자형태 메쉬 사용 0 : Off, 1 : ON

	// 추가 데이터
	Vector3		ObjectWorldPos;

	int			Module[(UINT)PARTICLE_MODULE::END];

	//int			padding;
};

// ===================
// 상수버퍼 연동 구조체
// ===================
struct tTransform
{
	Matrix	matWorld;		// 월드 매트릭스
	Matrix	matView;		// View 매트릭스
	Matrix	matProj;		// 투영 매트릭스

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_Trans;

struct tGlobal
{
	Vector2 g_Resolution;
	float g_DT;
	float g_EngineDT;

	float g_AccTime;
	float g_EnginAccTime;
	int g_Light2DCount;
	int g_Light3DCount;
};
extern tGlobal g_Global;

struct tMtrlConst
{
	int			iArr[4];
	float		fArr[4];
	Vector2		v2Arr[4];
	Vector4		v4Arr[4];
	Matrix		mArr[4];
	int			bTex[(UINT)TEX_PARAM::END];

	int			padding[2];
};

struct tLight2DInfo
{
	LIGHT_TYPE Type;		// 광원 종류
	Vector3 Color;			// 광원 색상
	Vector3 WorldPos;		// 광원 위치		: POINT, SPOT
	float Radius;			// 광원 반지름	: POINT, SPOT 
	Vector3 Dir;			// 광원 방향		: SPOT
	float Angle;			// 광원 각		: SPOT
};

struct tAnimationInfo
{
	bool AnimationTransform = false;					// 현재 애니메이션 위치 적용
	Vector3 Position;									// 오브젝트의 위치
	Vector3 Scale = Vector3(1.f,1.f,1.f);				// 오브젝트의 크기
	Vector3 Rotation;									// 오브젝트의 회전

	Vector4 Color = Vector4(1.f,1.f,1.f,1.f);			// Sprite의 색상

	float Duration = 1.f;			// 현재 스프라이트 유지 시간
};