// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Mariachi Engine. If not, see <http://www.gnu.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#import "stdafx.h"

#ifdef MARIACHI_PLATFORM_IPHONE

#import "opengles_uikit_window_view.h"

#import "renders/opengles1_uikit_renderer.h"

@implementation OpenglesUikitWindowView

@synthesize animating;
@dynamic animationFrameInterval;

+ (Class) layerClass {
    return [CAEAGLLayer class];
}

/**
 * The GL view is stored in the nib file.
 * When it's unarchived it's sent initWithCoder.
 */
- (id) initWithCoder: (NSCoder *) coder {
    if ((self = [super initWithCoder:coder])) {
        // retrieves the current layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *) self.layer;

        // sets the layer to opaque
        eaglLayer.opaque = TRUE;

        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        // allocates the renderer
        renderer = [[Opengles1UikitRenderer alloc] initWithLayer:eaglLayer depthBuffer:TRUE];

        // sets the animating flag
        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;

        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
    }

    return self;
}

/**
 * Draws (or redraws) the view calling the render.
 *
 * @param sender The sender object id.
 **/
- (void) drawView: (id) sender {
    [renderer render];
}

/**
 * Does the layout in the sub views, resizing
 * the screen if necessary.
 */
- (void) layoutSubviews {
    [renderer resizeFromLayer: (CAEAGLLayer *) self.layer];
    [self drawView:nil];
}

/**
 * Retrieves the animation frame interval.
 *
 * @return The animation frame interval.
 */
- (NSInteger) animationFrameInterval {
    return animationFrameInterval;
}

/**
 * Defines the number of times that a frame should be
 * rendered per second.
 *
 * @param frameInterval The interval in hertz of the frame rate.
 */
- (void) setAnimationFrameInterval: (NSInteger) frameInterval {
    // in case the frame interval is greater than one
    // otherwise the frame rate is invalid
    if(frameInterval >= 1) {
        // sets the animation frame interval
        animationFrameInterval = frameInterval;

        // in case is currently animating
        if(animating) {
            // restarts animation
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

/**
 * Starts the animation, setting up the scheduler for the animation.
 */
- (void) startAnimation {
    if (!animating) {
        // in case the display link is supported (3.1 or greater)
        if(displayLinkSupported) {
            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            // creates the animation timer for the animation (at sixty frames per second)
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        // sets the animating flag
        animating = TRUE;
    }
}

- (void) stopAnimation {
    if(animating) {
        if(displayLinkSupported) {
            [displayLink invalidate];
            displayLink = nil;
        } else {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
    }
}

- (void) dealloc {
    [renderer release];
    [super dealloc];
}

@end

#endif
