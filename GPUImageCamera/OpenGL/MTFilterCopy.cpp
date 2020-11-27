//
//  MTFilterCopy.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#include "MTFilterCopy.hpp"

const char *kMTFilterCopyVertexShaderString = SHADER_STRING
(
 attribute vec2 a_position;
 attribute vec2 a_texCoord;
 varying vec2 texcoordOut;

 void main()
 {
     texcoordOut = a_texCoord;
     gl_Position = vec4(a_position,0.0,1.0);
 }
);

const char *kMTFilterCopyFragmentShaderString = SHADER_STRING
(
 precision highp float;

 uniform sampler2D  u_texture;
 varying vec2 texcoordOut;
 void main()
 {
     vec4 srcColor = texture2D(u_texture,texcoordOut);
     gl_FragColor = srcColor;
 }
);

MTFilterCopy::MTFilterCopy():MTFilterBase() {
    
}

MTFilterCopy::~MTFilterCopy() {
    
}

void MTFilterCopy::init() {
    MTFilterBase::initWithVertexStringAndFragmentString(kMTFilterCopyVertexShaderString, kMTFilterCopyFragmentShaderString);
    positionAttribute = glGetAttribLocation(programID, "a_position");
    textureCoordinateAttribute = glGetAttribLocation(programID, "a_texCoord");
    inputImageTextureUniform = glGetUniformLocation(programID, "u_texture");
}

unsigned MTFilterCopy::render() {
    beforeDraw();
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, srcTextureID);
    glUniform1i(inputImageTextureUniform, 2);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    afterDraw();
    return textureID;
}

void MTFilterCopy::setSrcTextureID(unsigned srcTextureID) {
    this->srcTextureID = srcTextureID;
}
