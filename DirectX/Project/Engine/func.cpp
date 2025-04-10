#include "pch.h"

#include "GRenderManager.h"
#include "GTaskManager.h"
#include "GGameObject.h"


void DrawDebugRect(Vector4 _Color, Vector3 _WorldPos, Vector3 _WorldScale, Vector3 _WorldRotation, bool _DepthTest, float _Duration)
{
	// Debug Test
	tDebugShapeInfo tDR = {};

	tDR.Shape = DEBUG_SHAPE::RECT;
	tDR.WorldPos = _WorldPos;
	tDR.WorldScale = _WorldScale;
	tDR.WorldRotation = _WorldRotation;
	//tDR.matWorld = Transform()->GetWorldMat();
	tDR.Color = _Color;
	tDR.Duration = _Duration;
	tDR.DepthTest = _DepthTest;

	GRenderManager::GetInst()->AddDebugShapeInfo(tDR);
}

void DrawDebugRect(Vector4 _Color, Matrix _WorldMat, bool _DepthTest, float _Duration)
{
	// Debug Test
	tDebugShapeInfo tDR = {};

	tDR.Shape = DEBUG_SHAPE::RECT;
	//tDR.WorldPos = _WorldPos;
	//tDR.WorldScale = _WorldScale;
	//tDR.WorldRotation = _WorldRotation;
	tDR.MatWorld = _WorldMat;
	tDR.Color = _Color;
	tDR.Duration = _Duration;
	tDR.DepthTest = _DepthTest;

	GRenderManager::GetInst()->AddDebugShapeInfo(tDR);
}

void DrawDebugCircle(Vector4 _Color, Vector3 _WorldPos, float _Radius, Vector3 _WorldRotation, bool _DepthTest, float _Duration)
{
	// Debug Test
	tDebugShapeInfo tDR = {};

	tDR.Shape = DEBUG_SHAPE::CIRCLE;
	tDR.WorldPos = _WorldPos;
	tDR.WorldScale = Vector3(_Radius * 2,_Radius * 2, 0.f);
	tDR.WorldRotation = _WorldRotation;
	//tDR.matWorld = Transform()->GetWorldMat();
	tDR.Color = _Color;
	tDR.Duration = _Duration;
	tDR.DepthTest = _DepthTest;

	GRenderManager::GetInst()->AddDebugShapeInfo(tDR);
}

void DrawDebugLine(Vector4 _Color, Vector3 _WorldPos, Vector3 _Dir, float _Distance, bool _DepthTest, float _Duration)
{
	// Debug Test
	tDebugShapeInfo tDR = {};

	tDR.Shape = DEBUG_SHAPE::LINE;
	tDR.WorldPos = _WorldPos;
	tDR.WorldScale = Vector3(1.f, 1.f, 1.f);
	tDR.WorldRotation = Vector3(0.f, 0.f, 0.f);
	tDR.WorldDirction = Vector4(_Dir.Normalize(),0.f) * _Distance;
	tDR.Color = _Color;
	tDR.Duration = _Duration;
	tDR.DepthTest = _DepthTest;

	GRenderManager::GetInst()->AddDebugShapeInfo(tDR);
}

GGameObject* SpawnGameObject(GGameObject* _GameObject)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param0 = (DWORD_PTR)_GameObject;
	task.Param1 = (DWORD_PTR)_GameObject->GetLayer();

	GTaskManager::GetInst()->AddTask(task);

	return _GameObject;
}

string ToString(const wstring& _String)
{
	return string(_String.begin(), _String.end());
}

wstring ToWString(const string& _String)
{
	return wstring(_String.begin(), _String.end());
}

void SaveWString(const wstring& _Str, FILE* _File)
{
	size_t len = _Str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_Str.c_str(), sizeof(wchar_t), len, _File);
}

void LoadWString(wstring& _Str, FILE* _File)
{
	wchar_t Buff[255] = {};
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);
	fread(Buff, sizeof(wchar_t), len, _File);
	_Str = Buff;
}

bool IsValid(GGameObject*& _GameObject)
{
	if (_GameObject == nullptr)
	{
		_GameObject = nullptr;
		return false;
	}
	if (_GameObject->IsDead())
	{
		_GameObject = nullptr;
		return false;
	}

	return true;
}

void ChangeGameObjectLayer(GGameObject* _GameObject, UINT _Layer)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_OBJECT_LAYER;
	task.Param0 = (DWORD_PTR)_GameObject;
	task.Param1 = (DWORD_PTR)_Layer;

	GTaskManager::GetInst()->AddTask(task);
}


