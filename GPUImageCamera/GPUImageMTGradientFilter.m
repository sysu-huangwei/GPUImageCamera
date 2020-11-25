//
//  GPUImageMTGradientFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageMTGradientFilter.h"

NSString *const kGPUImageMTGradientVertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 varying vec2 textureCoordinate;
 
 void main()
 {
     gl_Position = position;
     textureCoordinate = inputTextureCoordinate.xy;
 }
 );

NSString *const kGPUImageMTGradientFragmentShaderString = SHADER_STRING
(
 precision highp float;
 varying highp vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform float textureWidthOffset;
 uniform float textureHeightOffset;
 
 void main()
 {
     vec2 shiftingSize = vec2(textureWidthOffset, textureHeightOffset);

     vec3 color00 = texture2D(inputImageTexture, textureCoordinate).rgb;
     vec3 color01 = texture2D(inputImageTexture, textureCoordinate + vec2(shiftingSize.x, 0.0)).rgb;
     vec3 color10 = texture2D(inputImageTexture, textureCoordinate + vec2(0.0, shiftingSize.y)).rgb;
     vec3 color11 = texture2D(inputImageTexture, textureCoordinate + shiftingSize).rgb;

     vec3 colorX = color01 + color11 - color00 - color10;
     vec3 colorY = color10 + color11 - color00 - color01;

     float grayX = dot(colorX, vec3(0.298912, 0.586611, 0.114478));
     float grayY = dot(colorY, vec3(0.298912, 0.586611, 0.114478));

     vec2 grad = vec2(grayX, grayY) * 0.5;
     vec2 grad2 = grad * grad;
     float gradLen2 = grad2.x + grad2.y;

     vec2 gradDouble = gradLen2 != 0.0 ? vec2(grad2.x - grad2.y, 2.0 * grad.x * grad.y) / gradLen2 : vec2(0.0);
     gl_FragColor = vec4(gradDouble * 0.5 + 0.5, 0.0, 1.0);
 }
);

@implementation GPUImageMTGradientFilter

- (id)init {
    if ((self = [super initWithVertexShaderFromString:kGPUImageMTGradientVertexShaderString fragmentShaderFromString:kGPUImageMTGradientFragmentShaderString])) {
        runSynchronouslyOnVideoProcessingQueue(^{
            self->textureWidthOffsetUniform = [self->filterProgram uniformIndex:@"textureWidthOffset"];
            self->textureHeightOffsetUniform = [self->filterProgram uniformIndex:@"textureHeightOffset"];
        });
    }
    return self;
}

- (void)setupFilterForSize:(CGSize)filterFrameSize {
    runSynchronouslyOnVideoProcessingQueue(^{
        self->textureWidthOffset = 1.0f / filterFrameSize.width;
        self->textureHeightOffset = 1.0f / filterFrameSize.height;
    });
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
    
    glUniform1f(textureWidthOffsetUniform, textureWidthOffset);
    glUniform1f(textureHeightOffsetUniform, textureHeightOffset);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [firstInputFramebuffer unlock];
    
    if (usingNextFrameForImageCapture)
    {
        dispatch_semaphore_signal(imageCaptureSemaphore);
    }
}

@end
