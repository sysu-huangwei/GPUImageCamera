//
//  GPUImageMTSoftLightFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageTwoInputFilter.h"

NS_ASSUME_NONNULL_BEGIN

@interface GPUImageMTSoftLightFilter : GPUImageTwoInputFilter
{
    GLfloat softLightAlpha;
    GLint softLightAlphaUniform;
}
@end

NS_ASSUME_NONNULL_END
