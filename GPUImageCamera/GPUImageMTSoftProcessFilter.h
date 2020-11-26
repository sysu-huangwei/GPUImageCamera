//
//  GPUImageMTSoftProcessFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageTwoInputFilter.h"

NS_ASSUME_NONNULL_BEGIN

@interface GPUImageMTSoftProcessFilter : GPUImageTwoInputFilter
{
    GLfloat textureWidthOffset, textureHeightOffset;
    GLint textureWidthOffsetUniform, textureHeightOffsetUniform, alphaUniform;
    GLfloat kernel[17];
    GLint kernelUniform;
}

@property (nonatomic, assign) GLfloat samplerInterval;
@property (nonatomic, assign) GLfloat alpha;

@end

NS_ASSUME_NONNULL_END
