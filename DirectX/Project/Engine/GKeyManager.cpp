#include "pch.h"
#include "GKeyManager.h"

#include "GEngine.h"

#include "GLevelManager.h"
#include "GRenderManager.h"
#include "GGameObject.h"
#include "GCamera.h"
#include "GTransform.h"

UINT KeyValue[(UINT)KEY::KEY_END] =
{
	'Q', 'W', 'E', 'R', 'T', 'Y',
	'A', 'S', 'D', 'F', 'G', 'H',
	'Z', 'X', 'C',  'V',  'B',  'N', 'M',

	VK_RETURN,
	VK_ESCAPE,
	VK_LSHIFT,
	VK_MENU,
	VK_CONTROL,
	VK_SPACE,

	VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

	VK_OEM_MINUS,
	VK_OEM_PLUS,
	VK_OEM_2,

	VK_LBUTTON,
	VK_RBUTTON,
};

GKeyManager::GKeyManager()
{
	POINT ptPos = {};

	// 전체 화면 기준으로 마우스의 좌표를 호출한다.
	GetCursorPos(&ptPos);
	// 그 좌표를 창 기준 좌표로 변경시켜준다.
	ScreenToClient(GEngine::GetInst()->GetMainWndHwnd(), &ptPos);

	m_MousePos = Vector2((float)ptPos.x, (float)ptPos.y);
}

GKeyManager::~GKeyManager()
{

}

void GKeyManager::Init()
{
	for (UINT i = 0; i < (int)KEY::KEY_END; ++i)
	{
		m_vecKeyInfo.push_back(KeyInfo{ KEY_STATE::NONE, false });
	}

	// 초기 마우스좌표 세팅
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(GEngine::GetInst()->GetMainWndHwnd(), &ptPos);

	m_MousePos = Vector2((float)ptPos.x, (float)ptPos.y);
	m_MousePrePos = m_MousePos;
}

void GKeyManager::Progress()
{
	if (GetFocus() == GEngine::GetInst()->GetMainWndHwnd())
	{
		for (size_t i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			// 현재 해당 키가 눌려있다.
			if (GetAsyncKeyState(KeyValue[i]) & 0x8001)
			{
				// 이전에도 눌려있었다.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = KEY_STATE::PRESSED;
				}

				// 이전에 눌려있지 않았다.
				else
				{
					m_vecKeyInfo[i].State = KEY_STATE::DOWN;
				}

				m_vecKeyInfo[i].bPrevPressed = true;
			}

			// 현재 해당키가 안눌려있다.
			else
			{
				// 이전에는 눌려있었다.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = KEY_STATE::UP;
				}

				else
				{
					m_vecKeyInfo[i].State = KEY_STATE::NONE;
				}

				m_vecKeyInfo[i].bPrevPressed = false;
			}
		}

		

		POINT ptPos = {};

		// 전체 화면 기준으로 마우스의 좌표를 호출한다.
		GetCursorPos(&ptPos);
		// 그 좌표를 창 기준 좌표로 변경시켜준다.
		ScreenToClient(GEngine::GetInst()->GetMainWndHwnd(), &ptPos);

		m_MousePrePos = m_MousePos;

		m_MousePos = Vector2((float)ptPos.x, (float)ptPos.y);

		m_MouseDir = m_MousePos - m_MousePrePos;
	}
	else
	{
		// 기존 키가 유지되므로 키를 뗀 상태로 변경 시켜준다.
		for (UINT i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			// 이전에 눌렸다면 키를 뗀 상태로 변경
			if (m_vecKeyInfo[i].bPrevPressed)
			{
				m_vecKeyInfo[i].State = KEY_STATE::UP;
				m_vecKeyInfo[i].bPrevPressed = false;
			}
			// 키를 뗀 상태라면 해제시켜 줌
			else if (m_vecKeyInfo[i].State == KEY_STATE::UP)
			{
				m_vecKeyInfo[i].State = KEY_STATE::NONE;
			}
		}

		m_MousePos = Vector2(NAN, NAN);
	}


}

/// <summary>
/// 기본 상태만 현재 잘 작동함
/// </summary>
/// <returns></returns>
Vector2 GKeyManager::GetGameMousePos()
{
	Vector2 GameMousePos = m_MousePos;
	Vector2 Resolution = GEngine::GetInst()->GetResolution();
	
	if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
	{
		GCamera* pEditorCam = GRenderManager::GetInst()->GetEditorCamera();
		float CamRatio = Resolution.x / pEditorCam->GetOrthoScaleX();
		//Vector2 CameraResoution = Vector2(pEditorCam->GetOrthoScaleX(), pEditorCam->GetOrthoScaleX() / pEditorCam->GetAspectRatio());
		GameMousePos = Vector2((GameMousePos.x - Resolution.x / 2)
			, (Resolution.y / 2 - GameMousePos.y)) / CamRatio;
		Matrix matTranslation = XMMatrixTranslation(GameMousePos.x, GameMousePos.y, 0.f);
		Matrix matCamW = pEditorCam->Transform()->GetWorldMat();
		Matrix matMousePos = matTranslation * matCamW;
		GameMousePos = Vector2(matMousePos._41, matMousePos._42);
	}

	return GameMousePos;
}

bool GKeyManager::IsMouseOffScreen()
{
	Vector2 Resolution = GEngine::GetInst()->GetResolution();

	if (Resolution < m_MousePos)
		return false;

	return true;
}