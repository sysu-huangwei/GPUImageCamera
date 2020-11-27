//
//  MTFilterGaussianBlurHorizonal.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//

#include "MTFilterGaussianBlurHorizonal.hpp"

const char *kMTFilterGaussianBlurBaseVertexShaderString_Horizonal = SHADER_STRING
(
 attribute vec2 a_position;
 attribute vec2 a_texCoord;
 varying vec2 v_texcoord;

 uniform float u_singleStepOffset;
 varying vec4 v_texcoordOffset[4];


 void main() {
     v_texcoord = a_texCoord;
     gl_Position =  vec4(a_position, 0.0, 1.0);
     v_texcoordOffset[0] = vec4(v_texcoord + vec2(-4.0*u_singleStepOffset,0.0),v_texcoord + vec2(1.0*u_singleStepOffset,0.0));
     v_texcoordOffset[1] = vec4(v_texcoord + vec2(-3.0*u_singleStepOffset,0.0),v_texcoord + vec2(2.0*u_singleStepOffset,0.0));
     v_texcoordOffset[2] = vec4(v_texcoord + vec2(-2.0*u_singleStepOffset,0.0),v_texcoord + vec2(3.0*u_singleStepOffset,0.0));
     v_texcoordOffset[3] = vec4(v_texcoord + vec2(-1.0*u_singleStepOffset,0.0),v_texcoord + vec2(4.0*u_singleStepOffset,0.0));
 }
);

const char *kMTFilterGaussianBlurBaseFragmentShaderString_Horizonal = SHADER_STRING
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

MTFilterGaussianBlurHorizonal::MTFilterGaussianBlurHorizonal() {
    
}

MTFilterGaussianBlurHorizonal::~MTFilterGaussianBlurHorizonal() {
    
}

void MTFilterGaussianBlurHorizonal::init() {
    MTFilterBase::initWithVertexStringAndFragmentString(kMTFilterGaussianBlurBaseVertexShaderString_Horizonal, kMTFilterGaussianBlurBaseFragmentShaderString_Horizonal);
    positionAttribute = glGetAttribLocation(programID, "a_position");
    textureCoordinateAttribute = glGetAttribLocation(programID, "a_texCoord");
    inputImageTextureUniform = glGetUniformLocation(programID, "s_texture");
    singleStepOffsetUniform = glGetUniformLocation(programID, "u_singleStepOffset");
}

void MTFilterGaussianBlurHorizonal::resize(int width, int height) {
    MTFilterBase::resize(width, height);
    singleStepOffset = samplerInterval / (float)width;
}
    
unsigned MTFilterGaussianBlurHorizonal::render() {
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

void MTFilterGaussianBlurHorizonal::setSrcTextureID(unsigned srcTextureID) {
    this->srcTextureID = srcTextureID;
}

void MTFilterGaussianBlurHorizonal::setSamplerInterval(float samplerInterval) {
    this->samplerInterval = samplerInterval;
}
