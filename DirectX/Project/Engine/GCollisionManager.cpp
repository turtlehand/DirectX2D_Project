#include "pch.h"
#include "GCollisionManager.h"

#include "GLevelManager.h"
#include "GLayer.h"
#include "GLevel.h"

#include "GCollider2D.h"
#include "GTransform.h"
#include "GRigidBody2D.h"

GCollisionManager::GCollisionManager()
	: m_Matrix{}
	, m_mapInfo{}
{

}

GCollisionManager::~GCollisionManager()
{

}

// 레이어 끼리 충돌 
void GCollisionManager::Progress()
{
	for (UINT row = 0;row < MAX_LAYER; ++row)
	{
		for (UINT col = row ;col < MAX_LAYER;++col)
		{
			if(m_Matrix[row] & (1<< col))
				CollisionBtwLayer(row, col);
		}
	}
}

void GCollisionManager::CollisionBtwLayer(UINT _Left, UINT _Right)
{
	// 현재 레벨에 저장된 Collider를 가져온다.
	const vector<GCollider2D*>& vecLeft = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(_Left)->GetCollider2D();
	const vector<GCollider2D*>& vecRight = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(_Right)->GetCollider2D();

	// 서로 다른 레이어간의 충돌 검사
	if (_Left != _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtwCollider(vecLeft[i], vecRight[j]);
			}
		}
	}

	// 동일 레이어간의 충돌 검사
	else
	{
		// 중복 검사 or 자기자신과의 검사를 피하기 위해서 j는 i +1 부터 시작한다.
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtwCollider(vecLeft[i], vecRight[j]);
			}
		}
	}
}

// _Left 레이어의 오브젝트와 _Right 레이어의 오브젝트의 충돌 체크 진행
void GCollisionManager::CollisionBtwCollider(GCollider2D* _LeftCol, GCollider2D* _RightCol)
{
	COLLISION_ID CID;
	CID.left = _LeftCol->GetID();
	CID.right = _RightCol->GetID();
	map<DWORD_PTR, bool>::iterator iter = m_mapInfo.find(CID.ID);

	if (iter == m_mapInfo.end())
	{
		m_mapInfo.insert(make_pair(CID.ID, false));
		iter = m_mapInfo.find(CID.ID);
	}

	// 충돌 검사를 하는 두 오트젝트중 1개 이상이 삭제 예정상태라면
	// 컴포넌트도
	bool IsDead = _LeftCol->GameObject()->IsDead() || _RightCol->GameObject()->IsDead() || _LeftCol->IsDead() || _RightCol->IsDead();

	// 충돌 중이다.
	if (!IsDead && IsOverlap(_LeftCol, _RightCol))
	{
		// 이전에 충돌한 적이 없고 현재는 충돌
		if (!iter->second)
		{
			_LeftCol->OnTriggerEnter(_RightCol);
			_RightCol->OnTriggerEnter(_LeftCol);
		}

		// 이전에 충돌한 적이 있고 현재도 충돌
		else
		{
			_LeftCol->OnTriggerStay(_RightCol);
			_RightCol->OnTriggerStay(_LeftCol);
		}
		iter->second = true;

	}
	// 충돌 중이 아니다.
	else
	{
		// 이전에 충돌한 적이 있으나 현재는 아님
		if (iter->second)
		{
			_LeftCol->OnTriggerExit(_RightCol);
			_RightCol->OnTriggerExit(_LeftCol);
		}
		iter->second = false;
	}
}

