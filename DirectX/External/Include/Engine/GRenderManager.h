#pragma once
#include "GTexture.h"

class GCamera;
class GGameObject;
class GStructuredBuffer;

class GLight2D;

class GRenderManager : public GSingleton<GRenderManager>
{
	SINGLE(GRenderManager);
private:
	vector<GCamera*>			m_vecCam;
	GCamera*					m_EditorCam;

	vector<GLight2D*>			m_vecLight2D;
	GStructuredBuffer*			m_Light2DBuffer;

	GGameObject*				m_DebugObject;
	bool						m_DrawDebug;
	vector<tDebugShapeInfo>		m_vecDebugInfo;

	Ptr<GTexture>				m_PostProcessTex;

	void(GRenderManager::* m_RenderFunc)();

public:
	void RegisterCamera(GCamera* _Cam, int _Priority);
	void RegisterEditorCamera(GCamera* _Cam) { m_EditorCam = _Cam; }
	void DeRegisterCamera(GCamera* _Cam);
	void ClearCamera() { m_vecCam.clear(); }

	void RegisterLight2D(GLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecDebugInfo.push_back(_info); }
	void CopyRenderTarget();
	Ptr<GTexture> GetPostProcessTex() { return m_PostProcessTex; }

	GCamera* GetEditorCamera() { return m_EditorCam; }

	void SetRenderMode(bool _IsPlay)
	{
		if (_IsPlay)
			m_RenderFunc = &GRenderManager::Render_Play;
		else
			m_RenderFunc = &GRenderManager::Render_Editor;
	}

	bool IsDrawDebug() { return m_DrawDebug; }
	void SetDrawDebug(bool _DrawDebug) { m_DrawDebug = _DrawDebug; }

public:
	void Init();
	void Progress();

private:
	void Binding();
	void Render_Play();
	void Render_Editor();
	void Clear();
	void DebugRender();
};

