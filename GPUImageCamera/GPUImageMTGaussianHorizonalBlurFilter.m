//
//  GPUImageMTGaussianHorizonalBlurFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageMTGaussianHorizonalBlurFilter.h"

@implementation GPUImageMTGaussianHorizonalBlurFilter

- (id)init {
    if (self = [super initWithVertexShaderFromString:kGPUImageMTGaussianBlurVertexShaderString_Horizonal fragmentShaderFromString:kGPUImageMTGaussianBlurFragmentShaderString]) {
        
    }
    return self;
}

- (void)setupFilterForSize:(CGSize)filterFrameSize {
    runSynchronouslyOnVideoProcessingQueue(^{
        self->singleStepOffset = self.samplerInterval / filterFrameSize.width;
    });
}

@end
