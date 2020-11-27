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

/// 求梯度
class MTFilterGradient : public MTFilterBase {
public:
    MTFilterGradient();
    ~MTFilterGradient();
    
    /// 初始化，必须在GL线程，子类实现这个方法去做GL相关的初始化操作
    virtual void init() override;
    
    /// 设置绘制尺寸，必须在GL线程，内部会创建对应尺寸的FBO
    /// @param width 宽
    /// @param height 高
    virtual void resize(int width, int height) override;
    
    /// 渲染，必须在GL线程
    /// @return 结果纹理ID
    virtual unsigned render() override;
    
    /// 设置输入图像的纹理ID
    /// @param srcTextureID 输入图像的纹理ID
    void setSrcTextureID(unsigned srcTextureID);
    
private:
    int inputImageTextureUniform = -1;
    unsigned srcTextureID = 0;
    
    float textureWidthOffset = 0.0f, textureHeightOffset = 0.0f;
    int textureWidthOffsetUniform = -1, textureHeightOffsetUniform = -1;
};

#endif /* MTFilterGradient_hpp */
