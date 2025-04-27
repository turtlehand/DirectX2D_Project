#pragma once

class GLevel;

typedef GLevel* (*LEVEL_LOAD)(wstring);

class GLevelManager
	: public GSingleton<GLevelManager>
{
	SINGLE(GLevelManager);

public:
	static LEVEL_LOAD g_Load_Level;

private:
	GLevel*	m_CurLevel;
	char** m_LayerName;

public:
	GLevel* GetCurrentLevel() { return m_CurLevel; }
	LEVEL_STATE GetCurrentLevelState();

	const char** GetLayerNames();
	const char* GetLayerName(int _Layer);

	void LoadLevel(const wstring& _FilePath);
private:
	void ChangeLevel(GLevel* _NextLevel);

public:
	void Init();
	void Progress();

	friend class GTaskManager;
};

