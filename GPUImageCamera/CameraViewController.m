//
//  CameraViewController.m
//  GPUImageCamera
//
//  Created by HW on 2019/2/25.
//  Copyright © 2019 meitu. All rights reserved.
//

#import "CameraViewController.h"
#import "CameraController.h"

typedef NS_ENUM (NSInteger, CurrentState) {
    CurrentStateTakingPhoto,//预览
    CurrentStateAfterTaking,//拍照
};

@interface CameraViewController ()
@property (strong, nonatomic) IBOutlet UIView *showView; //用于展示的view，下面的imageView会在这个view里
@property (strong, nonatomic) CameraController* cameraController;  //相机控制器
@property (strong, nonatomic) GPUImageView* imageView;
@property (assign) CurrentState currentState;
@end

@implementation CameraViewController


- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    _currentState = CurrentStateTakingPhoto;
    
    //初始化相机控制器
    _cameraController = [[CameraController alloc] init];
    
    //默认拍照比例4:3
    CGRect showViewFrame = _showView.frame;
    [_showView setFrame:CGRectMake(showViewFrame.origin.x, showViewFrame.origin.y, showViewFrame.size.width, showViewFrame.size.width * 4.0 / 3.0)];
    
    //用于展示预览的界面
    _imageView = [[GPUImageView alloc] initWithFrame:_showView.bounds];
    [self.showView addSubview:_imageView];
    
    [_cameraController setFilters:@[[[GPUImageSwirlFilter alloc] init]]];
    
    //给相机控制器设置输出界面
    [_cameraController setOutputFilter:_imageView];
    
    //打开相机
    [_cameraController open];
}


- (void)viewDidLoad {
    [super viewDidLoad];
}


/**
 拍照
 */
- (IBAction)takePhoto:(id)sender {
    if (_currentState == CurrentStateTakingPhoto) {
        [_cameraController takePhotoWithcompletion:^(UIImage *processedImage, NSError *error) {
            self->_currentState = CurrentStateAfterTaking;
            [self->_cameraController stop];
            GPUImagePicture* picture = [[GPUImagePicture alloc] initWithImage:processedImage];
            [picture addTarget:self->_imageView];
            [picture useNextFrameForImageCapture];
            [picture processImage];
            [picture removeAllTargets];
            picture = nil;
        }];
    }
}



/**
 返回上级界面
 */
- (IBAction)back:(id)sender {
    if (_currentState == CurrentStateTakingPhoto) {
        [self dismissViewControllerAnimated:YES completion:nil];
    }
    else {
        _currentState = CurrentStateTakingPhoto;
        [_cameraController open];
    }
    
}

@end
