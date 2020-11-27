//
//  MTFilterToonifyBackground.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/27.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterToonifyBackground_hpp
#define MTFilterToonifyBackground_hpp

#include "MTFilterBase.hpp"
#include "MTFilterGaussianBlur.hpp"
#include "MTFilterGradient.hpp"
#include "MTFilterSoftProcess.hpp"
#include "MTFilterSoftLight.hpp"

/// 参数
typedef struct ToonifyBackgroundConfig {
    float gradNoiseSamplerInterval = 0.9f;//梯度去噪采样间隔(0 ~ 2)
    float gradBlurSamplerInterval = 2.8f;//梯度模糊采样间隔(0 ~ 6)
    int scaleMaxSize = 640;//梯度缩放最大尺寸(0 ~ 1280)
    float refResolution = 1024.0f;//参考分辨率
    float samplerInterval = 0.8f;//柔顺采样间隔(0 ~ 3)
    float softAlpha = 0.82f;//柔顺融合度(0 ~ 1)
    float softBlurSigma = 30.0f;//柔顺模糊程度(0 ~ 30)
    float softLightAlpha = 0.36f;//柔光程度(0 ~ 1)
} ToonifyBackgroundConfig;

const ToonifyBackgroundConfig DefaultConfig;//默认配置

/// toonify背景效果组合滤镜
class MTFilterToonifyBackground : public MTFilterBase {
public:
    MTFilterToonifyBackground();
    ~MTFilterToonifyBackground();
    
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
    
    /// 获取当前的参数
    ToonifyBackgroundConfig getConfig();
    
    /// 设置参数，需要在初始化之前设置
    /// @param config 参数
    void setConfig(ToonifyBackgroundConfig config);
    
private:
    MTFilterGaussianBlur *gaussianBlurFilter1;
    MTFilterGradient *gradientFilter;
    MTFilterGaussianBlur *gaussianBlurFilter2;
    MTFilterSoftProcess *softProcessFilter;
    MTFilterGaussianBlur *gaussianBlurFilter3;
    MTFilterSoftLight *softLightFilter;
    
    ToonifyBackgroundConfig config;
};

#endif /* MTFilterToonifyBackground_hpp */
