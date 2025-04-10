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
	CULL_BACK, // Default - cull_ccw �޸�(�ݽð� ����) �ø�
	CULL_FRONT, // cull_cw �ո�(�ð� ����) �ø�
	CULL_NONE, // �ø����� ����
	WIRE_FRAME, // �ø����� ���� - ���� ����
	END,
};

// DepthStencilState
enum class DS_TYPE
{
	LESS,				// ���� ���� : �� �۾ƾ� ���, ���� ��� 0
	LESS_EQUAL,			// ���� ���� : �� �۰ų� ������ ���, ���� ��� ����
	GREATER,			// ���� ���� : �� �־�� ���, ���� ��� 0

	NO_WRITE,			// ���� ���� : �� �۾ƾ� ���(LESS), ���� ��� ����
	NO_TEST_NO_WIRTE,	// ���� ���� ����(�׻� �׸�), ���� ��� ����

	END,

};

// Rasterizer State
enum class BS_TYPE
{
	DEFAULT,	// �ҽ� : 1, ���� : 0
	ALPHABLEND,	// �ҽ� : A, ���� : 1 - A
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
	MESH_RENDER,			// ���� �⺻���� RenderComponent
	SPRITE_RENDER,
	FLIPBOOK_RENDER,
	TILE_RENDER,			// Ÿ�ϸ� ������
	PARTICLE_RENDER,		// ���� ������
	SKYBOX,					// 3D ���
	DECAL,					// ����
	LANDSCAPE,				// 3D ����
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
	GRAPHIC_SHADER,	//������
	COMPUTE_SHADER,	// ���, GP(Gerneral Perpose
	SPRITE,
	FLIPBOOK,
	TILEPALETTE,
	END,
};

extern const char* ASSET_TYPE_STR[];
extern const wchar_t* ASSET_TYPE_WSTR[];

// ���� ���
enum class PROJECTION_TYPE
{
	ORTHOGRAPHIC,	// ��������
	PERSPECTIVE,	// ��������
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
	DOMAIN_OPAQUE,		// ������
	DOMAIN_MASKED,		// ������ + ���� ����
	DOMAIN_TRANSPARENT,	// ������
	DOMAIN_POSTPROCESS, // ��ó��
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
	DIRECTIONAL,		// ���� (�¾� ��)
	POINT,				// ������, ����, ����, ȶ��
	SPOT,				// ������, ����Ʈ����Ʈ
};

// ��ƼŬ�� ������ ������ ��ɵ�
enum class PARTICLE_MODULE
{
	SPAWN,					// ������ ������ ������ �������� ����
	SPAWN_BURST,			// Ư�� ���ݸ��� �Ѳ����� ����
	ADD_VELOCITY,			// ���ڿ� �ӵ� ����
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,

	END,
};