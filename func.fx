#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "register.fx"

void CalcLight2D(inout tLightColor _LightColor, float3 _vWorldPos, int iLightIdx)
{
     // Directional Light
    if (0 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff;
    }
    
    // Point Light
    else if (1 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);
            
        if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
        {
            float fRatio = 1.f - fDistance / g_Light2DBuffer[iLightIdx].fRadius;

            _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff * fRatio;
        }
    }

    else
    {
        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);

        if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
        {
         
            if(g_Light2DBuffer[iLightIdx].fAngle)
            {
                _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff;
            }
            
        }
    }
}

// 0 ~ 1 사이 값을 반환
float Random(int key)
{
    float fValue = 0.f;
    
    return fValue;
}

#endif
