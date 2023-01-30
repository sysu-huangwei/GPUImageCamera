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
        _camera = [[GPUImageStillCamera alloc] initWithSessionPreset:AVCaptureSessionPresetPhoto cameraPosition:AVCaptureDevicePositionFront];
        _camera.outputImageOrientation = UIInterfaceOrientationPortrait;//设置照片的方向为设备的定向
        _camera.horizontallyMirrorFrontFacingCamera = YES;//设置前置是否为镜像
    }
    return self;
}

- (void)removeAllTargets {
    [_camera removeAllTargets];
}

- (void)setFilters:(NSMutableArray<GPUImageOutput<GPUImageInput> *> *)filters {
    _filters = filters;
    for (id filter in filters) {
        [_camera addTarget:filter];
    }
//    if (_filters != nil && _filters.count > 0) {
//        [_camera addTarget:[_filters firstObject]];
//    }
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


- (void) setPreviewType:(PreviewType) previewType {
    switch (previewType) {
        case PreviewType16_9:
            [_camera setCaptureSessionPreset:AVCaptureSessionPresetiFrame960x540];
            break;
        case PreviewType4_3:
            [_camera setCaptureSessionPreset:AVCaptureSessionPresetPhoto];
        default:
            break;
    }
}

- (void) takePhotoUpToFilter:(GPUImageOutput<GPUImageInput> *)finalFilterInChain WithCompletion:(captureComlpetion) completion {
    [_camera capturePhotoAsImageProcessedUpToFilter:finalFilterInChain withCompletionHandler:^(UIImage *image, NSError *error) {
        if (completion) {
            completion(image, error);
        }
    }];
}


- (void) takeOriginPhotoWithCompletion:(captureComlpetion) completion {
    __block GPUImageFilter* filter = [[GPUImageFilter alloc] init];
    [_camera addTarget:filter];
    [_camera capturePhotoAsImageProcessedUpToFilter:filter withCompletionHandler:^(UIImage *image, NSError *error) {
        [self->_camera removeTarget:filter];
        filter = nil;
        if (completion) {
            completion(image, error);
        }
    }];
}




@end
