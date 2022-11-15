#ifndef _STRUCT
#define _STRUCT

// LightInfo
struct tLightInfo
{
	float4	vDiff;	// ³­¹Ý»ç ±¤
	float4	vSpec;	// ¹Ý»ç±¤ °è¼ö
	float4	vEmv;	// È¯°æ±¤

	float4	vWorldPos;	// ±¤¿ø ¿ùµå À§Ä¡
	int		iLightType;	// ±¤¿ø Å¸ÀÔ
	float	fRadius;	// ±¤¿ø ¹Ý°æ (POINT, SPOT)
	float	fAngle;		// ±¤¿ø °¢µµ (SPOT)
	int		pad;
};

#endif