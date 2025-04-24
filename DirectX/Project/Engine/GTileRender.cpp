#include "pch.h"
#include "GTileRender.h"

#include "GTransform.h"
#include "GCollider2D.h"

#include "GAssetManager.h"
#include "GStructuredBuffer.h"

#include "GLevelManager.h"


GTileRender::GTileRender()
	: GRenderComponent(COMPONENT_TYPE::TILE_RENDER)
	, m_Collider(false)
	, m_Col(0)
	, m_Row(0)
	, m_vecTileInfo{}
{
	SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));

	CreateTileRenderMtrl();

	m_GpuBuffer = new GStructuredBuffer;
}

GTileRender::GTileRender(const GTileRender& _Origin)
	:GRenderComponent(_Origin)
	, m_Collider(_Origin.m_Collider)
	, m_Col(_Origin.m_Col)
	, m_Row(_Origin.m_Row)
	, m_TileSize(_Origin.m_TileSize)
	, m_GpuBuffer(nullptr)
{
	m_GpuBuffer = new GStructuredBuffer;

	// ����ũ�⸦ Ÿ�� ������ŭ �� ����
	m_vecTileInfo.resize(m_Col * m_Row);
	
	// Ÿ�Ϻ� ���� ����
	for (size_t i = 0; i < _Origin.m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i] = _Origin.m_vecTileInfo[i];
	}

	// Ÿ�� �ȷ�Ʈ ���� ����
	for (size_t i = 0; i < (size_t)TILE_PALETTE_PARAM::END;++i)
	{
		m_TilePalette[i] = _Origin.m_TilePalette[i];
	}
}

GTileRender::~GTileRender()
{
	if (nullptr != m_GpuBuffer)
		delete m_GpuBuffer;
}

void GTileRender::SetRowCol(UINT _Row, UINT _Col)
{
	m_Col = _Col < 1 ? 1 : _Col;
	m_Row = _Row < 1 ? 1 : _Row;

	// ���� ����� ũ�Ⱑ ������ �뷮���� �۴ٸ� ���͸� ��ü�Ͽ� �뷮�� ���̰� ���� ���ʹ� ���ŵȴ�.
	/*
	if ((m_Col * m_Row) < m_vecTileInfo.capacity())
	{
		// ���� ���ͷμ� �������ʹ� ���ŵȴ�.
		vector<tTileInfo*> temp;
		m_vecTileInfo.swap(temp);
		Delete_Vec(temp);
	}
	*/
	m_vecTileInfo.resize(m_Col * m_Row);

	for (size_t i = 0; i < m_vecTileInfo.size();++i)
	{
		m_vecTileInfo[i] = tTileInfo();
	}

	// ������Ʈ ������ ������
	Vector2 vScale = m_TileSize * Vector2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vector3(vScale.x, vScale.y, 1.f));

	// Ÿ�� �����͸� ���۷� ����
	UpdateBuffer();
}

void GTileRender::SetTileSize(Vector2 _TileSize)
{
	m_TileSize.x = _TileSize.x <= 0 ? 0 : _TileSize.x;
	m_TileSize.y = _TileSize.y <= 0 ? 0 : _TileSize.y;
	
	// ������Ʈ ������ ������
	Vector2 vScale = m_TileSize * Vector2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vector3(vScale.x, vScale.y, 1.f));
}

void GTileRender::SetTile(UINT _Row, UINT _Col, const tTileInfo& _Info)
{
	assert(m_Col > _Col && m_Row > _Row);

	int idx = m_Col * _Row + _Col;
	m_vecTileInfo[idx] = _Info;

	// Ÿ�� �����͸� ���۷� ����
	UpdateBuffer();
}

Ptr<GTexture> GTileRender::GetAtlasTex(TILE_PALETTE_PARAM _Palette)
{
	return m_TilePalette[(UINT)_Palette]->GetAtlasTex();
}

void GTileRender::Init()
{
	// ������Ʈ ������ ������
	Vector2 vScale = m_TileSize * Vector2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vector3(vScale.x, vScale.y, 1.f));

	// Ÿ�� �����͸� ���۷� ����
	UpdateBuffer();
}

void GTileRender::Begin()
{
	AddCollider();
}

