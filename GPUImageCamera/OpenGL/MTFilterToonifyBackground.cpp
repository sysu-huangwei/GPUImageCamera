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
#include "MTFilterGaussianBlur.hpp"
#include "MTFilterGradient.hpp"
#include "MTFilterSoftProcess.hpp"
#include "MTFilterSoftLight.hpp"

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
    delete (MTFilterGaussianBlur *)gaussianBlurFilter1;
    delete (MTFilterGradient *)gradientFilter;
    delete (MTFilterGaussianBlur *)gaussianBlurFilter2;
    delete (MTFilterSoftProcess *)softProcessFilter;
    delete (MTFilterGaussianBlur *)gaussianBlurFilter3;
    delete (MTFilterSoftLight *)softLightFilter;
}

void MTFilterToonifyBackground::init() {
    ((MTFilterGaussianBlur *)gaussianBlurFilter1)->setSamplerInterval(config.gradNoiseSamplerInterval);
    ((MTFilterGaussianBlur *)gaussianBlurFilter2)->setSamplerInterval(config.gradBlurSamplerInterval);
    ((MTFilterSoftProcess *)softProcessFilter)->setRefResolution(config.refResolution);
    ((MTFilterSoftProcess *)softProcessFilter)->setSamplerInterval(config.samplerInterval);
    ((MTFilterSoftProcess *)softProcessFilter)->setAlpha(config.softAlpha);
    ((MTFilterGaussianBlur *)gaussianBlurFilter3)->setSamplerInterval(1.5f);
    ((MTFilterSoftLight *)softLightFilter)->setSoftLightAlpha(config.softAlpha);
    
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
    
    ((MTFilterGradient *)gradientFilter)->setSrcTextureID(gaussianBlurTextureID1);
    GLuint gradientTextureID = gradientFilter->render();
    
    ((MTFilterGaussianBlur *)gaussianBlurFilter2)->setSrcTextureID(gradientTextureID);
    GLuint gaussianBlurTextureID2 = gaussianBlurFilter2->render();
    
    ((MTFilterSoftProcess *)softProcessFilter)->setProcessTextureID(gaussianBlurTextureID2);
    GLuint softProcessTextureID = softProcessFilter->render();
    
    ((MTFilterGaussianBlur *)gaussianBlurFilter3)->setSrcTextureID(softProcessTextureID);
    GLuint gaussianBlurTextureID3 = gaussianBlurFilter3->render();
    
    ((MTFilterSoftLight *)softLightFilter)->setSrcTextureID(softProcessTextureID);
    ((MTFilterSoftLight *)softLightFilter)->setOverlayTextureID(gaussianBlurTextureID3);
    return softLightFilter->render();
}

void MTFilterToonifyBackground::setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside) {
    softLightFilter->setOutsideTextureAndFbo(textureIDOutside, fboIDOutside);
}

void MTFilterToonifyBackground::setSrcTextureID(unsigned srcTextureID) {
    ((MTFilterGaussianBlur *)gaussianBlurFilter1)->setSrcTextureID(srcTextureID);
    ((MTFilterSoftProcess *)softProcessFilter)->setSrcTextureID(srcTextureID);
}

ToonifyBackgroundConfig MTFilterToonifyBackground::getConfig() {
    return config;
}

void MTFilterToonifyBackground::setConfig(ToonifyBackgroundConfig config) {
    this->config = config;
}
