#pragma once
#include "GRenderComponent.h"

#include "GStructuredBuffer.h"
#include "GSprite.h"

#include "GTilePalette.h"

/// <summary>
/// Material Param과 같이 TileRender가 가질 수 있는 팔레트 Param
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
/// Draw가 false라면 타일을 그리지 않는다.
/// Palette에 있는 타일 중 Row Col 타일을 칠한다는 뜻
/// </summary>
struct tTileInfo
{
	bool Draw = false;			// 현재 타일을 그림니다.
	TILE_PALETTE_PARAM Palette;	// 해당 팔레트
	UINT Row;
	UINT Col;
};

class GTileRender :
	public GRenderComponent
{
private:
	
	UINT                    m_Col;                  // 열
	UINT                    m_Row;                  // 행
	Vector2                 m_TileSize;             // 타일 1개의 사이즈
	vector<tTileInfo>		m_vecTileInfo;          // 각 타일의 정보
	GStructuredBuffer*      m_GpuBuffer;			// 타일 정보를 텍스쳐 레지스터로 바인하기 위함

	Ptr<GTilePalette>		m_TilePalette[(UINT)TILE_PALETTE_PARAM::END];		// 해당 타일 컴포넌트는 이 팔레트를 사용한다는 뜻

	// Collider 기능
	bool					m_Collider;				// ture 시 모양에 맞는 콜라이더 추가
	vector<GGameObject*>	m_ColliderObject;		// 오브젝트 리스트

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

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
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

