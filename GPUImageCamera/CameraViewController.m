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
@property (strong, nonatomic) IBOutlet UIButton *saveResultImageButton;
@property (strong, nonatomic) CameraController* cameraController;  //相机控制器
@property (strong, nonatomic) NSMutableArray* filters; //当前的滤镜链
@property (strong, nonatomic) GPUImageView* imageView;
@property (assign) CurrentState currentState; //当前处于预览还是拍后
@property (assign) CGRect showView4_3;
@property (assign) CGRect showView16_9;
@property (strong, nonatomic) UIImage* originImage; //原图
@property (strong, nonatomic) UIImage* resultImage; //结果图
@end

@implementation CameraViewController


- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    _currentState = CurrentStateTakingPhoto;
    [_saveResultImageButton setHidden:YES];
    
    _filters = [[NSMutableArray alloc] initWithObjects:[[GPUImageBilateralFilter alloc] init], nil];
    
    //初始化相机控制器
    _cameraController = [[CameraController alloc] init];
    
    //默认拍照比例4:3
    CGRect showViewFrame = _showView.frame;
    _showView4_3 = CGRectMake(showViewFrame.origin.x, showViewFrame.origin.y, showViewFrame.size.width, showViewFrame.size.width * 4.0 / 3.0);
    _showView16_9 = CGRectMake(showViewFrame.origin.x, showViewFrame.origin.y, showViewFrame.size.width, showViewFrame.size.width * 16.0 / 9.0);
    [_showView setFrame:_showView4_3];
    
    //用于展示预览的界面
    _imageView = [[GPUImageView alloc] initWithFrame:_showView.bounds];
    [self.showView addSubview:_imageView];
    
    //给相机控制器设置滤镜链
    [_cameraController setFilters:_filters];
    
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
        [_cameraController takeOriginPhotoWithCompletion:^(UIImage *image, NSError *error) {
            self->_originImage = image;
            self->_currentState = CurrentStateAfterTaking;
            [self->_saveResultImageButton setHidden:NO];
            [self->_cameraController stop];
            GPUImagePicture* picture = [[GPUImagePicture alloc] initWithImage:image];
            [picture addTarget:[self->_filters firstObject]];
            [picture useNextFrameForImageCapture];
            [picture processImageUpToFilter:[self->_filters firstObject] withCompletionHandler:^(UIImage *processedImage) {
                self->_resultImage = processedImage;
            }];
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
        [self->_saveResultImageButton setHidden:YES];
        [_cameraController open];
    }
    
}



/**
 切换前后置
 */
- (IBAction)rotateCamera:(id)sender {
    [_cameraController rotateCamera];
}



/**
 切换比例到4:3
 */
- (IBAction)changePreview4_3:(id)sender {
    [_cameraController setPreviewType:PreviewType4_3];
    [_showView setFrame:_showView4_3];
    [_imageView setFrame:_showView.bounds];
}


/**
 切换比例到16:9
 */
- (IBAction)changePreview16_9:(id)sender {
    [_cameraController setPreviewType:PreviewType16_9];
    [_showView setFrame:_showView16_9];
    [_imageView setFrame:_showView.bounds];
}


/**
 保存结果图到相册
 */
- (IBAction)saveResultImage:(id)sender {
    if (_resultImage) {
        UIImageWriteToSavedPhotosAlbum(_resultImage, nil, nil, nil);
    }
}


@end
