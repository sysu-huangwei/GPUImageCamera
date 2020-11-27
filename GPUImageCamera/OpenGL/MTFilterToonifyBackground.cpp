//
//  MTFilterToonifyBackground.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/27.
//  Copyright © 2020 meitu. All rights reserved.
//

#include "MTFilterToonifyBackground.hpp"
#include <stdio.h>
#include <cmath>

MTFilterToonifyBackground::MTFilterToonifyBackground() {
    gaussianBlurFilter1 = new MTFilterGaussianBlur();
    gradientFilter = new MTFilterGradient();
    gaussianBlurFilter2 = new MTFilterGaussianBlur();
    softProcessFilter = new MTFilterSoftProcess();
    gaussianBlurFilter3 = new MTFilterGaussianBlur();
    softLightFilter = new MTFilterSoftLight();
    config = DefaultConfig;
}

MTFilterToonifyBackground::~MTFilterToonifyBackground() {
    SAFE_DELETE(gaussianBlurFilter1);
    SAFE_DELETE(gradientFilter);
    SAFE_DELETE(gaussianBlurFilter2);
    SAFE_DELETE(softProcessFilter);
    SAFE_DELETE(gaussianBlurFilter3);
    SAFE_DELETE(softLightFilter);
}

void MTFilterToonifyBackground::init() {
    gaussianBlurFilter1->setSamplerInterval(config.gradNoiseSamplerInterval);
    gaussianBlurFilter2->setSamplerInterval(config.gradBlurSamplerInterval);
    softProcessFilter->setRefResolution(config.refResolution);
    softProcessFilter->setSamplerInterval(config.samplerInterval);
    softProcessFilter->setAlpha(config.softAlpha);
    gaussianBlurFilter3->setSamplerInterval(1.5f);
    softLightFilter->setSoftLightAlpha(config.softAlpha);
    
    gaussianBlurFilter1->init();
    gradientFilter->init();
    gaussianBlurFilter2->init();
    softProcessFilter->init();
    gaussianBlurFilter3->init();
    softLightFilter->init();
}

void MTFilterToonifyBackground::release() {
    gaussianBlurFilter1->release();
    gradientFilter->release();
    gaussianBlurFilter2->release();
    softProcessFilter->release();
    gaussianBlurFilter3->release();
    softLightFilter->release();
}

void MTFilterToonifyBackground::resize(int width, int height) {
    int scaleMaxSize = config.scaleMaxSize;
    int scaleWidth, scaleHeight;
    if (height >= width) {
        scaleHeight = scaleMaxSize;
        scaleWidth = std::ceil(width * scaleHeight / height);
    } else {
        scaleWidth = scaleMaxSize;
        scaleHeight = std::ceil(height * scaleWidth / width);
    }
    gaussianBlurFilter1->resize(scaleWidth, scaleHeight);
    gradientFilter->resize(scaleWidth, scaleHeight);
    gaussianBlurFilter2->resize(scaleWidth, scaleHeight);
    softProcessFilter->resize(width, height);
    gaussianBlurFilter3->resize(scaleWidth, scaleHeight);
    softLightFilter->resize(width, height);
}
    
unsigned MTFilterToonifyBackground::render() {
    GLuint gaussianBlurTextureID1 = gaussianBlurFilter1->render();
    
    gradientFilter->setSrcTextureID(gaussianBlurTextureID1);
    GLuint gradientTextureID = gradientFilter->render();
    
    gaussianBlurFilter2->setSrcTextureID(gradientTextureID);
    GLuint gaussianBlurTextureID2 = gaussianBlurFilter2->render();
    
    softProcessFilter->setProcessTextureID(gaussianBlurTextureID2);
    GLuint softProcessTextureID = softProcessFilter->render();
    
    gaussianBlurFilter3->setSrcTextureID(softProcessTextureID);
    GLuint gaussianBlurTextureID3 = gaussianBlurFilter3->render();
    
    softLightFilter->setSrcTextureID(softProcessTextureID);
    softLightFilter->setOverlayTextureID(gaussianBlurTextureID3);
    return softLightFilter->render();
}

void MTFilterToonifyBackground::setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside) {
    softLightFilter->setOutsideTextureAndFbo(textureIDOutside, fboIDOutside);
}

void MTFilterToonifyBackground::setSrcTextureID(unsigned srcTextureID) {
    gaussianBlurFilter1->setSrcTextureID(srcTextureID);
    softProcessFilter->setSrcTextureID(srcTextureID);
}

ToonifyBackgroundConfig MTFilterToonifyBackground::getConfig() {
    return config;
}

void MTFilterToonifyBackground::setConfig(ToonifyBackgroundConfig config) {
    this->config = config;
}
