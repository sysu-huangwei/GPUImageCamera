//
//  GPUImageMTGaussianBlurFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageMTGaussianBlurFilter.h"
#import "GPUImageMTGaussianHorizonalBlurFilter.h"
#import "GPUImageMTGaussianVerticalBlurFilter.h"

@implementation GPUImageMTGaussianBlurFilter

- (id)init {
    if (!(self = [super init])) {
        return nil;
    }

    GPUImageMTGaussianHorizonalBlurFilter *horizonalBlurFilter = [[GPUImageMTGaussianHorizonalBlurFilter alloc] init];
    [self addFilter:horizonalBlurFilter];
    
    GPUImageMTGaussianVerticalBlurFilter *verticalBlurFilter = [[GPUImageMTGaussianVerticalBlurFilter alloc] init];
    [self addFilter:verticalBlurFilter];
    
    [horizonalBlurFilter addTarget:verticalBlurFilter];
    
    self.initialFilters = [NSArray arrayWithObject:horizonalBlurFilter];
    self.terminalFilter = verticalBlurFilter;
    
    return self;
}

- (void)setSamplerInterval:(GLfloat)samplerInterval {
    for (id filter in filters) {
        if ([filter isKindOfClass:[GPUImageMTGaussianBaseBlurFilter class]]) {
            GPUImageMTGaussianBaseBlurFilter *baseBlurFilter = (GPUImageMTGaussianBaseBlurFilter *)filter;
            [baseBlurFilter setSamplerInterval:samplerInterval];
        }
    }
}

@end
