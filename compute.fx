#ifndef _COMPUTE
#define _COMPUTE

// 읽기쓰기가능 텍스쳐 Format float4  computeshader는 u 라인에 바인딩 걸어야된다.
RWTexture2D<float4> tex : register(u0);

// SV_GroupID                   : 스레드가 속한 그룹의 좌표
// SV_GroupThreadID             : 그룹 내에서, 스레드의 좌표
// SV_GroupIndex                : 그룹 내에서, 스레드의 인덱스 (1차원)
// SV_DispatchThreadID          : 전체 스레드(모든 그룹 통합) 기준으로, 호출된 스레드의 좌표

[numthreads(32,32,1)] // 그룹 당 스레드 개수 (최대 1024 까지 지정 가능)
void CS_Paint(uint3 _id : SV_DispatchThreadID)
{
    if(1024 <= _id.x || 1024 <= _id.y)
    {
        return;
    }
    
    tex[_id.xy] = float4(1.f, 0.f, 0.f, 1.f);
}

#endif