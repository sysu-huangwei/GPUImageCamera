//
//  MTFilterGaussianBlurVertical.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//

#ifndef MTFilterGaussianBlurVertical_hpp
#define MTFilterGaussianBlurVertical_hpp

#include "MTFilterBase.hpp"

/// 纵向高斯模糊
class MTFilterGaussianBlurVertical : public MTFilterBase {
public:
    MTFilterGaussianBlurVertical();
    ~MTFilterGaussianBlurVertical();
    
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
    
    /// 设置采样像素间隔，默认1.0f
    /// @param samplerInterval 高斯模糊采样间隔
    void setSamplerInterval(float samplerInterval);
    
private:
    int inputImageTextureUniform = -1;
    unsigned srcTextureID = 0;
    
    float singleStepOffset = 0.0f;
    int singleStepOffsetUniform = -1;
    
    float samplerInterval = 1.0f;
};

#endif /* MTFilterGaussianBlurVertical_hpp */
