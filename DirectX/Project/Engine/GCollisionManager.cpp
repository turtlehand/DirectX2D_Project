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

// ���̾� ���� �浹 
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
	// ���� ������ ����� Collider�� �����´�.
	const vector<GCollider2D*>& vecLeft = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(_Left)->GetCollider2D();
	const vector<GCollider2D*>& vecRight = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(_Right)->GetCollider2D();

	// ���� �ٸ� ���̾�� �浹 �˻�
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

	// ���� ���̾�� �浹 �˻�
	else
	{
		// �ߺ� �˻� or �ڱ��ڽŰ��� �˻縦 ���ϱ� ���ؼ� j�� i +1 ���� �����Ѵ�.
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtwCollider(vecLeft[i], vecRight[j]);
			}
		}
	}
}

// _Left ���̾��� ������Ʈ�� _Right ���̾��� ������Ʈ�� �浹 üũ ����
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

	// �浹 �˻縦 �ϴ� �� ��Ʈ��Ʈ�� 1�� �̻��� ���� �������¶��
	// ������Ʈ��
	bool IsDead = _LeftCol->GameObject()->IsDead() || _RightCol->GameObject()->IsDead() || _LeftCol->IsDead() || _RightCol->IsDead();

	// �浹 ���̴�.
	if (!IsDead && IsOverlap(_LeftCol, _RightCol))
	{
		// ������ �浹�� ���� ���� ����� �浹
		if (!iter->second)
		{
			_LeftCol->OnTriggerEnter(_RightCol);
			_RightCol->OnTriggerEnter(_LeftCol);
		}

		// ������ �浹�� ���� �ְ� ���絵 �浹
		else
		{
			_LeftCol->OnTriggerStay(_RightCol);
			_RightCol->OnTriggerStay(_LeftCol);
		}
		iter->second = true;

	}
	// �浹 ���� �ƴϴ�.
	else
	{
		// ������ �浹�� ���� ������ ����� �ƴ�
		if (iter->second)
		{
			_LeftCol->OnTriggerExit(_RightCol);
			_RightCol->OnTriggerExit(_LeftCol);
		}
		iter->second = false;
	}
}

// �ش� �ݶ��̴����� �浹
bool GCollisionManager::IsOverlap(GCollider2D* _LeftCol, GCollider2D* _RightCol)
{
	// �ڽ� �ݶ��̴��� ���� ��ġ�� �˱����� ���
	const Matrix& matLeft = _LeftCol->GetWorldMat();
	const Matrix& matRight = _RightCol->GetWorldMat();

	// ��� 4���� ��ġ��(�浹ü�� ���� ��ġ)
	Vector3 vLeftPos = matLeft.Translation();
	Vector3 vRightPos = matRight.Translation();

	// �� �浹ü�� �߽��� �մ� ����
	Vector3 vCenter = vRightPos - vLeftPos;

	// ������� �浹ü�� ������ ��ġ�� ã�Ƽ�, �� ǥ�� ������ �˾Ƴ���.
	// �� ���⺤�͸� ���������� ����� ��
	Vector3 vProj[4] = {};

	// �ּҷ� ������ ����
	Vector3 MTV = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	vProj[0] = XMVector3TransformCoord(Vector3(0.5f, -0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matLeft);			// ���� �Ʒ� ������ ������ �Ʒ� �� ����
	vProj[1] = XMVector3TransformCoord(Vector3(-0.5f, 0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matLeft);			// ���� �Ʒ� ������ ���� �� �� ����
	vProj[2] = XMVector3TransformCoord(Vector3(0.5f, -0.5f, 0.f), matRight) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matRight);		// ���� �Ʒ� ������ ������ �Ʒ� �� ����
	vProj[3] = XMVector3TransformCoord(Vector3(-0.5f, 0.5f, 0.f), matRight) - XMVector3TransformCoord(Vector3(-0.5f, -0.5f, 0.f), matRight);		// ���� �Ʒ� ������ ���� �� �� ����

	for (int i = 0; i < 4; ++i)
	{
		Vector3 vProjTarget = vProj[i];
		vProjTarget = vProjTarget.Normalize();		// ���� : �������ͷ� ����

		float ProjLength = 0.f;									// �� ���� ������ ��
		float ProjCenter = 0.f;									// �߽� ���� ������ ��

		for (int j = 0; j < 4; ++j)
		{
			ProjLength += fabs(vProjTarget.Dot(vProj[j]));		// ���� ������ ���� ��Ű�� ������ ���̸� ����
		}
		ProjLength /= 2;

		ProjCenter = fabs(vProjTarget.Dot(vCenter));			// �߽� ������ ������ ����

		// ProjLength < ProjCenter ��� ��賢�� ���ĵ� �浹�� ����
		// ProjLength <= ProjCenter ��� ��賢�� ��ġ�� ���� �浹�� �ƴ�
		if (ProjLength < ProjCenter)							// �߽��� �� ��ٸ� �浹 X
			return false;

		// ���� ���� - �߽� ���� = ħ�� ũ��
		// ���� ���� ũ�Ⱑ ���� ���͸� ����Ͽ� �о
		// ���� �浹���� �ʾҴٸ� ������ return �ǹǷ� ħ�� ũ�Ⱑ ������ �� ������ �ʿ� ����
		MTV = MTV.Length() > ProjLength - ProjCenter ? (ProjLength - ProjCenter ) * vProjTarget : MTV;
	}

	// ������ �ݴ��� �ٲٱ�
	MTV = vCenter.Dot(MTV) < 0.f ? -MTV : MTV;			

	// �о��
	Vector3 lPos = _LeftCol->Transform()->GetWorldPos();
	Vector3 rPos = _RightCol->Transform()->GetWorldPos();

	// RigidBody2D�� ������ �ִ� ��� �о
	if (_LeftCol->RigidBody2D() != nullptr && _RightCol->RigidBody2D() != nullptr)
	{
		// ������ ������� �ʰ� �׳� �ݹݾ� �о
		MTV = MTV / 2;
		_LeftCol->Transform()->SetRelativePos(lPos - MTV);
		_RightCol->Transform()->SetRelativePos(rPos + MTV);
	}
	// �� ��� RigidBody2D�� ������ �ʴ´ٸ� ���� ��� ������ �з���
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
