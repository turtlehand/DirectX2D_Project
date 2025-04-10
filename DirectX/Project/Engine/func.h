#pragma once

void DrawDebugRect(Vector4 _Color, Vector3 _WorldPos, Vector3 _WorldScale, Vector3 _WorldRotation, bool _DepthTest = false, float _Duration = 0.f);
void DrawDebugRect(Vector4 _Color, Matrix _WorldMat, bool _DepthTest = false, float _Duration = 0.f);
void DrawDebugCircle(Vector4 _Color, Vector3 _WorldPos, float _Radius, Vector3 _WorldRotation, bool _DepthTest = false, float _Duration = 0.f);
void DrawDebugLine(Vector4 _Color, Vector3 _WorldPos, Vector3 _Dir, float _Distance, bool _DepthTest = false, float _Duration = 0.f);

template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		delete iter->second;
	}

	_map.clear();


}

class GTransform;
class GMeshRender;
class GSpriteRender;
class GFlipbookRender;
class GTileRender;
class GParticleRender;
class GCamera;
class GCollider2D;
class GLight2D;
class GRigidBody2D;

template<typename T>
COMPONENT_TYPE GetComponentType()
{
	// RTTI (RunTime Type Information / Identification)
	if constexpr (is_same_v<GTransform, T>)
		return COMPONENT_TYPE::TRANSFORM;
	else if constexpr (is_same_v<GMeshRender, T>)
		return COMPONENT_TYPE::MESH_RENDER;
	else if constexpr (is_same_v<GSpriteRender, T>)
		return COMPONENT_TYPE::SPRITE_RENDER;
	else if constexpr (is_same_v<GFlipbookRender, T>)
		return COMPONENT_TYPE::FLIPBOOK_RENDER;
	else if constexpr (is_same_v<GTileRender, T>)
		return COMPONENT_TYPE::TILE_RENDER;
	else if constexpr (is_same_v<GParticleRender, T>)
		return COMPONENT_TYPE::PARTICLE_RENDER;
	else if constexpr (is_same_v<GCamera, T>)
		return COMPONENT_TYPE::CAMERA;
	else if constexpr (is_same_v<GCollider2D, T>)
		return COMPONENT_TYPE::COLLIDER2D;
	else if constexpr (is_same_v<GLight2D, T>)
		return COMPONENT_TYPE::LIGHT2D;
	else if constexpr (is_same_v<GRigidBody2D, T>)
		return COMPONENT_TYPE::RIGIDBODY2D;
	else
		return COMPONENT_TYPE::SCRIPT;

}

class GGameObject* SpawnGameObject(GGameObject* _GameObject);

string ToString(const wstring& _String);

wstring ToWString(const string& _String);

void SaveWString(const wstring& _Str, FILE* _File);
void LoadWString(wstring& _Str, FILE* _File);

bool IsValid(GGameObject*& _GameObject);

void ChangeGameObjectLayer(GGameObject* _GameObject, UINT _Layer);