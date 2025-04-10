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

	void SetRelativeRotation(Vector3 _Rotation);				// Degree(도) 단위로 입력해야한다.
	void SetRelativeRotation(float _x, float _y, float _z);		// Degree(도) 단위로 입력해야한다.

	Vector3 GetRelativePos() const { return m_RelativePosition; }
	Vector3 GetWorldPos() const { return m_matWorld.Translation(); }

	Vector3 GetRelativeScale() const { return m_RelativeScale; }
	Vector3 GetWorldScale() const;

	Vector3 GetRelativeRotation() const { return m_RelativeRotation * (180.f / XM_PI); }		// Degree(도) 단위로 변경시켜 반환해준다.
	Vector3 GetWorldRotation() const;


	Vector3 GetRelativeDirection(DIRECTION_TYPE _Type) const { return m_RelativeDir[(UINT)_Type]; }
	Vector3 GetWorldDirection(DIRECTION_TYPE _Type) const { return m_WorldDir[(UINT)_Type]; }

	const Matrix& GetWorldMat() { return m_matWorld; }
	void SetWorldMat(Matrix _matWorld) { m_matWorld = _matWorld; }
	void SetMatrix();		// 현재 Transform 정보를 기반으로 행렬을 생성한다.

public:
	virtual void FinalUpdate() override;
	void Binding();

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GTransform)

public:
	GTransform();
	~GTransform();
};

