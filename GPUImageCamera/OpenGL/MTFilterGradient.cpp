//
//  MTFilterGradient.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#include "MTFilterGradient.hpp"

const char *kMTFilterGradientVertexShaderString = SHADER_STRING
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

const char *kMTFilterGradientFragmentShaderString = SHADER_STRING
(
 precision highp float;

 uniform sampler2D u_texture;
 uniform float textureWidthOffset;
 uniform float textureHeightOffset;

 varying vec2 texcoordOut;

 void main()
 {
     vec2 shiftingSize = vec2(textureWidthOffset, textureHeightOffset);

     vec3 color00 = texture2D(u_texture, texcoordOut).rgb;
     vec3 color01 = texture2D(u_texture, texcoordOut + vec2(shiftingSize.x, 0.0)).rgb;
     vec3 color10 = texture2D(u_texture, texcoordOut + vec2(0.0, shiftingSize.y)).rgb;
     vec3 color11 = texture2D(u_texture, texcoordOut + shiftingSize).rgb;

     vec3 colorX = color01 + color11 - color00 - color10;
     vec3 colorY = color10 + color11 - color00 - color01;

     float grayX = dot(colorX, vec3(0.298912, 0.586611, 0.114478));
     float grayY = dot(colorY, vec3(0.298912, 0.586611, 0.114478));

     vec2 grad = vec2(grayX, grayY) * 0.5;
     vec2 grad2 = grad * grad;
     float gradLen2 = grad2.x + grad2.y;

     vec2 gradDouble = gradLen2 != 0.0 ? vec2(grad2.x - grad2.y, 2.0 * grad.x * grad.y) / gradLen2 : vec2(0.0);
     gl_FragColor = vec4(gradDouble * 0.5 + 0.5, 0.0, 1.0);
 }
);

MTFilterGradient::MTFilterGradient() {
    
}

MTFilterGradient::~MTFilterGradient() {
    
}

void MTFilterGradient::init() {
    MTFilterBase::initWithVertexStringAndFragmentString(kMTFilterGradientVertexShaderString, kMTFilterGradientFragmentShaderString);
    positionAttribute = glGetAttribLocation(programID, "a_position");
    textureCoordinateAttribute = glGetAttribLocation(programID, "a_texCoord");
    inputImageTextureUniform = glGetUniformLocation(programID, "u_texture");
    textureWidthOffsetUniform = glGetUniformLocation(programID, "textureWidthOffset");
    textureHeightOffsetUniform = glGetUniformLocation(programID, "textureHeightOffset");
}

void MTFilterGradient::resize(int width, int height) {
    MTFilterBase::resize(width, height);
    textureWidthOffset = 1.0f / (float)width;
    textureHeightOffset = 1.0f / (float)height;
}
    
unsigned MTFilterGradient::render() {
    beforeDraw();
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, srcTextureID);
    glUniform1i(inputImageTextureUniform, 2);
    
    glUniform1f(textureWidthOffsetUniform, textureWidthOffset);
    glUniform1f(textureHeightOffsetUniform, textureHeightOffset);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    afterDraw();
    
    if (isRenderToOutside) {
        return textureIDOutside;
    } else {
        return textureID;
    }
}

void MTFilterGradient::setSrcTextureID(unsigned srcTextureID) {
    this->srcTextureID = srcTextureID;
}
