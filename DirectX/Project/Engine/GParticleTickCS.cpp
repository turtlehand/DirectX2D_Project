#include "pch.h"
#include "GParticleTickCS.h"

#include "GTexture.h"

GParticleTickCS::GParticleTickCS()
    :GComputeShader(L"HLSL\\particle_tick.fx", "CS_ParticleTick", 256, 1, 1)
    , m_ParticleBuffer(nullptr)
    , m_SpawnBuffer(nullptr)
{
}

GParticleTickCS::~GParticleTickCS()
{
}

int GParticleTickCS::Binding()
{
    if (nullptr == m_ParticleBuffer)
        return E_FAIL;

    // ��ƼŬ ����
    m_ParticleBuffer->Binding_CS_UAV(0);

    // ���� ����
    m_SpawnBuffer->Binding_CS_UAV(1);

    // ��� ������
    m_ModuleBuffer->Binding_CS_SRV(20);

    // ��ƼŬ ���� �ִ� ����
    m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();

    // ������ �ؽ���
    if (nullptr != m_NoiseTex)
    {
        m_NoiseTex->Binding_CS_SRV(0);
        m_Const.bTex[0] = 1;
    }
    else
    {
        m_NoiseTex->Clear_CS_SRV(0);
        m_Const.bTex[0] = 0;
    }
       


    return S_OK;
}

void GParticleTickCS::CalcGroupNum()
{
    // X �׷� ���� ����
    m_GroupX = m_ParticleBuffer->GetElementCount() / m_GroupPerThreadX;

    // �������� �ִٸ� X �׷쿡 1 �߰�
    if (m_ParticleBuffer->GetElementCount() % m_GroupPerThreadX)
        m_GroupX += 1;

    m_GroupY = 1;
    m_GroupZ = 1;
}

void GParticleTickCS::Clear()
{
    m_ParticleBuffer->Clear_CS_UAV();
    m_ParticleBuffer = nullptr;

    m_SpawnBuffer->Clear_CS_UAV();
    m_SpawnBuffer = nullptr;

    m_ModuleBuffer->Clear_CS_SRV();
    m_ModuleBuffer = nullptr;

    // ������ �ؽ���
    if (nullptr != m_NoiseTex)
    {
        m_NoiseTex->Clear_CS_SRV(0);
        m_NoiseTex = nullptr;
    }
}


