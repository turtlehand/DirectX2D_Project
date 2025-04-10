#pragma once
#include "RenderComponentUI.h"

class GParticleRender;

class ParticleRenderUI :
    public RenderComponentUI
{
private:
    GParticleRender* m_ParticleRender;

public:
    virtual void Render_UI() override;

private:
    void SelectTexture(DWORD_PTR _UI);

    void Texture();
    void Spawn();
    void SpawnBurst();
    void Velocity();
    void Scale();
    void Drag();
    void NoiseForce();
    void RenderModule();


public:
    ParticleRenderUI();
    ~ParticleRenderUI();
};