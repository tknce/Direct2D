#ifndef _STRUCT
#define _STRUCT



struct tLightColor
{
    float4 vDiff; // ���ݻ� ��
    float4 vSpec; // �ݻ籤 ���
    float4 vEmb; // ȯ�汤
};


// LightInfo
struct tLightInfo
{
    tLightColor color;	
    float4      vWorldPos; // ���� ���� ��ġ
    float4      vWorldDir;  // ������ ���� ����
    int         iLightType; // ���� Ÿ��
    float       fRadius; // ���� �ݰ� (POINT, SPOT)
    float       fAngle; // ���� ���� (SPOT)
    int         pad;
};


struct tParticle
{
    float4  vRelativePos;
    float4  vDir;
		
    float   fMaxTime;
    float   fCurTime;
    float   fSpeed;
    uint    iActive;
};

struct tParticleShare
{
    uint iAliveCount;
    float fSeta;
    float2 vVelopcity;
    float2 vForce;
    uint2 padding;
};





#endif
