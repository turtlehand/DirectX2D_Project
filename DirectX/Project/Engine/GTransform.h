#pragma once
#include "GComponent.h"
class GTransform :
	public GComponent
{
private:
	Vector3		m_RelativePosition;
	Vector3		m_RelativeScale;
	Vector3		m_RelativeRotation;

	Vector3		m_RelativeDir[(UINT)DIRECTION_TYPE::END];
	Vector3		m_WorldDir[(UINT)DIRECTION_TYPE::END];

	Matrix		m_matWorld;

public:
	void SetRelativePos(Vector3 _Pos) { m_RelativePosition = _Pos; }
	void SetRelativePos(float _x, float _y, float _z) { m_RelativePosition = Vector3(_x, _y, _z); }

	void SetRelativeScale(Vector3 _Scale) { m_RelativeScale = _Scale; }
	void SetRelativeScale(float _x, float _y, float _z) { m_RelativeScale = Vector3(_x, _y, _z); }

	void SetRelativeRotation(Vector3 _Rotation);				// Degree(��) ������ �Է��ؾ��Ѵ�.
	void SetRelativeRotation(float _x, float _y, float _z);		// Degree(��) ������ �Է��ؾ��Ѵ�.

	Vector3 GetRelativePos() const { return m_RelativePosition; }
	Vector3 GetWorldPos() const { return m_matWorld.Translation(); }

	Vector3 GetRelativeScale() const { return m_RelativeScale; }
	Vector3 GetWorldScale() const;

	Vector3 GetRelativeRotation() const { return m_RelativeRotation * (180.f / XM_PI); }		// Degree(��) ������ ������� ��ȯ���ش�.
	Vector3 GetWorldRotation() const;


	Vector3 GetRelativeDirection(DIRECTION_TYPE _Type) const { return m_RelativeDir[(UINT)_Type]; }
	Vector3 GetWorldDirection(DIRECTION_TYPE _Type) const { return m_WorldDir[(UINT)_Type]; }

	const Matrix& GetWorldMat() { return m_matWorld; }
	void SetWorldMat(Matrix _matWorld) { m_matWorld = _matWorld; }
	void SetMatrix();		// ���� Transform ������ ������� ����� �����Ѵ�.

public:
	virtual void FinalUpdate() override;
	void Binding();

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GTransform)

public:
	GTransform();
	~GTransform();
};

