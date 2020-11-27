//
//  CameraViewController.m
//  GPUImageCamera
//
//  Created by HW on 2019/2/25.
//  Copyright © 2019 meitu. All rights reserved.
//

#import "CameraViewController.h"
#import "CameraController.h"
#import "GPUImageOilFilter.h"
#import "GPUImageMTGaussianBlurFilter.h"
#import "GPUImageMTGradientFilter.h"
#import "GPUImageMTSoftProcessFilter.h"
#import "GPUImageMTSoftLightFilter.h"
#import "GPUImageToonifyBackgroundFilter.h"

typedef NS_ENUM (NSInteger, CurrentState) {
    CurrentStateTakingPhoto,//预览
    CurrentStateAfterTaking,//拍照
};

@interface CameraViewController ()
@property (strong, nonatomic) IBOutlet UIView *showView; //用于展示的view，下面的imageView会在这个view里
@property (strong, nonatomic) IBOutlet UIButton *saveResultImageButton;
@property (strong, nonatomic) IBOutlet UIButton *saveOriginImageButton;
@property (strong, nonatomic) CameraController* cameraController;  //相机控制器
@property (strong, nonatomic) IBOutlet UISlider *blurSlider1;
@property (strong, nonatomic) IBOutlet UISlider *blurSlider2;
@property (strong, nonatomic) IBOutlet UISlider *blurSlider3;
@property (strong, nonatomic) IBOutlet UISlider *softSlider;
@property (strong, nonatomic) NSMutableArray* filters; //当前的滤镜链
@property (strong, nonatomic) GPUImageView* imageView;
@property (assign) CurrentState currentState; //当前处于预览还是拍后
@property (assign) CGRect showView4_3;
@property (assign) CGRect showView16_9;
@property (strong, nonatomic) UIImage* originImage; //原图
@property (strong, nonatomic) UIImage* resultImage; //结果图
@property (assign, nonatomic) BOOL showOrigin;
//@property (strong, nonatomic) GPUImagePicture *picture;
@property (strong, nonatomic) GPUImageOutput <GPUImageInput> *filter;
@property (strong, nonatomic) GPUImagePicture *lutPicture;
@property (strong, nonatomic) GPUImageLookupFilter *lutFilter;
@property (strong, nonatomic) GPUImageToonifyBackgroundFilter *toonifyBackgroundFilter;
@end

@implementation CameraViewController


- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    _currentState = CurrentStateTakingPhoto;
    
    NSString *lutFile = [NSBundle.mainBundle.bundlePath stringByAppendingPathComponent:@"lut.png"];
    UIImage *lutImage = [[UIImage alloc] initWithContentsOfFile:lutFile];
    _lutPicture = [[GPUImagePicture alloc] initWithImage:lutImage];
    _lutFilter = [[GPUImageLookupFilter alloc] init];
    
    NSString *file = [NSBundle.mainBundle.bundlePath stringByAppendingPathComponent:@"leiyi720x1280.png"];
    _originImage = [[UIImage alloc] initWithContentsOfFile:file];
//    _picture = [[GPUImagePicture alloc] initWithImage:_originImage];
    
    _filter = [[GPUImageFilter alloc] init];
    _toonifyBackgroundFilter = [[GPUImageToonifyBackgroundFilter alloc] init];
    
    
//    GPUImageMTGaussianBlurFilter* f = (GPUImageMTGaussianBlurFilter*)_filter;
//    [f setSamplerInterval:3.0f];
    
//    _filters = [[NSMutableArray alloc] initWithObjects:filter, nil];
    
    //初始化相机控制器
    _cameraController = [[CameraController alloc] init];
    
    //默认拍照比例4:3
//    CGRect showViewFrame = _showView.frame;
//    _showView4_3 = CGRectMake(showViewFrame.origin.x, showViewFrame.origin.y, showViewFrame.size.width, showViewFrame.size.width * 4.0 / 3.0);
//    _showView16_9 = CGRectMake(showViewFrame.origin.x, showViewFrame.origin.y, showViewFrame.size.width, showViewFrame.size.width * 16.0 / 9.0);
//    [_showView setFrame:_showView4_3];
    
    //用于展示预览的界面
    _imageView = [[GPUImageView alloc] initWithFrame:_showView.bounds];
    [self.showView addSubview:_imageView];
    
//    [_picture addTarget:_filter];
//    [_lutPicture addTarget:_filter];
//    [_filter addTarget:_imageView];
    
//    [_lutPicture useNextFrameForImageCapture];
//    [_lutPicture processImage];
    
//    [_picture useNextFrameForImageCapture];
//    [_picture processImageUpToFilter:_softLightFilter withCompletionHandler:^(UIImage *processedImage) {
//        NSLog(@"1");
//    }];
    
    //给相机控制器设置滤镜链
//    [_cameraController setFilters:@[_blurFilter1, _softProcessFilter]];
    [_cameraController setFilters:@[_toonifyBackgroundFilter]];
