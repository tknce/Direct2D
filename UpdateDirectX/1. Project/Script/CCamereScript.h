#pragma once
#include <Engine/CScript.h>
class CCamereScript :
    public CScript
{
private:

    
public:
    virtual void begin() override;
    virtual void tick() override;
    

    CLONE(CCamereScript)
public:
    CCamereScript();
    ~CCamereScript();
};

