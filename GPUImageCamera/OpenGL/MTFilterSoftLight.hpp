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

/// 柔光
class MTFilterSoftLight : public MTFilterBase {
public:
    MTFilterSoftLight();
    ~MTFilterSoftLight();
    
    /// 初始化，必须在GL线程，子类实现这个方法去做GL相关的初始化操作
    virtual void init() override;
    
    /// 渲染，必须在GL线程
    /// @return 结果纹理ID
    virtual unsigned render() override;
    
    /// 设置输入图像的纹理ID
    /// @param srcTextureID 输入图像的纹理ID
    void setSrcTextureID(unsigned srcTextureID);
    
    /// 设置经过柔顺之后的图像纹理ID
    /// @param overlayTextureID 柔顺之后的图像纹理ID
    void setOverlayTextureID(unsigned overlayTextureID);
    
    /// 设置柔光程度，默认0.36f
    /// @param softLightAlpha 柔光程度
    void setSoftLightAlpha(float softLightAlpha);
    
private:
    int inputImageTextureUniform = -1, overlayTextureUniform = -1;
    unsigned srcTextureID = 0, overlayTextureID = 0;
    
    float softLightAlpha = 0.36f;
    int softLightAlphaUniform = -1;
};


#endif /* MTFilterSoftLight_hpp */
