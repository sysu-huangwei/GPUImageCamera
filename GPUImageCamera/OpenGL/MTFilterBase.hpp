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
    virtual void initWithVertexStringAndFragmentString(const char* vs, const char* fs);
    virtual void resize(int width, int height);
    virtual void release();
    virtual unsigned render() = 0;
    virtual void beforeDraw();
    virtual void afterDraw();
    
private:
    int width, height;
    unsigned textureID, fboID, programID;
    int positionAttribute, textureCoordinateAttribute;
};

#endif /* MTFilterBase_hpp */
