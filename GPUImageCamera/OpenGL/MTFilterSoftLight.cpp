//
//  MTFilterSoftLight.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#include "MTFilterSoftLight.hpp"

const char *kMTFilterSoftLightVertexShaderString = SHADER_STRING
(
 attribute vec2 a_position;
 attribute vec2 a_texCoord;
 varying vec2 texcoordOut;

 void main()
 {
     texcoordOut = a_texCoord;
     gl_Position = vec4(a_position,0.0, 1.0);
 }
);

const char *kMTFilterSoftLightFragmentShaderString = SHADER_STRING
(
 precision highp  float;
 
 varying vec2 texcoordOut;
 uniform sampler2D s_texture;
 uniform sampler2D overlay_texture;
 uniform float softLightAlpha;

 float SoftLight_Fcn(float A, float B)
 {
     float C = 0.0;
     if (B <= 0.5)
     {
         C = A * B / 0.5 + A * A * ( 1.0 - 2.0 * B);
     }
     else
     {
         C = A * (1.0 - B) / 0.5 + sqrt(A) * (2.0 * B - 1.0);
     }
     
     return C;
 }

 void main()
 {
     vec3 src_color = texture2D(s_texture, texcoordOut).rgb;
     vec3 overlay_color = texture2D(overlay_texture, texcoordOut).rgb;
     
     vec3 res_color;
     res_color.r = SoftLight_Fcn(src_color.r, overlay_color.r);
     res_color.g = SoftLight_Fcn(src_color.g, overlay_color.g);
     res_color.b = SoftLight_Fcn(src_color.b, overlay_color.b);
     
     vec4 softLightColor = vec4(mix(src_color, res_color, softLightAlpha), 1.0);
     gl_FragColor = softLightColor;
 }
);

MTFilterSoftLight::MTFilterSoftLight() {
    inputImageTextureUniform = 0;
    overlayTextureUniform = 0;
    srcTextureID = 0;
    overlayTextureID = 0;
    softLightAlphaUniform = 0;
    softLightAlpha = 0.36f;
}

MTFilterSoftLight::~MTFilterSoftLight() {
    
}

void MTFilterSoftLight::init() {
    MTFilterBase::initWithVertexStringAndFragmentString(kMTFilterSoftLightVertexShaderString, kMTFilterSoftLightFragmentShaderString);
    positionAttribute = glGetAttribLocation(programID, "a_position");
    textureCoordinateAttribute = glGetAttribLocation(programID, "a_texCoord");
    inputImageTextureUniform = glGetUniformLocation(programID, "s_texture");
    overlayTextureUniform = glGetUniformLocation(programID, "overlay_texture");
    softLightAlphaUniform = glGetUniformLocation(programID, "softLightAlpha");
}

unsigned MTFilterSoftLight::render() {
    beforeDraw();
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, srcTextureID);
    glUniform1i(inputImageTextureUniform, 2);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, overlayTextureID);
    glUniform1i(overlayTextureUniform, 3);

    glUniform1f(softLightAlphaUniform, softLightAlpha);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    afterDraw();
    
    if (isRenderToOutside) {
        return textureIDOutside;
    } else {
        return textureID;
    }
}

void MTFilterSoftLight::setSrcTextureID(unsigned srcTextureID) {
    this->srcTextureID = srcTextureID;
}

void MTFilterSoftLight::setOverlayTextureID(unsigned overlayTextureID) {
    this->overlayTextureID = overlayTextureID;
}
