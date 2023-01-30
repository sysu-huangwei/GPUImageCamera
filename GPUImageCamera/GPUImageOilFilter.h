//
//  GPUImageOilFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/20.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageFilter.h"

NS_ASSUME_NONNULL_BEGIN

@interface GPUImageOilFilter : GPUImageFilter
{
    GLfloat texelWidthOffset, texelHeightOffset;
    GLint texelWidthOffsetUniform, texelHeightOffsetUniform;
}
@end

NS_ASSUME_NONNULL_END
