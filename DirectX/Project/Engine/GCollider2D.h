#pragma once
#include "GComponent.h"
class GCollider2D :
	public GComponent
{
private:
	PxRigidDynamic* m_Box;

	Vector3 m_Scale;
	Vector3 m_Offset;

	int m_OverlapCount;
	bool m_NotifyParent;

	Matrix m_matColWorld;

public:
public:
	void SetScale(Vector3 _Scale) { m_Scale = _Scale; }
	void SetScale(float _x, float _y, float _z) { m_Scale = Vector3(_x, _y, _z); }
	Vector3 GetScale() { return m_Scale; }

	void SetOffset(Vector3 _Offset) { m_Offset = _Offset; }
	Vector3 GetOffset() { return m_Offset; }
	
	bool GetNotifyParent() { return m_NotifyParent; }
	void SetNotifyParent(bool _NotrifyParent) { m_NotifyParent = _NotrifyParent; } // �� ������Ʈ�� �浹 �� �θ� ������Ʈ�鿡�� ������ ���� ���θ� �����մϴ�.

	const Matrix& GetWorldMat() { return m_matColWorld; }

public:
	virtual void FinalUpdate() override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	virtual void OnTriggerEnter(GCollider2D* _Other);
	virtual void OnTriggerStay(GCollider2D* _Other);
	virtual void OnTriggerExit(GCollider2D* _Other);

private:
	void NotifyEnter(GCollider2D* _Other);
	void NotifyStay(GCollider2D* _Other);
	void NotifyExit(GCollider2D* _Other);

public:
	CLONE(GCollider2D);

public:
	GCollider2D();
	GCollider2D(const GCollider2D& _Origin);
	~GCollider2D();
};