// 해당 콜라이더끼리 충돌
bool GCollisionManager::IsOverlap(GCollider2D* _LeftCol, GCollider2D* _RightCol)
{
	// 박스 콜라이더의 최종 위치를 알기위한 행렬
	const Matrix& matLeft = _LeftCol->GetWorldMat();
	const Matrix& matRight = _RightCol->GetWorldMat();

	// 행렬 4행의 위치값(충돌체의 월드 위치)
	Vector3 vLeftPos = matLeft.Translation();
	Vector3 vRightPos = matRight.Translation();

	// 두 충돌체의 중심을 잇는 벡터
	Vector3 vCenter = vRightPos - vLeftPos;

	// 월드상의 충돌체의 꼭지점 위치를 찾아서, 각 표면 방향을 알아낸다.
	// 이 방향벡터를 투영축으로 사용할 것
	Vector3 vProj[4] = {};

	// 최소로 겹쳐진 벡터
	Vector3 MTV = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	vProj[0] = XMVector3TransformCoord(Vector3(0.5f, -0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matLeft);			// 왼쪽 아래 점에서 오른쪽 아래 점 벡터
	vProj[1] = XMVector3TransformCoord(Vector3(-0.5f, 0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matLeft);			// 왼쪽 아래 점에서 왼쪽 위 점 벡터
	vProj[2] = XMVector3TransformCoord(Vector3(0.5f, -0.5f, 0.f), matRight) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matRight);		// 왼쪽 아래 점에서 오른쪽 아래 점 벡터
	vProj[3] = XMVector3TransformCoord(Vector3(-0.5f, 0.5f, 0.f), matRight) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matRight);		// 왼쪽 아래 점에서 왼쪽 위 점 벡터

	for (int i = 0; i < 4; ++i)
	{
		Vector3 vProjTarget = vProj[i];
		vProjTarget = vProjTarget.Normalize();		// 직선 : 단위벡터로 변경

		float ProjLength = 0.f;									// 변 투영 길이의 합
		float ProjCenter = 0.f;									// 중심 투영 길이의 합

		for (int j = 0; j < 4; ++j)
		{
			ProjLength += fabs(vProjTarget.Dot(vProj[j]));		// 변을 직선에 투영 시키고 투영된 길이를 더함
		}
		ProjLength /= 2;

		ProjCenter = fabs(vProjTarget.Dot(vCenter));			// 중심 선분을 직선에 투영

		// ProjLength < ProjCenter 라면 경계끼리 겹쳐도 충돌로 인정
		// ProjLength <= ProjCenter 라면 경계끼리 겹치는 것은 충돌이 아님
		if (ProjLength < ProjCenter)							// 중심이 더 길다면 충돌 X
			return false;

		// 투영 길이 - 중심 길이 = 침범 크기
		// 이중 가장 크기가 작은 벡터를 사용하여 밀어냄
		// 만약 충돌하지 않았다면 어차피 return 되므로 침범 크기가 음수가 될 걱정할 필요 없음
		MTV = MTV.Length() > ProjLength - ProjCenter ? (ProjLength - ProjCenter ) * vProjTarget : MTV;
	}

	// 방향이 반대라면 바꾸기
	MTV = vCenter.Dot(MTV) < 0.f ? -MTV : MTV;			

	// 밀어내기
	Vector3 lPos = _LeftCol->Transform()->GetWorldPos();
	Vector3 rPos = _RightCol->Transform()->GetWorldPos();

	// RigidBody2D를 가지고 있는 대상만 밀어냄
	if (_LeftCol->RigidBody2D() != nullptr && _RightCol->RigidBody2D() != nullptr)
	{
		// 질량은 계산하지 않고 그냥 반반씩 밀어냄
		MTV = MTV / 2;
		_LeftCol->Transform()->SetRelativePos(lPos - MTV);
		_RightCol->Transform()->SetRelativePos(rPos + MTV);
	}
	// 한 대상만 RigidBody2D를 가지지 않는다면 가진 대상만 온전히 밀려남
	else if (_RightCol->RigidBody2D() != nullptr)
	{
		_RightCol->Transform()->SetRelativePos(rPos + MTV);
	}
	else if (_LeftCol->RigidBody2D() != nullptr)
	{
		_LeftCol->Transform()->SetRelativePos(lPos - MTV);
	}
	
	return true;
}
