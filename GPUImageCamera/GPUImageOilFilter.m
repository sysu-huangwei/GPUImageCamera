//
//  GPUImageOilFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/20.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageOilFilter.h"

NSString *const kGPUImageOilVertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 varying vec2 textureCoordinate;
 varying vec2 nearCoordinatesVarying[9];
 
 uniform float texelWidthOffset;
 uniform float texelHeightOffset;
 varying vec2 texelOffset;
 
 void main()
 {
    gl_Position = position;
    textureCoordinate = inputTextureCoordinate.xy;
    texelOffset = vec2(texelWidthOffset, texelHeightOffset);
    nearCoordinatesVarying[0] = inputTextureCoordinate.xy + vec2(0, texelHeightOffset);
    nearCoordinatesVarying[1] = vec2(inputTextureCoordinate.x, inputTextureCoordinate.y + texelHeightOffset);
    nearCoordinatesVarying[2] = vec2(inputTextureCoordinate.x, inputTextureCoordinate.y - texelHeightOffset);
    nearCoordinatesVarying[3] = vec2(inputTextureCoordinate.x + texelWidthOffset, inputTextureCoordinate.y);
    nearCoordinatesVarying[4] = vec2(inputTextureCoordinate.x - texelWidthOffset, inputTextureCoordinate.y);
    nearCoordinatesVarying[5] = vec2(inputTextureCoordinate.x + texelWidthOffset, inputTextureCoordinate.y + texelHeightOffset);
    nearCoordinatesVarying[6] = vec2(inputTextureCoordinate.x + texelWidthOffset, inputTextureCoordinate.y - texelHeightOffset);
    nearCoordinatesVarying[7] = vec2(inputTextureCoordinate.x - texelWidthOffset, inputTextureCoordinate.y + texelHeightOffset);
    nearCoordinatesVarying[8] = vec2(inputTextureCoordinate.x - texelWidthOffset, inputTextureCoordinate.y - texelHeightOffset);
 }
 );


NSString *const kGPUImageOilFragment55ShaderString = SHADER_STRING
(
precision highp float;
precision highp int;
varying highp vec2 textureCoordinate;
varying vec2 nearCoordinatesVarying[9];
varying vec2 texelOffset;

uniform sampler2D inputImageTexture;

void main()
{
    vec2 nearCoordinates[25];
    for (int i = 0; i < 9; i++) {
        nearCoordinates[i] = nearCoordinatesVarying[i];
    }
    
    nearCoordinates[9] = vec2(textureCoordinate.x - 2.0 * texelOffset.x, textureCoordinate.y - 2.0 * texelOffset.y);
    nearCoordinates[10] = vec2(textureCoordinate.x - texelOffset.x, textureCoordinate.y - 2.0 * texelOffset.y);
    nearCoordinates[11] = vec2(textureCoordinate.x, textureCoordinate.y - 2.0 * texelOffset.y);
    nearCoordinates[12] = vec2(textureCoordinate.x + texelOffset.x, textureCoordinate.y - 2.0 * texelOffset.y);
    nearCoordinates[13] = vec2(textureCoordinate.x + 2.0 * texelOffset.x, textureCoordinate.y - 2.0 * texelOffset.y);
    nearCoordinates[14] = vec2(textureCoordinate.x - 2.0 * texelOffset.x, textureCoordinate.y - texelOffset.y);
    nearCoordinates[15] = vec2(textureCoordinate.x + 2.0 * texelOffset.x, textureCoordinate.y - texelOffset.y);
    nearCoordinates[16] = vec2(textureCoordinate.x - 2.0 * texelOffset.x, textureCoordinate.y);
    nearCoordinates[17] = vec2(textureCoordinate.x + 2.0 * texelOffset.x, textureCoordinate.y);
    nearCoordinates[18] = vec2(textureCoordinate.x - 2.0 * texelOffset.x, textureCoordinate.y + texelOffset.y);
    nearCoordinates[19] = vec2(textureCoordinate.x + 2.0 * texelOffset.x, textureCoordinate.y + texelOffset.y);
    nearCoordinates[20] = vec2(textureCoordinate.x - 2.0 * texelOffset.x, textureCoordinate.y + 2.0 * texelOffset.y);
    nearCoordinates[21] = vec2(textureCoordinate.x - texelOffset.x, textureCoordinate.y + 2.0 * texelOffset.y);
    nearCoordinates[22] = vec2(textureCoordinate.x, textureCoordinate.y + 2.0 * texelOffset.y);
    nearCoordinates[23] = vec2(textureCoordinate.x + texelOffset.x, textureCoordinate.y + 2.0 * texelOffset.y);
    nearCoordinates[24] = vec2(textureCoordinate.x + 2.0 * texelOffset.x, textureCoordinate.y + 2.0 * texelOffset.y);
    
    vec4 nearColors[25];
    float nearGrays[25];
    for (int i = 0; i < 25; i++) {
        nearColors[i] = texture2D(inputImageTexture, nearCoordinates[i]);
        nearGrays[i] = nearColors[i].r * 0.299 + nearColors[i].g * 0.587 + nearColors[i].b * 0.114;
    }
    
    int colorCounts[8];
    for (int i = 0; i < 8; i++) {
        colorCounts[i] = 0;
    }
    
    float increase1 = 0.125;
    float increase2 = 0.25;
    float increase3 = 0.375;
    float increase4 = 0.5;
    float increase5 = 0.625;
    float increase6 = 0.75;
    float increase7 = 0.875;
    float increase8 = 1.0;
    
    for (int i = 0; i < 25; i++) {
        if (nearGrays[i] <= increase1) {
            colorCounts[0]++;
        } else if (increase1 < nearGrays[i] && nearGrays[i] <= increase2) {
            colorCounts[1]++;
        } else if (increase2 < nearGrays[i] && nearGrays[i] <= increase3) {
            colorCounts[2]++;
        } else if (increase3 < nearGrays[i] && nearGrays[i] <= increase4) {
            colorCounts[3]++;
        } else if (increase4 < nearGrays[i] && nearGrays[i] <= increase5) {
            colorCounts[4]++;
        } else if (increase5 < nearGrays[i] && nearGrays[i] <= increase6) {
            colorCounts[5]++;
        } else if (increase6 < nearGrays[i] && nearGrays[i] <= increase7) {
            colorCounts[6]++;
        } else if (increase7 < nearGrays[i] && nearGrays[i] <= increase8) {
            colorCounts[7]++;
        }
    }
    
    int maxBucketsIndex = 0;
    for (int i = 0; i < 8; i++) {
        if (colorCounts[i] > colorCounts[maxBucketsIndex]) {
            maxBucketsIndex = i;
        }
    }
    float increaseMin = float(maxBucketsIndex) * 0.125;
    float increaseMax = float(maxBucketsIndex + 1) * 0.125;
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    int count = 0;
    for (int i = 0; i < 25; i++) {
        if (increaseMin < nearGrays[i] && nearGrays[i] <= increaseMax) {
            sum = sum + nearColors[i];
            count++;
        }
    }
    
    gl_FragColor = sum / float(count);
}
);

