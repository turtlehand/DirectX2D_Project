#pragma once

// Vertex
// ��ü�� ���¸� �̷�� ���� �⺻ ����

struct Vertex
{
	Vector3 vPos;		// ������ ��ġ
	Vector4 vColor;		// ������ ����
	Vector2 vUV;		// ������ ������ �ؽ����� ��ġ(��ǥ)

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
// ��ƼŬ ����
// =================

// ��ƼŬ �ϳ��� ����
struct tParticle
{
	Vector4			LocalPos;		// ���� ������Ʈ�� ������ ��� ��ǥ
	Vector4			WorldPos;		// ��ƼŬ�� ���� ��ǥ
	Vector4			InitWorldScale;	// ��ƼŬ�� �ʱ� ���� ũ��
	Vector4			WorldScale;		// ��ƼŬ ���� ũ��
	Vector4			Color;			// ��ƼŬ ����

	Vector4			Force;			// ��ƼŬ�� �־����� �մ� ���� ����
	Vector4			Velocity;		// ��ƼŬ �ӵ�
	float			Mass;			// ��ƼŬ ����

	float			NoiseForceAccTime;
	Vector3			NoiseForceDir;      // ����� Noise Forec �� ����

	float			Age;			// ��ƼŬ ����, Age�� Life�� �����ϸ� ������ ���� ��
	float			Life;			// ��ƼŬ �ִ� ����
	float			NormalizedAge;	// ��ü ���� ���, ���� Age ����, �ڽ��� Age�� Life ��� ����ȭ�� ��

	int				Active;
	int				padding[3];		// ��Ű�� ����ü ũ�⸦ 16����� ���߱� ���� �е� Byte
};

// GPU���� �Ѱ��� ����
struct tSpawnCount
{
	int SpawnCount = 0.f;
	Vector3 vPadding;
};

// Particle Module
// ��ƼŬ ���� ����
struct tParticleModule
{
	// Spawn Module
	float		SpawnRate = 1.f;				// �ʴ� ��ƼŬ ������
	Vector4		StartColor = Vector4(1.f,1.f,1.f,1.f);				// �ʱ� ��ƼŬ ����
	Vector3		MinScale = Vector3(1.f,1.f,1.f);				// ���� �� �ּ� ũ��
	Vector3		MaxScale = Vector3(1.f, 1.f, 1.f);				// ���� �� �ִ� ũ��
	float		MinLife = 1.f;				// ���� �� �ּ� ����
	float		MaxLife = 1.f;				// ���� �� �ִ� ����
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
	int			SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
	int			SpawnBurstRepeat;		// ����Ʈ Ƚ��
	float		SpawnBurstRepeatTime;	// ����Ʈ ��Ű�� �ð� ����

	// Scale Module
	float		StartScale;				// ���� �� ũ�� ����
	float		EndScale;				// ������ ũ�� ���� 

	// Drag Module (���� ���)
	float		DestNormalizedAge;
	float		LimitSpeed;

	// Noise Force Module (Term���� ������ �ӵ��� �ݿ���
	float		NoiseForceTerm; // Noise Force ���� ��
	float		NoiseForceScale; // Noise Force ũ��

	// Render Module
	Vector4		EndColor;	// ��ƼŬ ���� ����
	int			FadeOut;	// 0 : Off, 1 : Normalized Age
	float		FadeStartRatio; // FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
	UINT		VelocityAlignment;  // �ӵ� ���� 0 : Off, 1 : On
	UINT		CrossMesh;			// �������� �޽� ��� 0 : Off, 1 : ON

	// �߰� ������
	Vector3		ObjectWorldPos;

	int			Module[(UINT)PARTICLE_MODULE::END];

	//int			padding;
};

// ===================
// ������� ���� ����ü
// ===================
struct tTransform
{
	Matrix	matWorld;		// ���� ��Ʈ����
	Matrix	matView;		// View ��Ʈ����
	Matrix	matProj;		// ���� ��Ʈ����

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
	LIGHT_TYPE Type;		// ���� ����
	Vector3 Color;			// ���� ����
	Vector3 WorldPos;		// ���� ��ġ		: POINT, SPOT
	float Radius;			// ���� ������	: POINT, SPOT 
	Vector3 Dir;			// ���� ����		: SPOT
	float Angle;			// ���� ��		: SPOT
};

struct tAnimationInfo
{
	bool AnimationTransform = false;					// ���� �ִϸ��̼� ��ġ ����
	Vector3 Position;									// ������Ʈ�� ��ġ
	Vector3 Scale = Vector3(1.f,1.f,1.f);				// ������Ʈ�� ũ��
	Vector3 Rotation;									// ������Ʈ�� ȸ��

	Vector4 Color = Vector4(1.f,1.f,1.f,1.f);			// Sprite�� ����

	float Duration = 1.f;			// ���� ��������Ʈ ���� �ð�
};