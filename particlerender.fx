#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "register.fx"

// ======================
// Particle Render Shader
// RS_TYPE : CULL_NONE
// BS_TYPE : ALPHABLEND
// DS_TYPE : NO_WRITE
// DOMAIN : Transparent
StructuredBuffer<tParticle> ParticleBuffer : register(t15);
#define ParticleIndex g_int_0
// ======================

struct VS_IN
{
    float3 vPos : POSITIONT;
    // instance는 같은 랜더이지만 무수히 많은 랜더일 경우 파이프라인을 통해 요청하지 않고
    // 그래픽카드내에서 처리하게 하는데 몇개를 처리해야될지 보내는 변수
    uint iInstance : SV_InstanceID; 
    
};

struct VS_OUT
{
    // WorldPosition 정보 계산
    float3 vLocalPos : POSITION;
    uint iInstance : SV_InstanceID;
    
};
// VS의 단계에서 할일 - 파티클에선 그저 정점정보를 넘기는 것 뿐이다. GS에서 정점을 rectmesh로 확장시켜서 
// 그리기떄문
VS_OUT VS_ParticleRender(VS_IN _in)
{
    // VS_OUT 의 포지션 정보 생성 VS단계에서 최적화된 형태로 정점정보를 리턴
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vLocalPos = _in.vPos;
    output.iInstance = _in.iInstance;
    
    
    return output;
}

struct GS_OUT
{
    // 포지션 정보
    // SV_Position - 오프셋이 0.5인 정점좌표를 설명
    float4 vPosition : SV_Position;
    // 보간된 UV값  
    // TEXCOORD - 텍스쳐 좌표
    float2 vUV : TEXCOORD;
};

[maxvervexcount(6)]
void GS_ParticleRender(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    // 정점 정보 생성
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    // 죽어 있으면 리턴
    // PS에 까지 가면 모든 보간 작업하고 그려진 뒤에 discard가 되버린다 하지만 여기서 리턴 시키면 아무런
    // 작업을 하지 않은채 리턴된다.
    if (0 == ParticleBuffer[_in[0].iInstance].iActive)
        return;

    // NDC 좌표계에서 상태좌표를 더해준다.
    float3 vWorldPos = _in[0].vLocalPos + ParticleBuffer[_in[0].iInstance].vRelaticePos.xyz;
    // 객체의 worldpos를 뷰행렬로 곱해주어 카메라 앞으로 배치되게 한다.
    float3 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    // 옮겨졌으나 4방향으로 커져봤다 1픽셀이다 그래서 정점 좌표 위치를 옮겨준다.
    float3 NewPos[4] =
    {
        vViewPos - float3(-0.5f, 0.5f, 0.f) * float3(50.f, 50.f, 1.f),
        vViewPos - float3(0.5f, 0.5f, 0.f) * float3(50.f, 50.f, 1.f),
        vViewPos - float3(0.5f, -0.5f, 0.f) * float3(50.f, 50.f, 1.f),
        vViewPos - float3(-0.5f, -0.5f, 0.f) * float3(50.f, 50.f, 1.f)
    };
    
    // viewpos까지 구해졌으면 다시 투영행렬을 곱해준다.
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(NewPos[i], 1.f), g_matProj);
    }
    
    //UV값을 설정해준다. UV는 중심에서 시작하는 것이 아니라서 -가 없다
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
       
    
    // Rect Mesh 생성했던것처럼 사각형 메쉬 생성
    // 0 -- 1
    // | \  |
    // 3 -- 2
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();

}



float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    return vColor;
}

    #endif