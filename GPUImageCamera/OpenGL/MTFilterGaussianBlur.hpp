//
//  MTFilterGaussianBlur.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterGaussianBlur_hpp
#define MTFilterGaussianBlur_hpp

#include "MTFilterGaussianBlurHorizonal.hpp"
#include "MTFilterGaussianBlurVertical.hpp"

class MTFilterGaussianBlur : public MTFilterBase {
public:
    MTFilterGaussianBlur();
    ~MTFilterGaussianBlur();
    virtual void init();
    virtual void resize(int width, int height);
    virtual unsigned render();
    
    virtual void setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside);
    
    void setSrcTextureID(unsigned srcTextureID);
    
    void setSamplerInterval(float samplerInterval);
    
private:
    MTFilterGaussianBlurHorizonal horizonalFilter;
    MTFilterGaussianBlurVertical verticalFilter;
};


#endif /* MTFilterGaussianBlur_hpp */