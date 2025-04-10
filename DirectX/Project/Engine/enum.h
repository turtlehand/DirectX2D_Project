#pragma once

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP
};

enum class LAYER_TYPE
{
	DEFAULT = 0,
	PLAYER,
	MONSTER,

	END = MAX_LAYER
};

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2

	END,
};

// Rasterizer State
enum class RS_TYPE
{
	CULL_BACK, // Default - cull_ccw 뒷면(반시계 순서) 컬링
	CULL_FRONT, // cull_cw 앞면(시계 순서) 컬링
	CULL_NONE, // 컬링하지 않음
	WIRE_FRAME, // 컬링하지 않음 - 선만 보임
	END,
};

// DepthStencilState
enum class DS_TYPE
{
	LESS,				// 깊이 판정 : 더 작아야 통과, 깊이 기록 0
	LESS_EQUAL,			// 깊이 판정 : 더 작거나 같으면 통과, 깊이 기록 있음
	GREATER,			// 깊이 판정 : 더 멀어야 통과, 깊이 기록 0

	NO_WRITE,			// 깊이 판정 : 더 작아야 통과(LESS), 깊이 기록 없음
	NO_TEST_NO_WIRTE,	// 깊이 판정 없음(항상 그림), 깊이 기록 없음

	END,

};

// Rasterizer State
enum class BS_TYPE
{
	DEFAULT,	// 소스 : 1, 원본 : 0
	ALPHABLEND,	// 소스 : A, 원본 : 1 - A
	ALPHABLEND_COVERAGE,
	ONE_ONE,
	END,
};


enum class COMPONENT_TYPE
{
	TRANSFORM,
	COLLIDER2D,
	COLLIDER3D,
	LIGHT2D,
	LIGHT3D,
	CAMERA,
	BOUNDINGBOX,
	RIGIDBODY2D,

	// Rendering Component
	MESH_RENDER,			// 가장 기본적인 RenderComponent
	SPRITE_RENDER,
	FLIPBOOK_RENDER,
	TILE_RENDER,			// 타일맵 렌더링
	PARTICLE_RENDER,		// 입자 렌더링
	SKYBOX,					// 3D 배경
	DECAL,					// 문신
	LANDSCAPE,				// 3D 지형
	END,
	SCRIPT,
	
};
extern const char* COMPONENT_TYPE_STR[];
extern const wchar_t* COMPONENT_TYPE_WSTR[];

enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	TEXTURE,
	MATERIAL,
	PREFAB,
	SOUND,
	GRAPHIC_SHADER,	//렌더링
	COMPUTE_SHADER,	// 계산, GP(Gerneral Perpose
	SPRITE,
	FLIPBOOK,
	TILEPALETTE,
	END,
};

extern const char* ASSET_TYPE_STR[];
extern const wchar_t* ASSET_TYPE_WSTR[];

// 투영 방식
enum class PROJECTION_TYPE
{
	ORTHOGRAPHIC,	// 직교투영
	PERSPECTIVE,	// 원근투영
};

enum class DIRECTION_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명
	DOMAIN_MASKED,		// 불투명 + 완전 투명
	DOMAIN_TRANSPARENT,	// 반투명
	DOMAIN_POSTPROCESS, // 후처리
	DOMAIN_DEBUG,
};

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	END
};

enum class SCALAR_PARAM
{
	INT_0,		INT_1,		INT_2,		INT_3,
	FLOAT_0,	FLOAT_1,	FLOAT_2,	FLOAT_3,
	VEC2_0,		VEC2_1,		VEC2_2,		VEC2_3,
	VEC4_0,		VEC4_1,		VEC4_2,		VEC4_3,
	MAT_0,		MAT_1,		MAT_2,		MAT_3,
};
enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	LINE,
	CUBE,
	SHAPE,
};

enum class TASK_TYPE
{
	DESTROY_OBJECT,
	CREATE_OBJECT,
	CHANGE_OBJECT_LAYER,
	ADD_CHILD,
	DESTROY_COMPONENT,
	CHANGE_LEVEL,
	CHANGE_LEVEL_STATE,
	DELETE_ASSET
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,		// 전역 (태양 달)
	POINT,				// 점광원, 전구, 전등, 횃불
	SPOT,				// 손전등, 스포트라이트
};

// 파티클에 적용할 각각의 기능들
enum class PARTICLE_MODULE
{
	SPAWN,					// 정해진 개수를 정해진 간격으로 생성
	SPAWN_BURST,			// 특정 간격마다 한꺼번에 생성
	ADD_VELOCITY,			// 입자에 속도 적용
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,

	END,
};