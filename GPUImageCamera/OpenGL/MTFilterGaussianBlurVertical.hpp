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
    int inputImageTextureUniform;
    unsigned srcTextureID;
    float singleStepOffset;
    int singleStepOffsetUniform;
    
    float samplerInterval;
};

#endif /* MTFilterGaussianBlurVertical_hpp */
