#ifndef _STD2D
#define _STD2D

#include "register.fx"
#include "func.fx"

// HLSL 로 VertexShader 작성하기
struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);

        if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }

        vOutColor = g_Atals.Sample(g_sam_1, vUV);
    }
    else
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    if (0.f == vOutColor.a)
        discard;

    // 광원 처리
    tLightColor color = (tLightColor) 0.f;
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vWorldPos, i);
    }

    vOutColor *= color.vDiff;

    return vOutColor;
}

float4 PS_Std2D_AlphaBlend(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    return vOutColor;
}





#endif