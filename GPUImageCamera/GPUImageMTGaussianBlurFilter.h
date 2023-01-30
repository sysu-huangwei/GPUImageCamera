//
//  GPUImageMTGaussianBlurFilter.h
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageFilterGroup.h"

NS_ASSUME_NONNULL_BEGIN

@interface GPUImageMTGaussianBlurFilter : GPUImageFilterGroup

@property (nonatomic, assign) GLfloat samplerInterval;

@end

NS_ASSUME_NONNULL_END
