#pragma once

class GLevelSaveLoad
{
public:
	static void SaveLevel(wstring _FilePath);
	static class GLevel* LoadLevel(wstring _FilePath);
	static void SaveGameObject(GGameObject* _Object, FILE* _File);
	static class GGameObject* LoadGameObject(FILE* _File);

private:
	static void SaveProjectSetting(FILE* _File);
	static void LoadProjectSetting(FILE* _File);

private:
	GLevelSaveLoad() {}
	~GLevelSaveLoad() {}
};

