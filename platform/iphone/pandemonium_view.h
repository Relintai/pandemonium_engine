/*************************************************************************/
/*  pandemonium_view.h                                                   */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#import <AVFoundation/AVFoundation.h>
#import <MediaPlayer/MediaPlayer.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <UIKit/UIKit.h>

class String;

@class PandemoniumView;
@protocol DisplayLayer;
@protocol PandemoniumViewRendererProtocol;

@protocol PandemoniumViewDelegate

- (BOOL)pandemoniumViewFinishedSetup:(PandemoniumView *)view;

@end

@interface PandemoniumView : UIView

@property(assign, nonatomic) id<PandemoniumViewRendererProtocol> renderer;
@property(assign, nonatomic) id<PandemoniumViewDelegate> delegate;

@property(assign, readonly, nonatomic) BOOL isActive;

@property(strong, readonly, nonatomic) CALayer<DisplayLayer> *renderingLayer;
@property(assign, readonly, nonatomic) BOOL canRender;

@property(assign, nonatomic) NSTimeInterval renderingInterval;

- (CALayer<DisplayLayer> *)initializeRendering;
- (void)stopRendering;
- (void)startRendering;

@property(nonatomic, assign) BOOL useCADisplayLink;

@end
