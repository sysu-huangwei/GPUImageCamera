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
    virtual void init() override;
    virtual void resize(int width, int height) override;
    virtual unsigned render() override;
    
    void setSrcTextureID(unsigned srcTextureID);
    
    void setSamplerInterval(float samplerInterval);
    
private:
    int inputImageTextureUniform = -1;
    unsigned srcTextureID = 0;
    
    float singleStepOffset = 0.0f;
    int singleStepOffsetUniform = -1;
    
    float samplerInterval = 1.0f;
};

#endif /* MTFilterGaussianBlurHorizonal_hpp */
