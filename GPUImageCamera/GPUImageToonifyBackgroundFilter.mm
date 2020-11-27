//
//  GPUImageToonifyBackgroundFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageToonifyBackgroundFilter.h"

#import "MTFilterCopy.hpp"
#include "MTFilterGaussianBlur.hpp"
#include "MTFilterGradient.hpp"
#include "MTFilterSoftProcess.hpp"
#include "MTFilterSoftLight.hpp"
#include <cmath>

@interface GPUImageToonifyBackgroundFilter()
{
    MTFilterGaussianBlur *gaussianBlurFilter1;
    MTFilterGradient *gradientFilter;
    MTFilterGaussianBlur *gaussianBlurFilter2;
    MTFilterSoftProcess *softProcessFilter;
    MTFilterGaussianBlur *gaussianBlurFilter3;
    MTFilterSoftLight *softLightFilter;
}
@end

@implementation GPUImageToonifyBackgroundFilter

-(instancetype)init {
    if (self = [super init]) {
        runSynchronouslyOnVideoProcessingQueue(^{
            GLint err = glGetError();
            self->gaussianBlurFilter1 = new MTFilterGaussianBlur();
            self->gradientFilter = new MTFilterGradient();
            self->gaussianBlurFilter2 = new MTFilterGaussianBlur();
            self->softProcessFilter = new MTFilterSoftProcess();
            self->gaussianBlurFilter3 = new MTFilterGaussianBlur();
            self->softLightFilter = new MTFilterSoftLight();
            self->gaussianBlurFilter1->init();
            self->gradientFilter->init();
            self->gaussianBlurFilter2->init();
            self->softProcessFilter->init();
            self->gaussianBlurFilter3->init();
            self->softLightFilter->init();
            err = glGetError();
            NSLog(@"1");
        });
    }
    return self;
}

- (void)dealloc {
    runSynchronouslyOnVideoProcessingQueue(^{
        self->gaussianBlurFilter1->release();
        self->gradientFilter->release();
        self->gaussianBlurFilter2->release();
        self->softProcessFilter->release();
        self->gaussianBlurFilter3->release();
        self->softLightFilter->release();
    });
}

- (void)setupFilterForSize:(CGSize)filterFrameSize;
{
    runSynchronouslyOnVideoProcessingQueue(^{
        int scaleMaxSize = 640;
        int scaleWidth, scaleHeight;
        if (filterFrameSize.height >= filterFrameSize.width) {
            scaleHeight = scaleMaxSize;
            scaleWidth = std::ceil(filterFrameSize.width * scaleHeight / filterFrameSize.height);
        } else {
            scaleWidth = scaleMaxSize;
            scaleHeight = std::ceil(filterFrameSize.height * scaleWidth / filterFrameSize.width);
        }
        self->gaussianBlurFilter1->resize(scaleWidth, scaleHeight);
        self->gradientFilter->resize(scaleWidth, scaleHeight);
        self->gaussianBlurFilter2->resize(scaleWidth, scaleHeight);
        self->softProcessFilter->resize(filterFrameSize.width, filterFrameSize.height);
        self->gaussianBlurFilter3->resize(scaleWidth, scaleHeight);
        self->softLightFilter->resize(filterFrameSize.width, filterFrameSize.height);
    });
}

- (void)renderToTextureWithVertices:(const GLfloat *)vertices textureCoordinates:(const GLfloat *)textureCoordinates;
{
    if (self.preventRendering)
    {
        [firstInputFramebuffer unlock];
        return;
    }
    
//    [GPUImageContext setActiveShaderProgram:filterProgram];

    outputFramebuffer = [[GPUImageContext sharedFramebufferCache] fetchFramebufferForSize:[self sizeOfFBO] textureOptions:self.outputTextureOptions onlyTexture:NO];
    [outputFramebuffer activateFramebuffer];
    if (usingNextFrameForImageCapture)
    {
        [outputFramebuffer lock];
    }

    GLint err = glGetError();
    
    gaussianBlurFilter1->setSamplerInterval(0.9f);
    gaussianBlurFilter1->setSrcTextureID(firstInputFramebuffer.texture);
    GLuint gaussianBlurTextureID1 = gaussianBlurFilter1->render();
    
    gradientFilter->setSrcTextureID(gaussianBlurTextureID1);
    GLuint gradientTextureID = gradientFilter->render();
    
    gaussianBlurFilter2->setSamplerInterval(2.8f);
    gaussianBlurFilter2->setSrcTextureID(gradientTextureID);
    GLuint gaussianBlurTextureID2 = gaussianBlurFilter2->render();
    
    softProcessFilter->setSamplerInterval(0.8f);
    softProcessFilter->setSrcTextureID(firstInputFramebuffer.texture);
    softProcessFilter->setProcessTextureID(gaussianBlurTextureID2);
    GLuint softProcessTextureID = softProcessFilter->render();
    
    gaussianBlurFilter3->setSamplerInterval(1.5f);
    gaussianBlurFilter3->setSrcTextureID(softProcessTextureID);
    GLuint gaussianBlurTextureID3 = gaussianBlurFilter3->render();
    
    softLightFilter->setSrcTextureID(softProcessTextureID);
    softLightFilter->setOverlayTextureID(gaussianBlurTextureID3);
    softLightFilter->setOutsideTextureAndFbo(outputFramebuffer.texture, outputFramebuffer.framebuffer);
    softLightFilter->render();
    
    
    
//    [self setUniformsForProgramAtIndex:0];
//
//    glClearColor(backgroundColorRed, backgroundColorGreen, backgroundColorBlue, backgroundColorAlpha);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, [firstInputFramebuffer texture]);
//
//    glUniform1i(filterInputTextureUniform, 2);
//
//    glVertexAttribPointer(filterPositionAttribute, 2, GL_FLOAT, 0, 0, vertices);
//    glVertexAttribPointer(filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, textureCoordinates);
//
//
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [firstInputFramebuffer unlock];
    
    if (usingNextFrameForImageCapture)
    {
        dispatch_semaphore_signal(imageCaptureSemaphore);
    }
}

@end
