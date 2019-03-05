//
//  CameraController.h
//  GPUImageCamera
//
//  Created by HW on 2019/2/25.
//  Copyright © 2019 meitu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GPUImage.h>


/**
 预览比例
 */
typedef NS_ENUM (NSInteger, PreviewType) {
    PreviewType4_3,
    PreviewType16_9,
};


/**
 拍照完成的回调
 */
typedef void(^captureComlpetion)(UIImage *image, NSError *error);

NS_ASSUME_NONNULL_BEGIN

@interface CameraController : NSObject

@property (strong, nonatomic) NSMutableArray<GPUImageOutput <GPUImageInput>* >* filters; //滤镜链
@property (strong, nonatomic) id<GPUImageInput> outputFilter; //最终输出的位置


/**
 打开相机
 */
- (void) open;



/**
 停止相机
 */
- (void) stop;




/**
 切换前后置
 */
- (void) rotateCamera;



/**
 设置预览比例
 */
- (void) setPreviewType:(PreviewType) previewType;


/**
 拍照返回效果图
 */
- (void) takePhotoWithCompletion:(captureComlpetion) completion;



/**
 拍照返回原图
 */
- (void) takeOriginPhotoWithCompletion:(captureComlpetion) completion;


@end

NS_ASSUME_NONNULL_END
