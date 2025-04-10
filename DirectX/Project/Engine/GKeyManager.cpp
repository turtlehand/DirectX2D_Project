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

	// ��ü ȭ�� �������� ���콺�� ��ǥ�� ȣ���Ѵ�.
	GetCursorPos(&ptPos);
	// �� ��ǥ�� â ���� ��ǥ�� ��������ش�.
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

	// �ʱ� ���콺��ǥ ����
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
			// ���� �ش� Ű�� �����ִ�.
			if (GetAsyncKeyState(KeyValue[i]) & 0x8001)
			{
				// �������� �����־���.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = KEY_STATE::PRESSED;
				}

				// ������ �������� �ʾҴ�.
				else
				{
					m_vecKeyInfo[i].State = KEY_STATE::DOWN;
				}

				m_vecKeyInfo[i].bPrevPressed = true;
			}

			// ���� �ش�Ű�� �ȴ����ִ�.
			else
			{
				// �������� �����־���.
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

		// ��ü ȭ�� �������� ���콺�� ��ǥ�� ȣ���Ѵ�.
		GetCursorPos(&ptPos);
		// �� ��ǥ�� â ���� ��ǥ�� ��������ش�.
		ScreenToClient(GEngine::GetInst()->GetMainWndHwnd(), &ptPos);

		m_MousePrePos = m_MousePos;

		m_MousePos = Vector2((float)ptPos.x, (float)ptPos.y);

		m_MouseDir = m_MousePos - m_MousePrePos;
	}
	else
	{
		// ���� Ű�� �����ǹǷ� Ű�� �� ���·� ���� �����ش�.
		for (UINT i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			// ������ ���ȴٸ� Ű�� �� ���·� ����
			if (m_vecKeyInfo[i].bPrevPressed)
			{
				m_vecKeyInfo[i].State = KEY_STATE::UP;
				m_vecKeyInfo[i].bPrevPressed = false;
			}
			// Ű�� �� ���¶�� �������� ��
			else if (m_vecKeyInfo[i].State == KEY_STATE::UP)
			{
				m_vecKeyInfo[i].State = KEY_STATE::NONE;
			}
		}

		m_MousePos = Vector2(NAN, NAN);
	}


}

/// <summary>
/// �⺻ ���¸� ���� �� �۵���
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