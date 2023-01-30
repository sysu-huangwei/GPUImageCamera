//
//  GPUImageMTSoftLightFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageMTSoftLightFilter.h"

NSString *const kGPUImageMTSoftLightVertexShaderString = SHADER_STRING
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

NSString *const kGPUImageMTSoftLightFragmentShaderString = SHADER_STRING
(
 precision highp float;
 varying highp vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;
 uniform float softLightAlpha;
 
 float SoftLight_Fcn(float A, float B)
 {
     float C = 0.0;
     if (B <= 0.5)
     {
         C = A * B / 0.5 + A * A * ( 1.0 - 2.0 * B);
     }
     else
     {
         C = A * (1.0 - B) / 0.5 + sqrt(A) * (2.0 * B - 1.0);
     }
     
     return C;
 }

 void main()
 {
     vec3 src_color = texture2D(inputImageTexture, textureCoordinate).rgb;
     vec3 overlay_color = texture2D(inputImageTexture2, textureCoordinate).rgb;
     
     vec3 res_color;
     res_color.r = SoftLight_Fcn(src_color.r, overlay_color.r);
     res_color.g = SoftLight_Fcn(src_color.g, overlay_color.g);
     res_color.b = SoftLight_Fcn(src_color.b, overlay_color.b);
     
     vec4 softLightColor = vec4(mix(src_color, res_color, softLightAlpha), 1.0);
     gl_FragColor = softLightColor;
 }
);

@implementation GPUImageMTSoftLightFilter

- (id)init {
    if ((self = [super initWithVertexShaderFromString:kGPUImageMTSoftLightVertexShaderString fragmentShaderFromString:kGPUImageMTSoftLightFragmentShaderString])) {
        softLightAlpha = 0.36f;
        runSynchronouslyOnVideoProcessingQueue(^{
            self->softLightAlphaUniform = [self->filterProgram uniformIndex:@"softLightAlpha"];
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

    glUniform1f(softLightAlphaUniform, softLightAlpha);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [firstInputFramebuffer unlock];
    
    if (usingNextFrameForImageCapture)
    {
        dispatch_semaphore_signal(imageCaptureSemaphore);
    }
}

@end
