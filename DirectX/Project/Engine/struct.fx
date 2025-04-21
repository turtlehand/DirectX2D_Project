#ifndef _STRUCT
#define _STRUCT

// Particle
struct tParticle
{
    float4  LocalPos; // ���� ������Ʈ�� ������ ��� ��ǥ
    float4  WorldPos; // ��ƼŬ�� ���� ��ǥ
    float4  InitWorldScale; // ��ƼŬ�� �ʱ� ���� ũ��
    float4  WorldScale; // ��ƼŬ ���� ũ��
    float4  Color; // ��ƼŬ ����

    float4  Force; // ��ƼŬ�� �־����� �ִ� ���� ����
    float4  Velocity; // ��ƼŬ �ӵ�
    float   Mass; // ��ƼŬ ����
    
    float   NoiseForceAccTime;
    float3  NoiseForceDir; // ����� Noise Forec �� ����

    float   Age; // ��ƼŬ ����, Age �� Life �� �����ϸ� ������ ���� ��
    float   Life; // ��ƼŬ �ִ� ����
    float   NormalizedAge; // ��ü ���� ���, ���� Age ����. �ڽ��� Age �� Life ��� ����ȭ �� ��

    int     Active; // ��ƼŬ Ȱ��ȭ ����
    int     padding[3]; // ��ƼŬ ����ü ũ�⸦ 16 ����� ���߱� ���� �е� ����Ʈ
};

// SpawnCount
struct tSpawnCount
{
    int     SpawnCount;
    float3  vPadding;
};

// Particle Module
// ��ƼŬ ���� ����
struct tParticleModule
{
	// Spawn Module
    float   SpawnRate; // �ʴ� ��ƼŬ ������
    float4  StartColor; // �ʱ� ��ƼŬ ����
    float3  MinScale; // ���� �� �ּ� ũ��
    float3  MaxScale; // ���� �� �ִ� ũ��
    float   MinLife; // ���� �� �ּ� ����
    float   MaxLife; // ���� �� �ִ� ����
    int     SpawnShape; // 0 : Box, 1 : Spear
    float3  SpawnShapeScale;
    int     SpaceType; // 0 : Local, 2 : World

	// Add Velocity  Module
    float GravityScale;
    uint    AddVelocityType;       // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;
    
    // Spawn Burst  Module
    int     SpawnBurstCount;
    int     SpawnBurstRepeat;
    float   SpawnBurstRepeatTime;
    
    // Scale Module 
    float StartScale; // ���� �� ũ�� ����
    float EndScale; // ������ ũ�� ���� 
    
    // Drag Module (���� ���)
    float DestNormalizedAge;        // ���� ���� ����
    float LimitSpeed;               // �ش� �ӵ����� ũ�ٸ� �ش� �ӵ��� ��
    
    // Noise Force Module (Term���� ������ �ӵ��� �ݿ���)
    float NoiseForceTerm; // Noise Force ���� ��
    float NoiseForceScale; // Noise Force ũ��

	// Render Module
    float4  EndColor; // ��ƼŬ ���� ����
    int     FadeOut; // 0 : Off, 1 : Normalized Age
    float   FadeStartRatio; // FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
    uint    VelocityAlignment; // �ӵ� ���� 0 : Off, 1 : On
    uint    CrossMesh; // �������� �޽� ��� 0 : Off, 1 : ON

	// �߰� ������
    float3  ObjectWorldPos;

    int     Module[7];

    //int    padding;
};

struct tLight2DInfo
{
    uint Type; // ���� ����
    float3 Color; // ���� ����
    float3 WorldPos; // ���� ��ġ
    float Radius;       // ���� �ݰ�
    float3 Dir;         // ���� ����
    float Angle;        // ���� ��
		
};

#endif 