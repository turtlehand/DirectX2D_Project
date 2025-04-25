#pragma once
#include "GRenderComponent.h"

#include "GStructuredBuffer.h"
#include "GSprite.h"

#include "GTilePalette.h"

/// <summary>
/// Material Param�� ���� TileRender�� ���� �� �ִ� �ȷ�Ʈ Param
/// </summary>
enum class TILE_PALETTE_PARAM
{
	PALETTE_0,
	PALETTE_1,
	PALETTE_2,
	PALETTE_3,
	END
};

/// <summary>
/// Draw�� false��� Ÿ���� �׸��� �ʴ´�.
/// Palette�� �ִ� Ÿ�� �� Row Col Ÿ���� ĥ�Ѵٴ� ��
/// </summary>
struct tTileInfo
{
	bool Draw = false;			// ���� Ÿ���� �׸��ϴ�.
	TILE_PALETTE_PARAM Palette;	// �ش� �ȷ�Ʈ
	UINT Row;
	UINT Col;
};

class GTileRender :
	public GRenderComponent
{
private:
	
	UINT                    m_Col;                  // ��
	UINT                    m_Row;                  // ��
	Vector2                 m_TileSize;             // Ÿ�� 1���� ������
	vector<tTileInfo>		m_vecTileInfo;          // �� Ÿ���� ����
	GStructuredBuffer*      m_GpuBuffer;			// Ÿ�� ������ �ؽ��� �������ͷ� �����ϱ� ����

	Ptr<GTilePalette>		m_TilePalette[(UINT)TILE_PALETTE_PARAM::END];		// �ش� Ÿ�� ������Ʈ�� �� �ȷ�Ʈ�� ����Ѵٴ� ��

	// Collider ���
	bool					m_Collider;				// ture �� ��翡 �´� �ݶ��̴� �߰�
	vector<GGameObject*>	m_ColliderObject;		// ������Ʈ ����Ʈ

public:
	void SetRowCol(UINT _Row, UINT _Col);
	void SetTileSize(Vector2 _TileSize);
	void SetTile(UINT _Row, UINT _Col, const tTileInfo& _Info = tTileInfo());
	void SetTilePalette(TILE_PALETTE_PARAM _Param, Ptr<GTilePalette> _Palette) { m_TilePalette[(UINT)_Param] = _Palette; }

	Ptr<GTexture> GetAtlasTex(TILE_PALETTE_PARAM _Palette);
	int GetRow() { return m_Row; }
	int GetCol() { return m_Col; }
	Vector2 GetTileSize() { return m_TileSize; }
	Ptr<GTilePalette> GetTilePalette(TILE_PALETTE_PARAM _Param) { return m_TilePalette[(int)_Param]; }

	bool IsCollider() { return m_Collider; }
	void SetCollider(bool _Collider) { m_Collider = _Collider; }
	const vector<GGameObject*> GetColliders() { return m_ColliderObject; }


public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

private:
	void CreateTileRenderMtrl();
	void UpdateBuffer();
	void AddCollider();
	bool ExpandCheckRight(const Vector3& _leftTop, const Vector3& _rightDown, const vector<int>& _DArray);
	bool ExpandCheckDown(const Vector3& _leftTop, const Vector3& _rightDown, const vector<int>& _DArray);

public:
	CLONE(GTileRender);
	GTileRender();
	GTileRender(const GTileRender& _Origin);
	~GTileRender();


};

