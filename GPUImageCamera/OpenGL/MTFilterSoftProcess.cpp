//
//  MTFilterSoftProcess.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#include "MTFilterSoftProcess.hpp"

const char *kMTFilterSoftProcessVertexShaderString = SHADER_STRING
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

const char *kMTFilterSoftProcessFragmentShaderString = SHADER_STRING
(
 precision highp float;

 uniform sampler2D u_texture;
 uniform sampler2D processTexture;

 uniform float textureWidthOffset;
 uniform float textureHeightOffset;
 uniform float alpha;

 uniform float kernel[KERNEL_SIZE];

 varying vec2 texcoordOut;

 float sdEllipse( in vec2 p, in vec2 ab )
 {
     p = abs(p); if( p.x > p.y ) {p=p.yx;ab=ab.yx;}
     float l = ab.y*ab.y - ab.x*ab.x;
     float m = ab.x*p.x/l;      float m2 = m*m;
     float n = ab.y*p.y/l;      float n2 = n*n;
     float c = (m2+n2-1.0)/3.0; float c3 = c*c*c;
     float q = c3 + m2*n2*2.0;
     float d = c3 + m2*n2;
     float g = m + m*n2;
     float co;
     if( d<0.0 )
     {
         float h = acos(q/c3)/3.0;
         float s = cos(h);
         float t = sin(h)*sqrt(3.0);
         float rx = sqrt( -c*(s + t + 2.0) + m2 );
         float ry = sqrt( -c*(s - t + 2.0) + m2 );
         co = (ry+sign(l)*rx+abs(g)/(rx*ry)- m)/2.0;
     }
     else
     {
         float h = 2.0*m*n*sqrt( d );
         float s = sign(q+h)*pow(abs(q+h), 1.0/3.0);
         float u = sign(q-h)*pow(abs(q-h), 1.0/3.0);
         float rx = -s - u - c*4.0 + 2.0*m2;
         float ry = (s - u)*sqrt(3.0);
         float rm = sqrt( rx*rx + ry*ry );
         co = (ry/sqrt(rm-rx)+2.0*g/rm-m)/2.0;
     }
     vec2 r = ab * vec2(co, sqrt(1.0-co*co));
     return length(r-p) * sign(p.y-r.y);
 }

 float blendScreen(float base, float blend) {
     return 1.0-((1.0-base)*(1.0-blend));
 }

 vec3 blendScreen(vec3 base, vec3 blend) {
     return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b));
 }

 void main()
 {
     vec2 uv = texcoordOut;
     vec2 shiftingSize = vec2(textureWidthOffset, textureHeightOffset);

     vec4 origColor = texture2D(u_texture, uv);
     vec4 gradientColor = texture2D(processTexture, uv);
     
     vec2 gradient = gradientColor.rg * 2.0 - 1.0;

     float direction = atan(gradient.y, gradient.x+0.0001) * 0.5 + 1.570795;
     float sumWeight = kernel[0];
     vec4 sumColor = origColor*kernel[0];
     vec2 directionUV = vec2(cos(direction), sin(direction)) * shiftingSize;
     for (int i = 1; i < KERNEL_SIZE; ++i) {
         vec2 offset = directionUV * float(i);
         vec4 color1 = texture2D(u_texture, uv + offset);
         vec4 color2 = texture2D(u_texture, uv - offset);
         float weight = kernel[i];
         sumWeight += 2.0 * weight;
         sumColor += (color1 + color2) * weight;
     }
     vec4 resultColor = vec4(mix(origColor.rgb, sumColor.rgb / sumWeight, alpha),1.0);
     gl_FragColor = resultColor;
 }

);

MTFilterSoftProcess::MTFilterSoftProcess() {
    inputImageTextureUniform = 0;
    processTextureUniform = 0;
    srcTextureID = 0;
    processTextureID = 0;
    textureWidthOffset = 0.0f;
    textureHeightOffset = 0.0f;
    alpha = 0.82f;
    textureWidthOffsetUniform = 0;
    textureHeightOffsetUniform = 0;
    alphaUniform = 0;
    kernelUniform = 0;
    refResolution = 1024.0f;
    samplerInterval = 0.8f;
}

MTFilterSoftProcess::~MTFilterSoftProcess() {
    
}

void MTFilterSoftProcess::init() {
    MTFilterBase::initWithVertexStringAndFragmentString(kMTFilterSoftProcessVertexShaderString, kMTFilterSoftProcessFragmentShaderString);
    positionAttribute = glGetAttribLocation(programID, "a_position");
    textureCoordinateAttribute = glGetAttribLocation(programID, "a_texCoord");
    inputImageTextureUniform = glGetUniformLocation(programID, "u_texture");
    processTextureUniform = glGetUniformLocation(programID, "processTexture");
    textureWidthOffsetUniform = glGetUniformLocation(programID, "textureWidthOffset");
    textureHeightOffsetUniform = glGetUniformLocation(programID, "textureHeightOffset");
    alphaUniform = glGetUniformLocation(programID, "alpha");
    kernelUniform = glGetUniformLocation(programID, "kernel");
}

void MTFilterSoftProcess::resize(int width, int height) {
    MTFilterBase::resize(width, height);
    float adaptiveCoef = (float)(width > height ? width : height) / refResolution;
    textureWidthOffset = adaptiveCoef * samplerInterval / (float)width;
    textureHeightOffset = adaptiveCoef * samplerInterval / (float)height;
}
    
unsigned MTFilterSoftProcess::render() {
    beforeDraw();
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, srcTextureID);
    glUniform1i(inputImageTextureUniform, 2);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, processTextureID);
    glUniform1i(processTextureUniform, 3);
    
    glUniform1f(textureWidthOffsetUniform, textureWidthOffset);
    glUniform1f(textureHeightOffsetUniform, textureHeightOffset);
    glUniform1f(alphaUniform, alpha);
    glUniform1fv(kernelUniform, KERNEL_SIZE, kernel);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    afterDraw();
    
    if (isRenderToOutside) {
        return textureIDOutside;
    } else {
        return textureID;
    }
}

void MTFilterSoftProcess::setSrcTextureID(unsigned srcTextureID) {
    this->srcTextureID = srcTextureID;
}

void MTFilterSoftProcess::setProcessTextureID(unsigned processTextureID) {
    this->processTextureID = processTextureID;
}

void MTFilterSoftProcess::setRefResolution(float refResolution) {
    this->refResolution = refResolution;
}

void MTFilterSoftProcess::setSamplerInterval(float samplerInterval) {
    this->samplerInterval = samplerInterval;
}
