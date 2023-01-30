//
//  MTFilterGaussianBlur.cpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//

#include "MTFilterGaussianBlur.hpp"

MTFilterGaussianBlur::MTFilterGaussianBlur() {
    
}

MTFilterGaussianBlur::~MTFilterGaussianBlur() {
    
}

void MTFilterGaussianBlur::init() {
    horizonalFilter.init();
    verticalFilter.init();
}

void MTFilterGaussianBlur::release() {
    horizonalFilter.release();
    verticalFilter.release();
}

void MTFilterGaussianBlur::resize(int width, int height) {
    horizonalFilter.resize(width, height);
    verticalFilter.resize(width, height);
}
    
unsigned MTFilterGaussianBlur::render() {
    unsigned horizonalTextureID = horizonalFilter.render();
    verticalFilter.setSrcTextureID(horizonalTextureID);
    return verticalFilter.render();
}

void MTFilterGaussianBlur::setOutsideTextureAndFbo(unsigned textureIDOutside, unsigned fboIDOutside) {
    verticalFilter.setOutsideTextureAndFbo(textureIDOutside, fboIDOutside);
}


void MTFilterGaussianBlur::setSrcTextureID(unsigned srcTextureID) {
    horizonalFilter.setSrcTextureID(srcTextureID);
}

void MTFilterGaussianBlur::setSamplerInterval(float samplerInterval) {
    horizonalFilter.setSamplerInterval(samplerInterval);
    verticalFilter.setSamplerInterval(samplerInterval);
}
