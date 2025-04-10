#pragma once
#include "GComputeShader.h"

class GTexture;

class GParticleTickCS :
    public GComputeShader
{
private:
    GStructuredBuffer*  m_ParticleBuffer;
    GStructuredBuffer*  m_SpawnBuffer;
    GStructuredBuffer*  m_ModuleBuffer;
    Ptr<GTexture>       m_NoiseTex;

public:
    void SetParticleBuffer(GStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnBuffer(GStructuredBuffer* _Buffer) { m_SpawnBuffer = _Buffer; }
    void SetModuleBuffer(GStructuredBuffer* _Buffer) { m_ModuleBuffer = _Buffer; }
    void SetNoiseTex(Ptr<GTexture> _Noise) { m_NoiseTex = _Noise; }

public:
    virtual int Binding() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

public:
    GParticleTickCS();
    ~GParticleTickCS();
};

