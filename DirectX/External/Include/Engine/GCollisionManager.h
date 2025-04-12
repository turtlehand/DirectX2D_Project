#pragma once
#include "GSingleton.h"

class GCollider2D;

union COLLISION_ID
{
	struct
	{
		UINT left;
		UINT right;
	};

	DWORD_PTR ID;
};

class GCollisionManager :
	public GSingleton<GCollisionManager>
{
	SINGLE(GCollisionManager);
private:
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;

	PxFoundation* m_Foundation = NULL;
	PxPhysics* m_Physics = NULL;

	PxDefaultCpuDispatcher* m_Dispatcher = NULL;
	PxScene* m_Scene = NULL;

	PxMaterial* m_Material = NULL;

	UINT m_Matrix[MAX_LAYER];
	map<DWORD_PTR, bool> m_mapInfo;

public:
	void Init();
	void Progress();
	void CollisionLayerCheck(UINT _Left, UINT _Right) {

		UINT Row = _Left;		// ��
		UINT Col = _Right;		// ��

		if (Col < Row)
		{
			Row = _Right;
			Col = _Left;
		}
		
		m_Matrix[Row] ^=  (1 << Col);
	}
	void SetCollisionLayer(UINT _Row, UINT _Col) { m_Matrix[_Row] = _Col; }
	UINT GetCollisionLayer(UINT _Row) { return m_Matrix[_Row]; }

private:
	void CollisionBtwLayer(UINT _Left, UINT _Right);
	void CollisionBtwCollider(GCollider2D* _LeftCol, GCollider2D* _RightCol);
	bool IsOverlap(GCollider2D* _LeftCol, GCollider2D* _RightCol);
};

