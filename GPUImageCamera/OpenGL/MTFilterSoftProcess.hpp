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

/// 柔顺
class MTFilterSoftProcess : public MTFilterBase {
public:
    MTFilterSoftProcess();
    ~MTFilterSoftProcess();
    
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
    
    /// 设置经过梯度处理的图像纹理ID
    /// @param processTextureID 经过梯度处理的图像纹理ID
    void setProcessTextureID(unsigned processTextureID);
    
    /// 设置参考分辨率，默认1024.0f
    /// @param refResolution 默认参考分辨率
    void setRefResolution(float refResolution);
    
    /// 设置柔顺采样像素间隔，默认0.8f
    /// @param samplerInterval 柔顺采样间隔
    void setSamplerInterval(float samplerInterval);
    
    /// 设置柔顺融合度，默认0.82f
    /// @param alpha 柔顺融合度
    void setAlpha(float alpha);
    
private:
    int inputImageTextureUniform = -1, processTextureUniform = -1;
    unsigned srcTextureID = 0, processTextureID = 0;
    
    float textureWidthOffset = 0.0f, textureHeightOffset = 0.0f, alpha = 0.82f;
    float kernel[KERNEL_SIZE] = {0.013298, 0.013291, 0.013269, 0.013232, 0.013180, 0.013115, 0.013035, 0.012941, 0.012834, 0.012713, 0.012579, 0.012434, 0.012276, 0.012106, 0.011926, 0.011736, 0.011535};
    int textureWidthOffsetUniform = -1, textureHeightOffsetUniform = -1, alphaUniform = -1, kernelUniform = -1;
    
    float refResolution = 1024.0f, samplerInterval = 0.8f;
};

#endif /* MTFilterSoftProcess_hpp */
