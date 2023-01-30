//
//  GPUImageMTGaussianBaseBlurFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageFilter.h"

NS_ASSUME_NONNULL_BEGIN

extern NSString *const kGPUImageMTGaussianBlurVertexShaderString_Horizonal;
extern NSString *const kGPUImageMTGaussianBlurVertexShaderString_Vertical;
extern NSString *const kGPUImageMTGaussianBlurFragmentShaderString;

@interface GPUImageMTGaussianBaseBlurFilter : GPUImageFilter
{
    GLfloat singleStepOffset;
    GLint singleStepOffsetUniform;
}

@property (nonatomic, assign) GLfloat samplerInterval;

@end

NS_ASSUME_NONNULL_END
