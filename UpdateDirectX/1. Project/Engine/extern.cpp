#include "pch.h"

#include "global.h"

tTransform g_transform = {};
tGlobalData g_global = {};

const Vector3 Vector3::Zero = Vector3(0.f, 0.f, 0.f);


extern const char* COMPONENT_TYPE_CHAR[(UINT)COMPONENT_TYPE::END] =
{
	"TRANSFORM",
	"CAMERA",
	"COLLIDER2D",
	"COLLIDER3D",
	"ANIMATOR2D",
	"ANIMATOR3D",
	"LIGHT2D",
	"LIGHT3D",	
	"RIGIDBODY2D",
	"MESHRENDER",
	"TILEMAP",
	"PARTICLESYSTEM",
	"SKYBOX",
	"DECAL",
	"LANDSCAPE",
	"AI"
};

extern const wchar_t* COMPONENT_TYPE_WCHAR[(UINT)COMPONENT_TYPE::END] =
{
	L"TRANSFORM",
	L"CAMERA",
	L"COLLIDER2D",
	L"COLLIDER3D",
	L"ANIMATOR2D",
	L"ANIMATOR3D",
	L"LIGHT2D",
	L"LIGHT3D",
	L"RIGIDBODY2D",
	L"MESHRENDER",
	L"TILEMAP",
	L"PARTICLESYSTEM",
	L"SKYBOX",
	L"DECAL",
	L"LANDSCAPE",
	L"AI"
};


extern const char* RES_TYPE_CHAR[(UINT)RES_TYPE::END] =
{
	"PREFAB",
	"MESHDATA",
	"COMPUTE_SHADER",
	"MATERIAL",
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
};

extern const wchar_t* RES_TYPE_WCHAR[(UINT)RES_TYPE::END] =
{
	L"PREFAB",
	L"MESHDATA",
	L"COMPUTE_SHADER",
	L"MATERIAL",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"GRAPHICS_SHADER",
};