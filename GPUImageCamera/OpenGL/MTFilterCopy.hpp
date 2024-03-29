//
//  MTFilterCopy.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//

#ifndef MTFilterCopy_hpp
#define MTFilterCopy_hpp

#include "MTFilterBase.hpp"

/// 画原图的滤镜
class MTFilterCopy : public MTFilterBase {
public:
    MTFilterCopy();
    ~MTFilterCopy();
    
    /// 初始化，必须在GL线程，子类实现这个方法去做GL相关的初始化操作
    virtual void init() override;
    
    /// 渲染，必须在GL线程
    /// @return 结果纹理ID
    virtual unsigned render() override;
    
    /// 设置输入图像的纹理ID
    /// @param srcTextureID 输入图像的纹理ID
    void setSrcTextureID(unsigned srcTextureID);
    
private:
    int inputImageTextureUniform = -1;
    unsigned srcTextureID = 0;
};

#endif /* MTFilterCopy_hpp */
