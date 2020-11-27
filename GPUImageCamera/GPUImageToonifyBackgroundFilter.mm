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
#include "MTFilterToonifyBackground.hpp"

@interface GPUImageToonifyBackgroundFilter()
{
    MTFilterToonifyBackground *toonifyBackgroundFilter;
}
@end

@implementation GPUImageToonifyBackgroundFilter

-(instancetype)init {
    if (self = [super init]) {
        runSynchronouslyOnVideoProcessingQueue(^{
            GLint err = glGetError();
            self->toonifyBackgroundFilter = new MTFilterToonifyBackground();
            self->toonifyBackgroundFilter->init();
            err = glGetError();
            NSLog(@"1");
        });
    }
    return self;
}

- (void)dealloc {
    runSynchronouslyOnVideoProcessingQueue(^{
        self->toonifyBackgroundFilter->release();
    });
}

- (void)setupFilterForSize:(CGSize)filterFrameSize;
{
    runSynchronouslyOnVideoProcessingQueue(^{
        self->toonifyBackgroundFilter->resize(filterFrameSize.width, filterFrameSize.height);
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
    
    self->toonifyBackgroundFilter->setSrcTextureID(firstInputFramebuffer.texture);
    self->toonifyBackgroundFilter->setOutsideTextureAndFbo(outputFramebuffer.texture, outputFramebuffer.framebuffer);
    self->toonifyBackgroundFilter->render();
    
    
    
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
