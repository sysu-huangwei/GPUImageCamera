//
//  MTFilterGradient.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterGradient_hpp
#define MTFilterGradient_hpp

#include "MTFilterBase.hpp"

class MTFilterGradient : public MTFilterBase {
public:
    MTFilterGradient();
    ~MTFilterGradient();
    virtual void init() override;
    virtual void resize(int width, int height) override;
    virtual unsigned render() override;
    
    void setSrcTextureID(unsigned srcTextureID);
    
private:
    int inputImageTextureUniform;
    unsigned srcTextureID;
    float textureWidthOffset, textureHeightOffset;
    int textureWidthOffsetUniform, textureHeightOffsetUniform;
};

#endif /* MTFilterGradient_hpp */