NSString *const kGPUImageOilFragment33ShaderString = SHADER_STRING
(
precision highp float;
precision highp int;
varying highp vec2 textureCoordinate;
varying vec2 nearCoordinatesVarying[9];
varying vec2 texelOffset;

uniform sampler2D inputImageTexture;

void main()
{
    vec2 nearCoordinates[9];
    for (int i = 0; i < 9; i++) {
        nearCoordinates[i] = nearCoordinatesVarying[i];
    }
    
    vec4 nearColors[9];
    float nearGrays[9];
    for (int i = 0; i < 9; i++) {
        nearColors[i] = texture2D(inputImageTexture, nearCoordinates[i]);
        nearGrays[i] = nearColors[i].r * 0.299 + nearColors[i].g * 0.587 + nearColors[i].b * 0.114;
    }
    
    int colorCounts[8];
    for (int i = 0; i < 8; i++) {
        colorCounts[i] = 0;
    }
    
    float increase1 = 0.125;
    float increase2 = 0.25;
    float increase3 = 0.375;
    float increase4 = 0.5;
    float increase5 = 0.625;
    float increase6 = 0.75;
    float increase7 = 0.875;
    float increase8 = 1.0;
    
    for (int i = 0; i < 9; i++) {
        if (nearGrays[i] <= increase1) {
            colorCounts[0]++;
        } else if (increase1 < nearGrays[i] && nearGrays[i] <= increase2) {
            colorCounts[1]++;
        } else if (increase2 < nearGrays[i] && nearGrays[i] <= increase3) {
            colorCounts[2]++;
        } else if (increase3 < nearGrays[i] && nearGrays[i] <= increase4) {
            colorCounts[3]++;
        } else if (increase4 < nearGrays[i] && nearGrays[i] <= increase5) {
            colorCounts[4]++;
        } else if (increase5 < nearGrays[i] && nearGrays[i] <= increase6) {
            colorCounts[5]++;
        } else if (increase6 < nearGrays[i] && nearGrays[i] <= increase7) {
            colorCounts[6]++;
        } else if (increase7 < nearGrays[i] && nearGrays[i] <= increase8) {
            colorCounts[7]++;
        }
    }
    
    int maxBucketsIndex = 0;
    for (int i = 0; i < 8; i++) {
        if (colorCounts[i] > colorCounts[maxBucketsIndex]) {
            maxBucketsIndex = i;
        }
    }
    float increaseMin = float(maxBucketsIndex) * 0.125;
    float increaseMax = float(maxBucketsIndex + 1) * 0.125;
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (increaseMin < nearGrays[i] && nearGrays[i] <= increaseMax) {
            sum = sum + nearColors[i];
            count++;
        }
    }
    
    gl_FragColor = sum / float(count);
}
);

@implementation GPUImageOilFilter

-(instancetype)init {
    if (self = [super initWithVertexShaderFromString:kGPUImageOilVertexShaderString fragmentShaderFromString:kGPUImageOilFragment55ShaderString]) {
        runSynchronouslyOnVideoProcessingQueue(^{
            self->texelWidthOffsetUniform = [self->filterProgram uniformIndex:@"texelWidthOffset"];
            self->texelHeightOffsetUniform = [self->filterProgram uniformIndex:@"texelHeightOffset"];
        });
    }
    return self;
}

- (void)setupFilterForSize:(CGSize)filterFrameSize;
{
    runSynchronouslyOnVideoProcessingQueue(^{
        self->texelWidthOffset = 1.0f / filterFrameSize.width;
        self->texelHeightOffset = 1.0f / filterFrameSize.height;
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
    
    glUniform1f(texelWidthOffsetUniform, texelWidthOffset);
    glUniform1f(texelHeightOffsetUniform, texelHeightOffset);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [firstInputFramebuffer unlock];
    
    if (usingNextFrameForImageCapture)
    {
        dispatch_semaphore_signal(imageCaptureSemaphore);
    }
}

@end
