#ifndef _STRUCT
#define _STRUCT


struct tLightColor
{
    float4 vDiff; // ³­¹Ý»ç ±¤
    float4 vSpec; // ¹Ý»ç±¤ °è¼ö
    float4 vEmb; // È¯°æ±¤
};

// LightInfo
struct tLightInfo
{

    tLightColor color;      
	float4	    vWorldPos;	// ±¤¿ø ¿ùµå À§Ä¡
    float4      vWorldDir;  // ±¤¿øÀÇ ¿ùµå ¹æÇâ
	int		    iLightType;	// ±¤¿ø Å¸ÀÔ
	float	    fRadius;	// ±¤¿ø ¹Ý°æ (POINT, SPOT)
	float	    fAngle;		// ±¤¿ø °¢µµ (SPOT)
	int		    pad;
};

struct tParticle
{
    float4  vRelaticePos;
    float4  vDir;
    
    float   fMaxTime;
    float   fCurTime;
    float   fSpeed;
    uint    iActive;
};

#endif