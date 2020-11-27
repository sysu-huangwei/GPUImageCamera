//
//  MTFilterGaussianBlurVertical.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#include "MTFilterGaussianBlurVertical.hpp"

const char *kMTFilterGaussianBlurBaseVertexShaderString_Vertical = SHADER_STRING
(
 attribute vec2 a_position;
 attribute vec2 a_texCoord;
 varying vec2 v_texcoord;

 uniform float u_singleStepOffset;
 varying vec4 v_texcoordOffset[4];


 void main() {
     v_texcoord = a_texCoord;
     gl_Position =  vec4(a_position, 0.0, 1.0);
     v_texcoordOffset[0] =  vec4(v_texcoord + vec2(0.0,-4.0*u_singleStepOffset),v_texcoord + vec2(0.0,1.0*u_singleStepOffset));
     v_texcoordOffset[1] =  vec4(v_texcoord + vec2(0.0,-3.0*u_singleStepOffset),v_texcoord + vec2(0.0,2.0*u_singleStepOffset));
     v_texcoordOffset[2] =  vec4(v_texcoord + vec2(0.0,-2.0*u_singleStepOffset),v_texcoord + vec2(0.0,3.0*u_singleStepOffset));
     v_texcoordOffset[3] =  vec4(v_texcoord + vec2(0.0,-1.0*u_singleStepOffset),v_texcoord + vec2(0.0,4.0*u_singleStepOffset));
 }
);

const char *kMTFilterGaussianBlurBaseFragmentShaderString_Vertical = SHADER_STRING
(
 precision highp float;

 varying vec2 v_texcoord;
 uniform sampler2D s_texture;
 varying vec4 v_texcoordOffset[4];

 vec4 gauss() {
     vec4 sum = vec4(0.0);
     sum += texture2D(s_texture, v_texcoordOffset[0].rg) * 0.05;
     sum += texture2D(s_texture, v_texcoordOffset[1].rg) * 0.09;
     sum += texture2D(s_texture, v_texcoordOffset[2].rg) * 0.12;
     sum += texture2D(s_texture, v_texcoordOffset[3].rg) * 0.15;
     sum += texture2D(s_texture, v_texcoord) * 0.18;
     sum += texture2D(s_texture, v_texcoordOffset[0].ba) * 0.15;
     sum += texture2D(s_texture, v_texcoordOffset[1].ba) * 0.12;
     sum += texture2D(s_texture, v_texcoordOffset[2].ba) * 0.09;
     sum += texture2D(s_texture, v_texcoordOffset[3].ba) * 0.05;
     return sum;
 }

 void main() {
     vec4 resultColor = gauss();
    gl_FragColor = resultColor;
 }
);

MTFilterGaussianBlurVertical::MTFilterGaussianBlurVertical() {
    
}

MTFilterGaussianBlurVertical::~MTFilterGaussianBlurVertical() {
    
}

void MTFilterGaussianBlurVertical::init() {
    MTFilterBase::initWithVertexStringAndFragmentString(kMTFilterGaussianBlurBaseVertexShaderString_Vertical, kMTFilterGaussianBlurBaseFragmentShaderString_Vertical);
    positionAttribute = glGetAttribLocation(programID, "a_position");
    textureCoordinateAttribute = glGetAttribLocation(programID, "a_texCoord");
    inputImageTextureUniform = glGetUniformLocation(programID, "s_texture");
    singleStepOffsetUniform = glGetUniformLocation(programID, "u_singleStepOffset");
}

void MTFilterGaussianBlurVertical::resize(int width, int height) {
    MTFilterBase::resize(width, height);
    singleStepOffset = samplerInterval / (float)height;
}
    
unsigned MTFilterGaussianBlurVertical::render() {
    beforeDraw();
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, srcTextureID);
    glUniform1i(inputImageTextureUniform, 2);
    
    glUniform1f(singleStepOffsetUniform, singleStepOffset);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    afterDraw();
    
    if (isRenderToOutside) {
        return textureIDOutside;
    } else {
        return textureID;
    }
}

void MTFilterGaussianBlurVertical::setSrcTextureID(unsigned srcTextureID) {
    this->srcTextureID = srcTextureID;
}

void MTFilterGaussianBlurVertical::setSamplerInterval(float samplerInterval) {
    this->samplerInterval = samplerInterval;
}
