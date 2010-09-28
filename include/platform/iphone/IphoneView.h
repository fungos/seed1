//
//  Created by Danny Grein on 10/13/08.
//  Copyright TechFront Studios 2008. All rights reserved.

#include "Defines.h"
#include "interface/IGameApp.h"

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

/*
This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
The view content is basically an EAGL surface you render your OpenGL scene into.
Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
*/

@interface AppController : UIViewController
{
}
@end

@interface AppView : UIView
{
	enum eiOSOpenGLVersion
	{
		OpenGLES1,
		OpenGLES2,
		OpenGLFailed,
	};
	
	@private
		EAGLContext *context;
		NSTimer *updateTimer;
		eiOSOpenGLVersion iOSOpenGLVersion;
}

- (void)Start;
- (void)Stop;
- (void)Pause;
- (void)Update;
- (void)SetUpdateRate:(NSTimeInterval)rate;
- (void)PrepareContext;
- (void)SetContext;
- (void)ContextPresentRenderBuffer;
- (EAGLContext *)GetContext;
- (void)SetContext:(EAGLContext *)context;
- (eiOSOpenGLVersion)iOSOpenGLVersion;

@end

extern AppView *__view;

void iphSetUpdateRate(double rate);

void iphPrepareGLContext();
void iphSetContext();
void iphContextPresentRenderBuffer();
EAGLContext *iphGetContext();
void iphSetContext(EAGLContext *c);

const FilePath *iphGetRootPath();
const FilePath *iphGetHomePath();

namespace Seed {
	void SetGameApp(IGameApp *app);
} // namespace

