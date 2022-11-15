#ifndef _TEST
#define _TEST

#include "register.fx"
// ========================================================
// DeadCellShader
// mesh                 : Rect
// Rasterizer           : CULL_NONE 
// BlendState           : DEFAULT
// DepthStencilState    : LESS
// Domain               : MASK

// Material Scalar Parameter
#define ColorTex    g_tex_0
#define NormalTex   g_tex_1
// ========================================================
struct VTX_IN
{   
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vWorldPos : POSITIONT;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_DeadCell(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
                   
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DeadCell(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    float4 vNormal = (float4) 0.f;
    
    vOutColor = ColorTex.Sample(g_sam_1, _in.vUV);
    
    vNormal = NormalTex.Sample(g_sam_1, _in.vUV);

    
    // NormalMap 에서 추출한 방향값을 -1 ~ 1 로 변경
    // 이유 : NormalMap은 방향벡터값이 0~2값으로 정보가 저장되어있다.
    // 그래서 계산하기 쉽게 2배를 곱하고 -1을 뺀다.
    vNormal = vNormal * 2.f - 1.f;
    
    
    
    // y 축과 z 축 반전
    // 정보 저장을 위가 z축 y가 안쪽으로 되어있어서 바꿔줘야한다.
    vNormal.xyz = float3(vNormal.x, vNormal.z, vNormal.y);
    
    // 노말 방향벡터를 월드 공간으로 옮김
    float3 vWorldNormal = normalize(mul(float4(vNormal.xyz, 0.f), g_matWorld));
    
    if(0.f == vOutColor.a)
        discard;
    
    tLightColor color = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        if (0 != g_iLight2DCount)
            continue;

        // 표면이 빛으로 부터 받는 강도(세기)
        // 빛과 표면의 사이를 내적해서 값이 0으로 수렴할수록 빛이 더 받는다.
        float fDiffusePow = saturate(dot(-g_Light2DBuffer[i].vWorldDir.xyz, vWorldNormal));
        
        // 누적 빛 = 빛의 세기 * 빛의 색상
        color.vDiff += g_Light2DBuffer[i].color.vDiff * fDiffusePow;
        color.vEmb += g_Light2DBuffer[i].color.vEmb;
    }
    
    // 최종 색 = 원래 물체의 색 * 누적된 빛
    return vOutColor * (color.vDiff + color.vEmb);

}



#endif