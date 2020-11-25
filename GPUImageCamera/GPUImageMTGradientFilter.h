//
//  GPUImageMTGradientFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageFilter.h"

NS_ASSUME_NONNULL_BEGIN

@interface GPUImageMTGradientFilter : GPUImageFilter
{
    GLfloat textureWidthOffset, textureHeightOffset;
    GLint textureWidthOffsetUniform, textureHeightOffsetUniform;
}
@end

NS_ASSUME_NONNULL_END
