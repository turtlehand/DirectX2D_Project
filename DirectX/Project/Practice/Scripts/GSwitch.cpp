#include "pch.h"
#include "GSwitch.h"

#include <Engine/GAssetManager.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/GSprite.h>

#include <Engine/components.h>

#include "GInteractable.h"

GSwitch::GSwitch()
	:GScript(SWITCH)
{
}

GSwitch::GSwitch(const GSwitch& _Origin)
	: GScript(SWITCH)
	, m_IsPressed(false)
	, m_ButtonSprite(_Origin.m_ButtonSprite)
	, m_PressedSprite(_Origin.m_PressedSprite)
	, m_ObjectName()
	, m_SucessFindObject()
	, m_Object()
{
	for (int i = 0; i < 3; ++i)
	{
		m_ObjectName[i] = _Origin.m_ObjectName[i];
		m_SucessFindObject[i] = false;
		m_Object[i] = nullptr;
	}
}

GSwitch::~GSwitch()
{
}

void GSwitch::Init()
{
	ADD_SPRITE("Button Sprite", &m_ButtonSprite);
	ADD_SPRITE("Pressed Sprite", &m_PressedSprite);

	ADD_BOOL("Pressed", &m_IsPressed);

	ADD_STRING("0 Object", m_ObjectName);
	ADD_BOOL("0 Find Object" ,&m_SucessFindObject);

	ADD_STRING("1 Object",(m_ObjectName + 1));
	ADD_BOOL("1 Find Object", (m_SucessFindObject+1));

	ADD_STRING("2 Object", (m_ObjectName + 2));
	ADD_BOOL("2 Find Object", (m_SucessFindObject + 2));
}

void GSwitch::Begin()
{
	if(m_PressedSprite == nullptr)
		m_PressedSprite = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\ButtonPressed_0000.sprite");
	
	if(m_ButtonSprite == nullptr)
		m_ButtonSprite = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Button_0000.sprite");

	for (int i = 0; i < 3; ++i)
	{
		if (m_ObjectName[i] == "")
			continue;

		m_Object[i] = GLevelManager::GetInst()->GetCurrentLevel()->FindObjectByName(ToWString( m_ObjectName[i]));
		GInteractable* Interactable = m_Object[i]->GetComponent<GInteractable>();
		assert(Interactable);

		m_SucessFindObject[i] = true;
	}

}

void GSwitch::Update()
{

}

void GSwitch::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		++m_IsPressed;
		if (m_IsPressed)
		{
			SpriteRender()->SetSprite(m_PressedSprite);
		}
		for (int i = 0; i < 3; ++i)
		{
			if (!m_SucessFindObject[i])
				continue;

			GInteractable* Interactable = m_Object[i]->GetComponent<GInteractable>();
			Interactable->InteractEnter();
		}

	}
}

void GSwitch::OnOverlapExit(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		--m_IsPressed;
		if (m_IsPressed == 0)
		{
			SpriteRender()->SetSprite(m_ButtonSprite);
		}
		for (int i = 0; i < 3; ++i)
		{
			if (!m_SucessFindObject[i])
				continue;

			GInteractable* Interactable = m_Object[i]->GetComponent<GInteractable>();
			Interactable->InteractExit();
		}
	}
}

void GSwitch::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_ButtonSprite, _File);
	SaveAssetRef(m_PressedSprite, _File);

	for (int i = 0; i < 3; ++i)
	{
		wstring temp = ToWString(m_ObjectName[i]);
		SaveWString(temp, _File);
	}

}

void GSwitch::LoadFromFile(FILE* _File)
{
	m_ButtonSprite = LoadAssetRef<GSprite>(_File);
	m_PressedSprite = LoadAssetRef<GSprite>(_File);

	for (int i = 0; i < 3; ++i)
	{
		wstring temp;
		LoadWString(temp, _File);
		m_ObjectName[i] = ToString(temp);
		
	}
}


