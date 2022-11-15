#ifndef _REGISTER
#define _REGISTER

#include "struct.fx"

// 상수 레지스터
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;

    row_major matrix g_matWV;
    row_major matrix g_matWVP;
};


cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;

    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
};

cbuffer GLOBAL : register(b2)
{
    float2  g_vRenderResolution;
    int     g_iLight2DCount;
    int     g_iLight3DCount;

    float   g_fAccTime;
    float   g_fDT;
    int2    pad;
}

cbuffer ANIM2DINFO : register(b3)
{
    float2  g_vLeftTop;
    float2  g_vSlice;
    float2  g_vOffset;
    float2  g_vFullSize;

    int     g_iAnim2DUse;
    int3    iPadding;
}


// 텍스쳐 레지스터
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

TextureCube g_texcube_0 : register(t8);
TextureCube g_texcube_1 : register(t9);

Texture2DArray g_texarr_0 : register(t10);
Texture2DArray g_texarr_1 : register(t11);

// Animation2D Atlas
Texture2D g_Atals : register(t12);

// Light Buffer
StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t13);
StructuredBuffer<tLightInfo> g_Light3DBuffer : register(t14);

//Noise Texture





// 샘플러 레지스터
SamplerState g_sam_0 : register(s0); // anisotropic sampler
SamplerState g_sam_1 : register(s1); // point sampler


#endif

