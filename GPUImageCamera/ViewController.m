//
//  ViewController.m
//  GPUImageCamera
//
//  Created by HW on 2019/2/22.
//  Copyright © 2019 meitu. All rights reserved.
//

#import "ViewController.h"
#import "CameraViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (IBAction)openCamera:(id)sender {
    CameraViewController* cameraViewController = [[CameraViewController alloc] init];
    [self presentViewController:cameraViewController animated:YES completion:nil];
}


@end
