//
//  CameraController.h
//  GPUImageCamera
//
//  Created by HW on 2019/2/25.
//  Copyright © 2019 meitu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GPUImage.h>

typedef void(^captureComlpetion)(UIImage *processedImage, NSError *error);

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
 拍照返回效果图
 */
- (void) takePhotoWithCompletion:(captureComlpetion) completion;



/**
 拍照返回原图
 */
- (void) takeOriginPhotoWithCompletion:(captureComlpetion) completion;


@end

NS_ASSUME_NONNULL_END