//    [_picture addTarget:_toonifyBackgroundFilter];
    [_toonifyBackgroundFilter addTarget:_imageView];
    
//    [_picture useNextFrameForImageCapture];
//    [_picture processImageUpToFilter:_toonifyBackgroundFilter withCompletionHandler:^(UIImage *processedImage) {
//        NSLog(@"1");
//    }];
    

    //    [_picture addTarget:_blurFilter1];
    
    //给相机控制器设置输出界面
//    [_cameraController setOutputFilter:_imageView];
    
    //打开相机
    [_cameraController open];
}


- (void)viewDidLoad {
    [super viewDidLoad];
    [_saveResultImageButton setHidden:YES];
    [_saveOriginImageButton setHidden:YES];
    _showOrigin = NO;
}


/**
 拍照
 */
- (IBAction)takePhoto:(id)sender {
    [_cameraController stop];
    [_cameraController removeAllTargets];
    [_filter removeAllTargets];
    [_toonifyBackgroundFilter removeAllTargets];
    _showOrigin = !_showOrigin;
    if (_showOrigin) {
        [_cameraController setFilters:@[_filter]];
        [_filter addTarget:_imageView];
    } else {
        [_cameraController setFilters:@[_toonifyBackgroundFilter]];
        [_toonifyBackgroundFilter addTarget:_imageView];
    }
    [_cameraController open];
////    [_picture useNextFrameForImageCapture];
//    if (!_showOrigin) {
//        [_picture processImageUpToFilter:_softLightFilter withCompletionHandler:^(UIImage *processedImage) {
//            NSLog(@"1");
//        }];
//    } else {
//        [_picture processImage];
//    }
//    if (_currentState == CurrentStateTakingPhoto) {
//        [_cameraController takeOriginPhotoWithCompletion:^(UIImage *image, NSError *error) {
//            self->_originImage = image;
//            self->_currentState = CurrentStateAfterTaking;
//            [self->_saveResultImageButton setHidden:NO];
//            [self->_saveOriginImageButton setHidden:NO];
//            [self->_cameraController stop];
//            GPUImagePicture* picture = [[GPUImagePicture alloc] initWithImage:image];
//            [picture addTarget:[self->_filters firstObject]];
//            [picture useNextFrameForImageCapture];
//            [picture processImageUpToFilter:[self->_filters firstObject] withCompletionHandler:^(UIImage *processedImage) {
//                self->_resultImage = processedImage;
//            }];
//            [picture removeAllTargets];
//            picture = nil;
//        }];
//    }
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
        [self->_saveOriginImageButton setHidden:YES];
//        [_cameraController open];
    }
    
}



/**
 切换前后置
 */
- (IBAction)rotateCamera:(id)sender {
//    [_cameraController rotateCamera];
}



/**
 切换比例到4:3
 */
- (IBAction)changePreview4_3:(id)sender {
//    [_cameraController setPreviewType:PreviewType4_3];
    [_showView setFrame:_showView4_3];
    [_imageView setFrame:_showView.bounds];
}


/**
 切换比例到16:9
 */
- (IBAction)changePreview16_9:(id)sender {
//    [_cameraController setPreviewType:PreviewType16_9];
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


/**
 保存原图到相册
 */
- (IBAction)saveOriginImage:(id)sender {
    if (_originImage) {
        UIImageWriteToSavedPhotosAlbum(_originImage, nil, nil, nil);
    }
}

- (IBAction)blurSliderChange1:(id)sender {
    NSLog(@"11111");
    if ([sender isKindOfClass:[UISlider class]]) {
        UISlider *slider = (UISlider *)sender;
        float realValue = slider.value * 2.0f;
//        [_blurFilter1 setSamplerInterval:realValue];
//        [_picture processImage];
    }
}

- (IBAction)blurSliderChange2:(id)sender {
    NSLog(@"22222");
    if ([sender isKindOfClass:[UISlider class]]) {
        UISlider *slider = (UISlider *)sender;
        float realValue = slider.value * 6.0f;
//        [_blurFilter2 setSamplerInterval:realValue];
//        [_picture processImage];
    }
}

- (IBAction)blurSliderChange3:(id)sender {
    if ([sender isKindOfClass:[UISlider class]]) {
        UISlider *slider = (UISlider *)sender;
        float realValue = slider.value * 2.0f;
//        [_blurFilter3 setSamplerInterval:realValue];
//        [_picture processImage];
    }
    NSLog(@"33333");
}

- (IBAction)softSliderChange:(id)sender {
    NSLog(@"44444");
    if ([sender isKindOfClass:[UISlider class]]) {
        UISlider *slider = (UISlider *)sender;
        float realValue = slider.value * 1.0f;
//        [_softProcessFilter setAlpha:realValue];
//        [_softProcessFilter setSamplerInterval:realValue];
//        [_picture processImage];
    }
}



@end
