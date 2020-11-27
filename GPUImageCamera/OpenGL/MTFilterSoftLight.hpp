//
//  MTFilterSoftLight.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterSoftLight_hpp
#define MTFilterSoftLight_hpp

#include "MTFilterBase.hpp"

class MTFilterSoftLight : public MTFilterBase {
public:
    MTFilterSoftLight();
    ~MTFilterSoftLight();
    virtual void init() override;
    virtual unsigned render() override;
    
    void setSrcTextureID(unsigned srcTextureID);
    void setOverlayTextureID(unsigned overlayTextureID);
    
private:
    int inputImageTextureUniform, overlayTextureUniform;
    unsigned srcTextureID, overlayTextureID;
    float softLightAlpha;
    int softLightAlphaUniform;
};


#endif /* MTFilterSoftLight_hpp */
