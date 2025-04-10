#pragma once
#include "GComponent.h"
class GCamera :
	public GComponent
{
private:
	PROJECTION_TYPE m_ProjType;			// 투여방법
	float			m_OrthoScaleX;		// 직교 투영 가로길이	
	float			m_AspectRatio;		// 가로 세로 종횡비
	float			m_FOV;				// 시야각(FieldOfView)
	float			m_Far;				// 최대 시야거리
	int				m_Priority;			// 카메라 우선순위, -1 : 미등록 카메라, 0 : 메인 카메라, 1 ~-> :서브 카메라
	UINT			m_LayerCheck;		// 카메라가 렌더링할 비트 체크

	Matrix			m_matView;			// View 행렬
	Matrix			m_matProj;			// 투영행렬
	
	// 물체 분류 용도
	vector<GGameObject*> m_vecOpaque;
	vector<GGameObject*> m_vecMasked;
	vector<GGameObject*> m_vecTransparent;
	vector<GGameObject*> m_vecPostProcess;


public:
	void SetProjType(PROJECTION_TYPE _Type) { m_ProjType = _Type; }
	void SetOrthoScaleX(float _Scale) { m_OrthoScaleX = max(_Scale, 1.f); }
	void SetAspectRatio(float _Ratio) { m_AspectRatio = _Ratio; }
	void SetFOV(float _FOV) { m_FOV = clamp(_FOV, 1.f, 180.f) * (XM_PI/180.f); }
	void SetFar(float _Far) { m_Far = clamp(_Far, 10.f, 10000.f); }
	void SetPriority(int _Priority) { m_Priority = _Priority; }

	PROJECTION_TYPE GetProjType() { return m_ProjType; }
	float GetOrthoScaleX() { return m_OrthoScaleX; }
	float GetAspectRatio() { return m_AspectRatio; }
	float GetFar() { return m_Far; }
	float GetFOV() { return m_FOV * (180.f / XM_PI); }
	void CheckLayer(UINT _LayerIdx) {
		// ^ : Xor 연산
		// 1 비트를 _LayerIdx만큼 Shift 한다.
		m_LayerCheck = m_LayerCheck ^ (1 << _LayerIdx);
	}
	bool GetLayerCheck(UINT _LayerIdx)		// 해당 레이어가 체크되어있는지 알려준다.
	{
		return m_LayerCheck & (1 << _LayerIdx);
	}
	int GetPriority() { return m_Priority; }

	/// <summary>
	/// 모든 레이어 Render
	/// </summary>
	void CheckLayerAll() { m_LayerCheck = 0xffffffff; }

	void ClearLayer() { m_LayerCheck = 0; }

	const Matrix& GetProjMat() { return m_matProj; }

private:
	void SortObject();

public:
	void Begin();
	void FinalUpdate();
	void Render();
	void End();

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GCamera)

public:
	GCamera();
	GCamera(const GCamera& _Origin);
	virtual ~GCamera();

	friend class GRenderManager;
};

