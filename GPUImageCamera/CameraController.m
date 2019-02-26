//
//  CameraController.m
//  GPUImageCamera
//
//  Created by HW on 2019/2/25.
//  Copyright © 2019 meitu. All rights reserved.
//

#import "CameraController.h"


@interface CameraController ()
@property (strong, nonatomic) GPUImageStillCamera* camera;

@end

@implementation CameraController


- (instancetype) init {
    self = [super init];
    if (self) {
        _camera = [[GPUImageStillCamera alloc] initWithSessionPreset:AVCaptureSessionPresetPhoto cameraPosition:AVCaptureDevicePositionBack];
        _camera.outputImageOrientation = UIInterfaceOrientationPortrait;
    }
    return self;
}

- (void)setFilters:(NSMutableArray<GPUImageOutput<GPUImageInput> *> *)filters {
    _filters = filters;
    if (_filters != nil && _filters.count > 0) {
        [_camera addTarget:[_filters firstObject]];
    }
}

- (void)setOutputFilter:(id<GPUImageInput>)outputFilter {
    _outputFilter = outputFilter;
    if (_filters != nil && _filters.count > 0) {
        [[_filters lastObject] addTarget:_outputFilter];
    }
    else {
        [_camera addTarget:_outputFilter];
    }
}

- (void) open {
    [_camera startCameraCapture];
}

- (void) stop {
    [_camera stopCameraCapture];
}

- (void) rotateCamera {
    [_camera rotateCamera];
}

- (void) takePhotoWithcompletion:(captureComlpetion) completion {
    [_camera capturePhotoAsImageProcessedUpToFilter:[_filters firstObject] withCompletionHandler:^(UIImage *processedImage, NSError *error) {
        if (completion) {
            completion(processedImage, error);
        }
    }];
}




@end
