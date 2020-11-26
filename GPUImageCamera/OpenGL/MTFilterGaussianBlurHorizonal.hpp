//
//  MTFilterGaussianBlurHorizonal.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterGaussianBlurHorizonal_hpp
#define MTFilterGaussianBlurHorizonal_hpp

#include "MTFilterBase.hpp"

class MTFilterGaussianBlurHorizonal : public MTFilterBase {
public:
    MTFilterGaussianBlurHorizonal();
    ~MTFilterGaussianBlurHorizonal();
    virtual void init();
    virtual void resize(int width, int height);
    virtual unsigned render();
    
    void setSrcTextureID(unsigned srcTextureID);
    
    void setSamplerInterval(float samplerInterval);
    
private:
    int inputImageTextureUniform;
    unsigned srcTextureID;
    float singleStepOffset;
    int singleStepOffsetUniform;
    
    float samplerInterval;
};

#endif /* MTFilterGaussianBlurHorizonal_hpp */
