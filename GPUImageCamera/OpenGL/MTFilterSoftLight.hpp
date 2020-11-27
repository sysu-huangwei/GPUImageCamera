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
    int inputImageTextureUniform = -1, overlayTextureUniform = -1;
    unsigned srcTextureID = 0, overlayTextureID = 0;
    
    float softLightAlpha = 0.36f;
    int softLightAlphaUniform = -1;
};


#endif /* MTFilterSoftLight_hpp */
