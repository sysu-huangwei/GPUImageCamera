//
//  GPUImageMTSoftProcessFilter.m
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/25.
//  Copyright © 2020 meitu. All rights reserved.
//

#import "GPUImageMTSoftProcessFilter.h"

const CGFloat MTAdaptiveCoef = 1024.0f;

NSString *const kGPUImageMTSoftProcessVertexShaderString = SHADER_STRING
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

NSString *const kGPUImageMTSoftProcessFragmentShaderString = SHADER_STRING
(
 precision highp float;
 varying highp vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;
 uniform float textureWidthOffset;
 uniform float textureHeightOffset;
 uniform float alpha;
 
 uniform float kernel[17];
 
 float sdEllipse( in vec2 p, in vec2 ab )
 {
     p = abs(p); if( p.x > p.y ) {p=p.yx;ab=ab.yx;}
     float l = ab.y*ab.y - ab.x*ab.x;
     float m = ab.x*p.x/l;      float m2 = m*m;
     float n = ab.y*p.y/l;      float n2 = n*n;
     float c = (m2+n2-1.0)/3.0; float c3 = c*c*c;
     float q = c3 + m2*n2*2.0;
     float d = c3 + m2*n2;
     float g = m + m*n2;
     float co;
     if( d<0.0 )
     {
         float h = acos(q/c3)/3.0;
         float s = cos(h);
         float t = sin(h)*sqrt(3.0);
         float rx = sqrt( -c*(s + t + 2.0) + m2 );
         float ry = sqrt( -c*(s - t + 2.0) + m2 );
         co = (ry+sign(l)*rx+abs(g)/(rx*ry)- m)/2.0;
     }
     else
     {
         float h = 2.0*m*n*sqrt( d );
         float s = sign(q+h)*pow(abs(q+h), 1.0/3.0);
         float u = sign(q-h)*pow(abs(q-h), 1.0/3.0);
         float rx = -s - u - c*4.0 + 2.0*m2;
         float ry = (s - u)*sqrt(3.0);
         float rm = sqrt( rx*rx + ry*ry );
         co = (ry/sqrt(rm-rx)+2.0*g/rm-m)/2.0;
     }
     vec2 r = ab * vec2(co, sqrt(1.0-co*co));
     return length(r-p) * sign(p.y-r.y);
 }
 
 float blendScreen(float base, float blend) {
     return 1.0-((1.0-base)*(1.0-blend));
 }

 vec3 blendScreen(vec3 base, vec3 blend) {
     return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b));
 }
 
 void main()
 {
     vec2 uv = textureCoordinate;
     vec2 shiftingSize = vec2(textureWidthOffset, textureHeightOffset);

     vec4 origColor = texture2D(inputImageTexture, uv);
     vec4 gradientColor = texture2D(inputImageTexture2, uv);
     
     vec2 gradient = gradientColor.rg * 2.0 - 1.0;

     float direction = atan(gradient.y, gradient.x+0.0001) * 0.5 + 1.570795;
     float sumWeight = kernel[0];
     vec4 sumColor = origColor*kernel[0];
     vec2 directionUV = vec2(cos(direction), sin(direction)) * shiftingSize;
     for (int i = 1; i < 17; ++i) {
         vec2 offset = directionUV * float(i);
         vec4 color1 = texture2D(inputImageTexture, uv + offset);
         vec4 color2 = texture2D(inputImageTexture, uv - offset);
         float weight = kernel[i];
         sumWeight += 2.0 * weight;
         sumColor += (color1 + color2) * weight;
     }
     vec4 resultColor = vec4(mix(origColor.rgb, sumColor.rgb / sumWeight, alpha),1.0);
     gl_FragColor = resultColor;
 }
);


@implementation GPUImageMTSoftProcessFilter

- (id)init {
    if ((self = [super initWithVertexShaderFromString:kGPUImageMTSoftProcessVertexShaderString fragmentShaderFromString:kGPUImageMTSoftProcessFragmentShaderString])) {
        _samplerInterval = 0.6f;//0.8f;
        _alpha = 0.82f;
        GLfloat kernelf[17] = {0.013298, 0.013291, 0.013269, 0.013232, 0.013180, 0.013115, 0.013035, 0.012941, 0.012834, 0.012713, 0.012579, 0.012434, 0.012276, 0.012106, 0.011926, 0.011736, 0.011535};
        for (int i = 0; i < 17; i++) {
            kernel[i] = kernelf[i];
        }
        runSynchronouslyOnVideoProcessingQueue(^{
            self->textureWidthOffsetUniform = [self->filterProgram uniformIndex:@"textureWidthOffset"];
            self->textureHeightOffsetUniform = [self->filterProgram uniformIndex:@"textureHeightOffset"];
            self->alphaUniform = [self->filterProgram uniformIndex:@"alpha"];
            self->kernelUniform = [self->filterProgram uniformIndex:@"kernel"];
        });
    }
    return self;
}


- (void)setupFilterForSize:(CGSize)filterFrameSize {
    runSynchronouslyOnVideoProcessingQueue(^{
        CGFloat adaptiveCoef = (filterFrameSize.width > filterFrameSize.height ? filterFrameSize.width : filterFrameSize.height) / MTAdaptiveCoef;
        self->textureWidthOffset = adaptiveCoef * self->_samplerInterval / filterFrameSize.width;
        self->textureHeightOffset = adaptiveCoef * self->_samplerInterval / filterFrameSize.height;
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
    glUniform1f(alphaUniform, _alpha);
    glUniform1fv(kernelUniform, 17, kernel);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [firstInputFramebuffer unlock];
    
    if (usingNextFrameForImageCapture)
    {
        dispatch_semaphore_signal(imageCaptureSemaphore);
    }
}

@end
