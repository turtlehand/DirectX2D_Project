#pragma once
#include "GComponent.h"
class GCamera :
	public GComponent
{
private:
	PROJECTION_TYPE m_ProjType;			// �������
	float			m_OrthoScaleX;		// ���� ���� ���α���	
	float			m_AspectRatio;		// ���� ���� ��Ⱦ��
	float			m_FOV;				// �þ߰�(FieldOfView)
	float			m_Far;				// �ִ� �þ߰Ÿ�
	int				m_Priority;			// ī�޶� �켱����, -1 : �̵�� ī�޶�, 0 : ���� ī�޶�, 1 ~-> :���� ī�޶�
	UINT			m_LayerCheck;		// ī�޶� �������� ��Ʈ üũ

	Matrix			m_matView;			// View ���
	Matrix			m_matProj;			// �������
	
	// ��ü �з� �뵵
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
		// ^ : Xor ����
		// 1 ��Ʈ�� _LayerIdx��ŭ Shift �Ѵ�.
		m_LayerCheck = m_LayerCheck ^ (1 << _LayerIdx);
	}
	bool GetLayerCheck(UINT _LayerIdx)		// �ش� ���̾ üũ�Ǿ��ִ��� �˷��ش�.
	{
		return m_LayerCheck & (1 << _LayerIdx);
	}
	int GetPriority() { return m_Priority; }

	/// <summary>
	/// ��� ���̾� Render
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

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
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

