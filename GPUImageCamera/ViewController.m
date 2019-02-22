//
//  ViewController.m
//  GPUImageCamera
//
//  Created by HW on 2019/2/22.
//  Copyright © 2019 meitu. All rights reserved.
//

#import "ViewController.h"
#import <GPUImage.h>

@interface ViewController ()
@property (strong, nonatomic) GPUImageStillCamera* mCamera;
@property (strong, nonatomic) GPUImageFilter* mFilter;
@property (strong, nonatomic) GPUImageView* mGPUImageView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    _mCamera = [[GPUImageStillCamera alloc] initWithSessionPreset:AVCaptureSessionPresetPhoto cameraPosition:AVCaptureDevicePositionBack];
    _mCamera.outputImageOrientation = UIInterfaceOrientationPortrait;
    
    _mFilter = [[GPUImageFilter alloc] init];
    
    _mGPUImageView = [[GPUImageView alloc] initWithFrame:self.view.bounds];
    
    [self.view addSubview:_mGPUImageView];
    
    [_mCamera addTarget:_mFilter];
    [_mFilter addTarget:_mGPUImageView];
    
    [_mCamera startCameraCapture];
    
}


@end