void GTileRender::FinalUpdate()
{
	// STop ����� ���� DeBugRender�� ǥ���Ѵ�.
	if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::PLAY)
		return;

	Vector3 vPos = Transform()->GetWorldPos();
	Vector3 vScale = Transform()->GetWorldScale() / 2;
	vScale.z = 0.f;

	for (int i = 0; i < m_Row + 1; ++i)
	{
		DrawDebugLine(Vector4(0.f, 1.f, 0.f, 1.f),
			vPos - vScale + Vector3(0, m_TileSize.y * i, 0),
			Vector3(1.f, 0.f, 0.f),
			Transform()->GetWorldScale().x);
	}

	for (int i = 0; i < m_Col + 1; ++i)
	{
		DrawDebugLine(Vector4(0.f, 1.f, 0.f, 1.f),
			vPos - vScale + Vector3(m_TileSize.x * i, 0, 0),
			Vector3(0.f, 1.f, 0.f),
			Transform()->GetWorldScale().y);
	}
}

void GTileRender::Render()
{
	Ptr<GMaterial> Mtrl = GetMaterial();
	// ��ġ ���� ������Ʈ
	Transform()->Binding();

	// ��� ũ�⸦ ���Ѵ�.
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_Col);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_2, m_Row);

	// Ÿ�� �ȷ�Ʈ ������ ������.
	for (int i = 0; i < (int)TILE_PALETTE_PARAM::END; ++i)
	{
		Ptr<GTilePalette> TP = m_TilePalette[i];
		if (TP == nullptr)
			continue;

		GetMaterial()->SetTexture((TEX_PARAM)i, GetAtlasTex((TILE_PALETTE_PARAM)i));
	}

	GetMaterial()->Binding();

	// Ÿ�� ������ �����ϰ� �ִ� ����ȭ ���� ���ε�
	m_GpuBuffer->Binding(20);

	GetMesh()->Render();
}


void GTileRender::CreateTileRenderMtrl()
{
	if (nullptr == GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"TileRenderShader"))
	{
		// TileRenderShader
		Ptr<GGraphicShader> pShader = new GGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\tilerender.fx", "VS_TileRender");
		pShader->CreatePixelShader(L"HLSL\\tilerender.fx", "PS_TileRender");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		GAssetManager::GetInst()->AddAsset(L"TileRenderShader", pShader.Get());
	}

	if (nullptr == GAssetManager::GetInst()->FindAsset<GMaterial>(L"TileRenderMtrl"))
	{
		// TileRenderMtrl
		Ptr<GMaterial> pMtrl = new GMaterial;
		pMtrl->SetShader(GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"TileRenderShader"));
		GAssetManager::GetInst()->AddAsset(L"TileRenderMtrl", pMtrl.Get());
	}

	SetMaterial(GAssetManager::GetInst()->FindAsset<GMaterial>(L"TileRenderMtrl"));
}

/// <summary>
/// ���ۿ� Ÿ�� ������ ������Ʈ �ϴ� ���
/// </summary>
void GTileRender::UpdateBuffer()
{
	struct SpriteInfo {
		int iTP;
		Vector2 vLeftTop;
		Vector2 vSlice;
		Vector3 Padding;
	};

	// ���� ������ ũ�Ⱑ ����� ũ�⺸�� �۴ٸ� ���� �����.
	if (m_GpuBuffer->GetElementCount() < m_Row * m_Col)
	{
		m_GpuBuffer->Create(sizeof(SpriteInfo), m_Row* m_Col, SB_TYPE::SRV_ONLY, true);
	}

	static vector<SpriteInfo> vecInfo;
	vecInfo.clear();

	// ����� ũ�⸸ŭ ���۸� �����.
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		SpriteInfo info;

		// Sprite�� �ִٸ�
		if (m_vecTileInfo[i].Draw)
		{
			UINT Row = m_vecTileInfo[i].Row;
			UINT Col = m_vecTileInfo[i].Col;

			info.iTP = (int)m_vecTileInfo[i].Palette;
			info.vLeftTop = m_TilePalette[(UINT)m_vecTileInfo[i].Palette]->GetTileLeftTop(Row, Col);
			info.vSlice = m_TilePalette[(UINT)m_vecTileInfo[i].Palette]->GetSlice();
		}

		// Sprite�� ���ٸ� ������ �ʱ�ȭ
		else
		{
			info.iTP = (int)TILE_PALETTE_PARAM::END;
			info.vLeftTop = Vector2(-1.f, -1.f);
			info.vSlice = Vector2(-1.f, -1.f);
		}

		vecInfo.push_back(info);
	}

	if (!vecInfo.empty())
	{
		m_GpuBuffer->SetData(vecInfo.data(), sizeof(SpriteInfo), vecInfo.size());
	}
}

