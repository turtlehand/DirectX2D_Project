#pragma once

#include <Windows.h>
#include <typeinfo>
#include <filesystem>
using namespace std::filesystem;

// ComPtr
#include<wrl/client.h>
using namespace Microsoft::WRL;

// DirectX 라이브러리 링크
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// DirectTex 라이버러리 링크
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>


#ifndef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex_d.lib")
#endif // _DEBUG

// FMOD
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

#include <vector>
#include <list>
#include <stack>
#include <map>
#include <string>
#include <algorithm>
#include <format>
using namespace std;

#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"

#include "GSingleton.h"
#include "Ptr.h"