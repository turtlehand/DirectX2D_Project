#pragma once
#include "GAsset.h"

#include "GGraphicShader.h"
#include "GTexture.h"

class GMaterial :
	public GAsset
{
private:
	Ptr<GGraphicShader> m_Shader;
	Ptr<GTexture>		m_arrTex[(UINT)TEX_PARAM::END];
	tMtrlConst          m_Const;

public:
	void SetShader(Ptr<GGraphicShader> _Shader) { m_Shader = _Shader; }
	Ptr<GGraphicShader> GetShader() { return m_Shader; }

	void SetTexture(TEX_PARAM _Param, const Ptr<GTexture>& _Texture) { assert(_Param != TEX_PARAM::END); m_arrTex[(UINT)_Param] = _Texture; }
	Ptr<GTexture> GetTexture(TEX_PARAM _Param) { return m_arrTex[(UINT)_Param]; }

	template<typename T>
	void SetScalarParam(SCALAR_PARAM _Param, const T& _Data);

	template<typename T>
	T& GetScalarParam(SCALAR_PARAM _Param);

	void Binding();

	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;

public:
	CLONE(GMaterial);

public:
	GMaterial(bool _EngineAsset = false);
	~GMaterial();
};

template<typename T>
void GMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Data)
{
	bool bInput = false;

	switch (_Param)
	{
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
	{
		if constexpr (is_same_v<int, T>)
		{
			m_Const.iArr[(int)_Param - (int)SCALAR_PARAM::INT_0] = _Data;
			bInput = true;
		}
		else if constexpr (is_same_v<UINT, T>)
		{
			m_Const.iArr[(int)_Param - (int)SCALAR_PARAM::INT_0] = _Data;
			bInput = true;
		}
		break;
	}
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
	{
		if constexpr (is_same_v<float, T>)
		{
			m_Const.fArr[(int)_Param - (int)SCALAR_PARAM::FLOAT_0] = _Data;
			bInput = true;
		}
		break;
	}
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
	{
		if constexpr (is_same_v<Vector2, T>)
		{
			m_Const.v2Arr[(int)_Param - (int)SCALAR_PARAM::VEC2_0] = _Data;
			bInput = true;
		}
		break;
	}
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
	{
		if constexpr (is_same_v<Vector4, T> || is_same_v<Vector3, T>)
		{
			m_Const.v4Arr[(int)_Param - (int)SCALAR_PARAM::VEC4_0] = _Data;
			bInput = true;
		}
		break;
	}
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
	{
		if constexpr (is_same_v<Matrix, T>)
		{
			m_Const.mArr[(int)_Param - (int)SCALAR_PARAM::MAT_0] = _Data;
			bInput = true;
		}
		break;
	}
	}

	assert(bInput);

	
}

template<typename T>
inline T& GMaterial::GetScalarParam(SCALAR_PARAM _Param)
{
	bool bInput = false;

	switch (_Param)
	{
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
	{
		if constexpr (is_same_v<int, T> || is_same_v<UINT, T>)
		{
			bInput = true;
			return m_Const.iArr[(int)_Param - (int)SCALAR_PARAM::INT_0];
		}
		break;
	}
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
	{
		if constexpr (is_same_v<float, T>)
		{
			bInput = true;
			return m_Const.fArr[(int)_Param - (int)SCALAR_PARAM::FLOAT_0];
		}
		break;
	}
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
	{
		if constexpr (is_same_v<Vector2, T>)
		{
			bInput = true;
			return m_Const.v2Arr[(int)_Param - (int)SCALAR_PARAM::VEC2_0];
		}
		break;
	}
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
	{
		if constexpr (is_same_v<Vector4, T> || is_same_v<Vector3, T>)
		{
			bInput = true;
			return m_Const.v4Arr[(int)_Param - (int)SCALAR_PARAM::VEC4_0];
		}
		break;
	}
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
	{
		if constexpr (is_same_v<Matrix, T>)
		{
			bInput = true;
			return m_Const.mArr[(int)_Param - (int)SCALAR_PARAM::MAT_0];
		}
		break;
	}
	}

	assert(bInput);



}
