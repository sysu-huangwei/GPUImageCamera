//
//  GPUImageToonifyBackgroundFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#if __cplusplus
extern "C" {
#endif
#import "GPUImage.h"
#if __cplusplus
}
#endif

#import "GPUImageFilter.h"

NS_ASSUME_NONNULL_BEGIN

@interface GPUImageToonifyBackgroundFilter : GPUImageFilter
@property (nonatomic, assign) float gradNoiseSamplerInterval;
@property (nonatomic, assign) float gradBlurSamplerInterval;
@property (nonatomic, assign) float samplerInterval;
@property (nonatomic, assign) float softAlpha;
@end

NS_ASSUME_NONNULL_END
