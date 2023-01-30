//
//  GPUImageMTGaussianBaseBlurFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageMTGaussianBaseBlurFilter.h"


NSString *const kGPUImageMTGaussianBlurVertexShaderString_Horizonal = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 varying vec2 textureCoordinate;
 varying vec4 v_texcoordOffset[4];
 
 uniform float u_singleStepOffset;
 
 void main()
 {
    gl_Position = position;
    textureCoordinate = inputTextureCoordinate.xy;
    v_texcoordOffset[0] = vec4(textureCoordinate + vec2(-4.0*u_singleStepOffset,0.0),textureCoordinate + vec2(1.0*u_singleStepOffset,0.0));
    v_texcoordOffset[1] = vec4(textureCoordinate + vec2(-3.0*u_singleStepOffset,0.0),textureCoordinate + vec2(2.0*u_singleStepOffset,0.0));
    v_texcoordOffset[2] = vec4(textureCoordinate + vec2(-2.0*u_singleStepOffset,0.0),textureCoordinate + vec2(3.0*u_singleStepOffset,0.0));
    v_texcoordOffset[3] = vec4(textureCoordinate + vec2(-1.0*u_singleStepOffset,0.0),textureCoordinate + vec2(4.0*u_singleStepOffset,0.0));
 }
 );

NSString *const kGPUImageMTGaussianBlurVertexShaderString_Vertical = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 varying vec2 textureCoordinate;
 varying vec4 v_texcoordOffset[4];
 
 uniform float u_singleStepOffset;
 
 void main()
 {
    gl_Position = position;
    textureCoordinate = inputTextureCoordinate.xy;
    v_texcoordOffset[0] = vec4(textureCoordinate + vec2(0.0,-4.0*u_singleStepOffset),textureCoordinate + vec2(0.0,1.0*u_singleStepOffset));
    v_texcoordOffset[1] = vec4(textureCoordinate + vec2(0.0,-3.0*u_singleStepOffset),textureCoordinate + vec2(0.0,2.0*u_singleStepOffset));
    v_texcoordOffset[2] = vec4(textureCoordinate + vec2(0.0,-2.0*u_singleStepOffset),textureCoordinate + vec2(0.0,3.0*u_singleStepOffset));
    v_texcoordOffset[3] = vec4(textureCoordinate + vec2(0.0,-1.0*u_singleStepOffset),textureCoordinate + vec2(0.0,4.0*u_singleStepOffset));
 }
 );

NSString *const kGPUImageMTGaussianBlurFragmentShaderString = SHADER_STRING
(
precision highp float;

varying vec2 textureCoordinate;
varying vec4 v_texcoordOffset[4];

uniform sampler2D inputImageTexture;

vec4 gauss() {
    vec4 sum = vec4(0.0);
    //9x9
    sum += texture2D(inputImageTexture, v_texcoordOffset[0].rg) * 0.05;
    sum += texture2D(inputImageTexture, v_texcoordOffset[1].rg) * 0.09;
    sum += texture2D(inputImageTexture, v_texcoordOffset[2].rg) * 0.12;
    sum += texture2D(inputImageTexture, v_texcoordOffset[3].rg) * 0.15;
    sum += texture2D(inputImageTexture, textureCoordinate) * 0.18;
    sum += texture2D(inputImageTexture, v_texcoordOffset[0].ba) * 0.15;
    sum += texture2D(inputImageTexture, v_texcoordOffset[1].ba) * 0.12;
    sum += texture2D(inputImageTexture, v_texcoordOffset[2].ba) * 0.09;
    sum += texture2D(inputImageTexture, v_texcoordOffset[3].ba) * 0.05;
    return sum;
}
 
void main()
{
    gl_FragColor = gauss();
}
);


@implementation GPUImageMTGaussianBaseBlurFilter

- (id)initWithVertexShaderFromString:(NSString *)vertexShaderString fragmentShaderFromString:(NSString *)fragmentShaderString {
    if (self = [super initWithVertexShaderFromString:vertexShaderString fragmentShaderFromString:fragmentShaderString]) {
        _samplerInterval = 1.0f;
        singleStepOffset = 1.0f;
        runSynchronouslyOnVideoProcessingQueue(^{
            self->singleStepOffsetUniform = [self->filterProgram uniformIndex:@"u_singleStepOffset"];
        });
    }
    return self;
}

- (void)renderToTextureWithVertices:(const GLfloat *)vertices textureCoordinates:(const GLfloat *)textureCoordinates;
{
    if (self.preventRendering)
    {
        [firstInputFramebuffer unlock];
        return;
    }
    
    [GPUImageContext setActiveShaderProgram:filterProgram];

    outputFramebuffer = [[GPUImageContext sharedFramebufferCache] fetchFramebufferForSize:[self sizeOfFBO] textureOptions:self.outputTextureOptions onlyTexture:NO];
    [outputFramebuffer activateFramebuffer];
    if (usingNextFrameForImageCapture)
    {
        [outputFramebuffer lock];
    }

    [self setUniformsForProgramAtIndex:0];
    
    glClearColor(backgroundColorRed, backgroundColorGreen, backgroundColorBlue, backgroundColorAlpha);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, [firstInputFramebuffer texture]);
    
    glUniform1i(filterInputTextureUniform, 2);

    glVertexAttribPointer(filterPositionAttribute, 2, GL_FLOAT, 0, 0, vertices);
    glVertexAttribPointer(filterTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, textureCoordinates);
    
    glUniform1f(singleStepOffsetUniform, singleStepOffset);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [firstInputFramebuffer unlock];
    
    if (usingNextFrameForImageCapture)
    {
        dispatch_semaphore_signal(imageCaptureSemaphore);
    }
}

@end