void GTileRender::AddCollider()
{
	if (!m_Collider)
		return;

	// ���� ������ �Ʒ���
	vector<int> DArray(m_Row * m_Col, 0);
	int num = 1;

	for (int Row = 0; Row < m_Row; ++Row)
	{
		for (int Col = 0; Col < m_Col; ++Col)
		{
			if (DArray[Row * m_Col + Col] != 0)
				continue;

			if (m_vecTileInfo[Row * m_Col + Col].Draw)
			{
				Vector3 LeftTop = Vector3(Col, Row, 0.f);
				Vector3 RightDown = Vector3(Col, Row, 0.f);

				while (true)
				{
					int rd = (RightDown.y + 1) * m_Col + RightDown.x + 1;
					int r = RightDown.y * m_Col + RightDown.x + 1;
					int d = (RightDown.y + 1) * m_Col + RightDown.x;

					// ������ �Ʒ� ���� ���� �Ѵٸ� 
					if (RightDown.x + 1 < m_Col && RightDown.y + 1 < m_Row && m_vecTileInfo[rd].Draw && DArray[rd] == 0)
					{
						// ������, �Ʒ� �� ��� Ȯ��
						// ������ �� ��� Ȯ��
						bool ROK = true;
						// ������ ���� ���� ��� �����ϴ� �� Ȯ���Ѵ�.
						for (int down = LeftTop.y; down < RightDown.y + 1; ++down)
						{
							// ������ Ÿ���� �׸��� �ʴ´ٸ� ����������.
							int test = RightDown.x + 1 + down * m_Col;
							if (!m_vecTileInfo[test].Draw)
							{
								ROK = false;
								break;
							}
						}

						bool DOK = true;

						// �Ʒ��� ���� �Ʒ��� ��� �����ϴ� �� Ȯ���Ѵ�.
						for (int right = LeftTop.x; right < RightDown.x + 1; ++right)
						{
							// �Ʒ��� Ÿ���� �׸��� �ʴ´ٸ� ����������.
							int test = right + (RightDown.y + 1) * m_Col;
							if (!m_vecTileInfo[test].Draw)
							{
								DOK = false;
								break;
							}
						}


						if (DOK && ROK)
						{
							//������ ���� �� �� �ִ�.
							++RightDown.x;
							++RightDown.y;
						}
						//else if (DOK)
						//{
						//	++RightDown.y;
						//}
						//else if (ROK)
						//{
						//	++RightDown.x;;
						//}
						else
						{
							break;
						}

					}
					else
					{
						// ������ ���� �����Ѵٸ� 
						if (RightDown.x + 1 < m_Col && m_vecTileInfo[r].Draw && DArray[r] == 0)
						{
							// ������ �� ��� Ȯ��
							bool OK = true;
							// ������ ���� ���� ��� �����ϴ� �� Ȯ���Ѵ�.
							for (int down = LeftTop.y; down < RightDown.y + 1; ++down)
							{
								// �������� ���� �ƴϸ� ����������.
								int test = RightDown.x + 1 + down * m_Col;
								if (!m_vecTileInfo[test].Draw)
								{
									OK = false;
									break;
								}
							}

							// ��� ���̶�� 1 RightTop�� 1 ������Ų��.
							if (OK)
								++RightDown.x;
							else
								break;
						}
						// �� ���� ���� �Ѵٸ�
						else if (RightDown.y + 1 < m_Row && m_vecTileInfo[d].Draw && DArray[d] == 0)
						{
							// �� �� ��� Ȯ��
							bool OK = true;
							// ���� ���� �Ʒ��� ��� �����ϴ� �� Ȯ���Ѵ�.
							for (int right = LeftTop.x; right < RightDown.x + 1; ++right)
							{
								// ���� ���� �ƴϸ� ����������.
								int test = right + (RightDown.y + 1) * m_Col;
								if (!m_vecTileInfo[test].Draw)
								{
									OK = false;
									break;
								}
							}

							// ��� ���̶�� 1 RightTop�� 1 ������Ų��.
							if (OK)
								++RightDown.y;
							else
								break;
						}
						else
							break;
					}

				}

				GGameObject* pWall = new GGameObject(GameObject()->GetLayer());
				wstring Name = L"TileCollider_" + to_wstring(num);
				pWall->SetName(Name);
				pWall->AddComponent<GTransform>();
				pWall->AddComponent<GCollider2D>();
				Vector3 Scale = (RightDown - LeftTop) + Vector3(1.f, 1.f, 0.f);

				//Scale.x = Scale.x / m_TileSize.x;
				//Scale.y = Scale.y / m_TileSize.y;
				Scale.x = Scale.x / (float)m_Col;
				Scale.y = Scale.y / (float)m_Row;
				pWall->Collider2D()->SetScale(Scale);

				//Scale = Scale / 2;
				Vector3 Pos = (RightDown + LeftTop) / 2;
				Pos = Vector3((Pos.x - m_Col / 2) / m_Col, (m_Row / 2 - Pos.y) / m_Row, 0);
				Pos.x += (m_Col % 2 == 0) ? (1 / (float)m_Col / 2) : 0;
				Pos.y -= (m_Row % 2 == 0) ? (1 / (float)m_Row / 2) : 0;
				pWall->Transform()->SetRelativePos(Pos);

				SpawnGameObject(pWall);
				GameObject()->SetChild(pWall);
				m_ColliderObject.push_back(pWall);

				for (int col = LeftTop.x; col < RightDown.x + 1; ++col)
				{
					for (int row = LeftTop.y; row < RightDown.y + 1; ++row)
					{
						DArray[row * m_Col + col] = num;
					}
				}
				++num;
			}
		}
	}

	DArray;
}

