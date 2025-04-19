#pragma once
#include "GComponent.h"
class GCollider2D :
	public GComponent
{
private:
	bool m_IsTrigger;
	Vector3 m_Scale;
	Vector3 m_Offset;

	int m_OverlapCount;
	bool m_NotifyParent;

	Matrix m_matColWorld;

public:
	void SetTrigger(bool _Trigger) { m_IsTrigger = _Trigger; }
	bool IsTrigger() { return m_IsTrigger; }

	void SetScale(Vector3 _Scale) { m_Scale = _Scale; }
	void SetScale(float _x, float _y, float _z) { m_Scale = Vector3(_x, _y, _z); }
	Vector3 GetScale() { return m_Scale; }

	void SetOffset(Vector3 _Offset) { m_Offset = _Offset; }
	Vector3 GetOffset() { return m_Offset; }
	
	bool GetNotifyParent() { return m_NotifyParent; }
	void SetNotifyParent(bool _NotrifyParent) { m_NotifyParent = _NotrifyParent; } // 이 오브젝트가 충돌 시 부모 오브젝트들에게 영향을 줄지 여부를 설정합니다.

	const Matrix& GetWorldMat() { return m_matColWorld; }

public:
	virtual void FinalUpdate() override;

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	virtual void OnOverlapEnter(GCollider2D* _Other);
	virtual void OnOverlapStay(GCollider2D* _Other);
	virtual void OnOverlapExit(GCollider2D* _Other);

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

