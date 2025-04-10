#include "pch.h"
#include "InspectorUI.h"

#include "ui_component.h"
#include "ui_asset.h"

#include "AddComponentUI.h"

void InspectorUI::CreateComponentUI()
{
	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSizeAsChild(ImVec2(0.f, 130.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSizeAsChild(ImVec2(0.f, 300.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSizeAsChild(ImVec2(0.f, 300.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSizeAsChild(ImVec2(0.f, 300.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D] = new RigidBody2DUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSizeAsChild(ImVec2(0.f, 300.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER] = new MeshRenderUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSizeAsChild(ImVec2(0.f, 0.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::SPRITE_RENDER] = new SpriteRenderUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSizeAsChild(ImVec2(0.f, 0.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::SPRITE_RENDER]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::FLIPBOOK_RENDER] = new FlipbookRenderUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSizeAsChild(ImVec2(0.f, 0.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::FLIPBOOK_RENDER]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::TILE_RENDER] = new TileRenderUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSizeAsChild(ImVec2(0.f, 0.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::TILE_RENDER]);

	// 切縦 UI 持失
	m_ComUI[(UINT)COMPONENT_TYPE::PARTICLE_RENDER] = new ParticleRenderUI;
	//m_ComUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSizeAsChild(ImVec2(0.f, 0.f));
	AddChildUI(m_ComUI[(UINT)COMPONENT_TYPE::PARTICLE_RENDER]);

	// 切縦 UI 持失
	m_AddComponentUI = new AddComponentUI;
	m_AddComponentUI->SetSizeAsChild(ImVec2(0.f, 30.f));
	AddChildUI(m_AddComponentUI);
}

void InspectorUI::CreateAssetUI()
{
	m_AssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	m_AssetUI[(UINT)ASSET_TYPE::MESH]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::MESH]);

	m_AssetUI[(UINT)ASSET_TYPE::MESHDATA] = new MeshDataUI;
	m_AssetUI[(UINT)ASSET_TYPE::MESHDATA]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::MESHDATA]);

	m_AssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	m_AssetUI[(UINT)ASSET_TYPE::TEXTURE]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::TEXTURE]);

	m_AssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	m_AssetUI[(UINT)ASSET_TYPE::MATERIAL]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::MATERIAL]);

	m_AssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	m_AssetUI[(UINT)ASSET_TYPE::PREFAB]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::PREFAB]);

	m_AssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	m_AssetUI[(UINT)ASSET_TYPE::SOUND]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::SOUND]);

	m_AssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = new GraphicShaderUI;
	m_AssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER]);

	m_AssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
	m_AssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER]);

	m_AssetUI[(UINT)ASSET_TYPE::SPRITE] = new SpriteUI;
	m_AssetUI[(UINT)ASSET_TYPE::SPRITE]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::SPRITE]);

	m_AssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = new FlipbookUI;
	m_AssetUI[(UINT)ASSET_TYPE::FLIPBOOK]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::FLIPBOOK]);

	m_AssetUI[(UINT)ASSET_TYPE::TILEPALETTE] = new TilePaletteUI;
	m_AssetUI[(UINT)ASSET_TYPE::TILEPALETTE]->SetActive(false);
	AddChildUI(m_AssetUI[(UINT)ASSET_TYPE::TILEPALETTE]);
}