void GTileRender::SaveToFile(FILE* _File)
{
	GRenderComponent::SaveToFile(_File);
	fwrite(&m_Collider, sizeof(bool), 1, _File);
	fwrite(&m_Col, sizeof(UINT), 1, _File);
	fwrite(&m_Row, sizeof(UINT), 1, _File);
	fwrite(&m_TileSize, sizeof(Vector2), 1, _File);

	for (size_t i = 0; i < m_Col * m_Row; ++i)
	{
		fwrite(&(m_vecTileInfo[i].Draw), sizeof(bool), 1, _File);
		fwrite(&(m_vecTileInfo[i].Palette), sizeof(TILE_PALETTE_PARAM), 1, _File);
		fwrite(&(m_vecTileInfo[i].Row), sizeof(UINT), 1, _File);
		fwrite(&(m_vecTileInfo[i].Col), sizeof(UINT), 1, _File);
	}

	for (int i = 0; i < (int)TILE_PALETTE_PARAM::END; ++i)
	{
		SaveAssetRef(m_TilePalette[i],_File);
	}
}

void GTileRender::LoadFromFile(FILE* _File)
{
	GRenderComponent::LoadFromFile(_File);
	fread(&m_Collider, sizeof(bool), 1, _File);
	fread(&m_Col, sizeof(UINT), 1, _File);
	fread(&m_Row, sizeof(UINT), 1, _File);
	fread(&m_TileSize, sizeof(Vector2), 1, _File);

	m_vecTileInfo.resize(m_Row * m_Col);
	for (size_t i = 0; i < m_Col * m_Row; ++i)
	{
		fread(&(m_vecTileInfo[i].Draw), sizeof(bool), 1, _File);
		fread(&(m_vecTileInfo[i].Palette), sizeof(TILE_PALETTE_PARAM), 1, _File);
		fread(&(m_vecTileInfo[i].Row), sizeof(UINT), 1, _File);
		fread(&(m_vecTileInfo[i].Col), sizeof(UINT), 1, _File);
	}
	SetTileSize(m_TileSize);

	for (int i = 0; i < (int)TILE_PALETTE_PARAM::END; ++i)
	{
		m_TilePalette[i] = LoadAssetRef<GTilePalette>(_File);
	}

	UpdateBuffer();
}

