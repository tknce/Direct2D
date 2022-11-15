#ifndef _EDITOR
#define _EDITOR

#include "register.fx"

// ================
// Grid Shader
// mesh : RectMesh
#define RenderResolution    g_vec2_0
#define CamerWorldPos       g_vec4_0.xy
#define CameraScale         g_float_0
#define Thickness           g_float_1
#define GridInterval        g_float_2    
#define GridColor           g_vec4_1
// ================


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;

    float2 vWorldPos : POSITION;
};


VTX_OUT VS_Grid(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    float2 vWorldPos = _in.vPos.xy * g_vec2_0 * g_float_0 + g_vec4_0.xy;


    output.vPos = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;

    output.vWorldPos = vWorldPos;

    return output;
}

float4 PS_Grid(VTX_OUT _in) : SV_Target
{
    // 100
    // +-1

    float fThick = Thickness * CameraScale;
    
    uint iWorldX = (int)_in.vWorldPos.x;
    uint iRestX = (iWorldX + (uint) fThick / 2) % (uint) GridInterval;
    
    uint iWorldY = (int)_in.vWorldPos.y;
    uint iRestY = (iWorldY + (uint) fThick / 2) % (uint) GridInterval;
    
    
    if (0 <= iRestX && iRestX <= (fThick / 2) * 2
        || 0 <= iRestY && iRestY <= (fThick / 2) * 2)
    {
        return GridColor;
    }
    
    discard;
    
    return GridColor;
}





#endif