//
//  MTFilterToonifyBackground.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/27.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterToonifyBackground_hpp
#define MTFilterToonifyBackground_hpp

#include "MTFilterBase.hpp"
#include "MTFilterGaussianBlur.hpp"
#include "MTFilterGradient.hpp"
#include "MTFilterSoftProcess.hpp"
#include "MTFilterSoftLight.hpp"

class MTFilterToonifyBackground : public MTFilterBase {
public:
    MTFilterToonifyBackground();
    ~MTFilterToonifyBackground();
    virtual void init() override;
    virtual void release() override;
    virtual void resize(int width, int height) override;
    virtual unsigned render() override;
    
    virtual void setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside) override;
    
    void setSrcTextureID(unsigned srcTextureID);
    
private:
    MTFilterGaussianBlur *gaussianBlurFilter1;
    MTFilterGradient *gradientFilter;
    MTFilterGaussianBlur *gaussianBlurFilter2;
    MTFilterSoftProcess *softProcessFilter;
    MTFilterGaussianBlur *gaussianBlurFilter3;
    MTFilterSoftLight *softLightFilter;
};

#endif /* MTFilterToonifyBackground_hpp */
