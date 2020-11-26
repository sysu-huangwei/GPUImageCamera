//
//  MTFilterSoftProcess.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterSoftProcess_hpp
#define MTFilterSoftProcess_hpp

#include "MTFilterBase.hpp"

#define KERNEL_SIZE 17

class MTFilterSoftProcess : public MTFilterBase {
public:
    MTFilterSoftProcess();
    ~MTFilterSoftProcess();
    virtual void init();
    virtual void resize(int width, int height);
    virtual unsigned render();
    
    void setSrcTextureID(unsigned srcTextureID);
    void setProcessTextureID(unsigned processTextureID);
    
    void setRefResolution(float refResolution);
    void setSamplerInterval(float samplerInterval);
    
private:
    int inputImageTextureUniform, processTextureUniform;
    unsigned srcTextureID, processTextureID;
    float textureWidthOffset, textureHeightOffset, alpha, kernel[KERNEL_SIZE] = {0.013298, 0.013291, 0.013269, 0.013232, 0.013180, 0.013115, 0.013035, 0.012941, 0.012834, 0.012713, 0.012579, 0.012434, 0.012276, 0.012106, 0.011926, 0.011736, 0.011535};
    int textureWidthOffsetUniform, textureHeightOffsetUniform, alphaUniform, kernelUniform;
    
    float refResolution, samplerInterval;
};

#endif /* MTFilterSoftProcess_hpp */
