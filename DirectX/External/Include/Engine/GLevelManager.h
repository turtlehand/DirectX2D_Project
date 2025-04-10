#pragma once

class GLevelManager
	: public GSingleton<GLevelManager>
{
	SINGLE(GLevelManager);
private:
	class GLevel*	m_CurLevel;
	char** m_LayerName;

public:
	GLevel* GetCurrentLevel() { return m_CurLevel; }
	LEVEL_STATE GetCurrentLevelState();

	const char** GetLayerNames();
	const char* GetLayerName(int _Layer);


private:
	void ChangeLevel(GLevel* _NextLevel);

public:
	void Init();
	void Progress();

	friend class GTaskManager;
};

