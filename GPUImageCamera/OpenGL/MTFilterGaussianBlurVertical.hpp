//
//  MTFilterGaussianBlurVertical.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterGaussianBlurVertical_hpp
#define MTFilterGaussianBlurVertical_hpp

#include "MTFilterBase.hpp"

class MTFilterGaussianBlurVertical : public MTFilterBase {
public:
    MTFilterGaussianBlurVertical();
    ~MTFilterGaussianBlurVertical();
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

#endif /* MTFilterGaussianBlurVertical_hpp */
