#ifndef _STRUCT
#define _STRUCT

// LightInfo
struct tLightInfo
{
	float4	vDiff;	// ���ݻ� ��
	float4	vSpec;	// �ݻ籤 ���
	float4	vEmv;	// ȯ�汤

	float4	vWorldPos;	// ���� ���� ��ġ
	int		iLightType;	// ���� Ÿ��
	float	fRadius;	// ���� �ݰ� (POINT, SPOT)
	float	fAngle;		// ���� ���� (SPOT)
	int		pad;
};

#endif