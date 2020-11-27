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

/// 高斯模糊
class MTFilterGaussianBlur : public MTFilterBase {
public:
    MTFilterGaussianBlur();
    ~MTFilterGaussianBlur();
    
    /// 初始化，必须在GL线程，子类实现这个方法去做GL相关的初始化操作
    virtual void init() override;
    
    /// 释放资源，必须在GL线程，子类实现这个方法去做GL相关的释放操作
    virtual void release() override;
    
    /// 设置绘制尺寸，必须在GL线程，内部会创建对应尺寸的FBO
    /// @param width 宽
    /// @param height 高
    virtual void resize(int width, int height) override;
    
    /// 渲染，必须在GL线程
    /// @return 结果纹理ID
    virtual unsigned render() override;
    
    /// 设置外部的纹理ID和FBO，如果都设置了>0的合法值，渲染的时候会绘制到这个buffer上，如果需要重新绘制到内置的FBO，设置0, 0
    /// @param textureIDOutside 外部的纹理ID
    /// @param fboIDOutside 外部的FBO
    virtual void setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside) override;
    
    /// 设置输入图像的纹理ID
    /// @param srcTextureID 输入图像的纹理ID
    void setSrcTextureID(unsigned srcTextureID);
    
    /// 设置采样像素间隔，默认1.0f
    /// @param samplerInterval 高斯模糊采样间隔
    void setSamplerInterval(float samplerInterval);
    
private:
    MTFilterGaussianBlurHorizonal horizonalFilter;
    MTFilterGaussianBlurVertical verticalFilter;
};


#endif /* MTFilterGaussianBlur_hpp */
