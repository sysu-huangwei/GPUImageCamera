//
//  MTFilterBase.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterBase_hpp
#define MTFilterBase_hpp

#include "BaseGLUtils.hpp"

// 顶点坐标
const static float imageVertices[8] = {
    -1.0f, 1.0f, // top left
    1.0f, 1.0f,  // top right
    -1.0f, -1.0f, // bottom left
    1.0f, -1.0f, // bottom right
};

// 纹理坐标
const static float textureCoordinates[8] = {
    0.0f, 1.0f, // bottom left
    1.0f, 1.0f, // bottom right
    0.0f, 0.0f, // top left
    1.0f, 0.0f, // top right
};

class MTFilterBase {
public:
    MTFilterBase();
    ~MTFilterBase();
    virtual void init();
    virtual void resize(int width, int height);
    virtual void release();
    virtual unsigned render() = 0;
    virtual void beforeDraw();
    virtual void afterDraw();
    
    virtual void setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside);
    
protected:
    int width = 0, height = 0;
    unsigned textureID = 0, fboID = 0, programID = 0;
    int positionAttribute = -1, textureCoordinateAttribute = -1;
    
    virtual void initWithVertexStringAndFragmentString(const char* vs, const char* fs);
    
    unsigned textureIDOutside = 0, fboIDOutside = 0;
    bool isRenderToOutside = false;
};

#endif /* MTFilterBase_hpp */
