//  Created by Danny Grein on 10/13/08.
//  Copyright TechFront Studios 2008. All rights reserved.

#if defined(_IPHONE_)

#include <QuartzCore/QuartzCore.h>
#include <OpenGLES/EAGLDrawable.h>

#include "platform/iphone/IphoneView.h"
#include "platform/iphone/IphoneSystemStub.h"
#include "System.h"
#include "SeedInit.h"
#include "Screen.h"
#include "Input.h"
#include "EventInputPointer.h"

using namespace Seed;

#define MAX_PATH_SIZE	1024

iphTouchInfo iphTouchBuff[PLATFORM_MAX_INPUT];

FilePath _defaultRootPathA[MAX_PATH_SIZE];
FilePath _defaultHomePathA[MAX_PATH_SIZE];
AppView *__view;


@implementation AppController

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	(void)interfaceOrientation;
	return YES;
}

@end


// A class extension to declare private methods
@interface AppView()
	@property (nonatomic, retain) EAGLContext *context;
	@property (nonatomic, assign) NSTimer *updateTimer;
	@property (nonatomic, assign) eiOSOpenGLVersion iOSOpenGLVersion;
@end


@implementation AppView

@synthesize context;
@synthesize updateTimer;
@synthesize iOSOpenGLVersion;


// You must implement this method
+ (Class)layerClass 
{
    return [CAEAGLLayer class];
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder 
{
	//printf(">> INIT\n");
    if ((self = [super initWithCoder:coder])) 
	{
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		iOSOpenGLVersion = OpenGLES1;
		
		if (!context)
		{
			context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
			iOSOpenGLVersion = OpenGLES1;
		}
			
        if (!context || ![EAGLContext setCurrentContext:context]) 
		{
			iOSOpenGLVersion = OpenGLFailed;
            [self release];
            return nil;
        }
    }
    return self;
}

- (eiOSOpenGLVersion)iOSOpenGLVersion
{
	return iOSOpenGLVersion;
}

- (void)Update
{
	Seed::Update();
}

- (void)layoutSubviews 
{
    [EAGLContext setCurrentContext:context];
	Seed::Initialize();
}

- (void)PrepareContext
{
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
}

- (void)SetContext
{
	[EAGLContext setCurrentContext:context];
}

- (void)ContextPresentRenderBuffer
{
	[context presentRenderbuffer:GL_RENDERBUFFER];
}

- (EAGLContext *)GetContext
{
	return [EAGLContext currentContext];
}

- (void)SetContext:(EAGLContext *)c
{
	[EAGLContext setCurrentContext:c];
}

- (void)Start
{
	__view = self;
}

- (void)Pause
{
	pSystem->Sleep();
}

- (void)Stop 
{
	Seed::Shutdown();
    self.updateTimer = nil;
}

- (void)SetUpdateRate:(NSTimeInterval)rate
{
    [updateTimer invalidate];
    updateTimer = [NSTimer scheduledTimerWithTimeInterval:rate target:self selector:@selector(Update) userInfo:nil repeats:YES];
	
	[self Update];
}

- (void)dealloc 
{
    [self Stop];
    
    if ([EAGLContext currentContext] == context)
	{
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];  
    [super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	(void)event;
	NSUInteger i = 0;
	
	for (UITouch *touch in touches) 
	{
		CGPoint p = [touch locationInView:self];
		CGPoint tp = p;
		
		if (pScreen->GetMode() == Seed::Video_iPhoneLandscape)
		{
			tp.x = p.y;
			tp.y = pScreen->GetHeight() - p.x;
		}
		else if (pScreen->GetMode() == Seed::Video_iPhoneLandscapeGoofy)
		{
			tp.x = pScreen->GetWidth() - p.y;
			tp.y = p.x;
		}
		
		i++;
		//Info("Press %d %f,%f", i, tp.x, tp.y);
		EventInputPointer ev(0, Seed::Button0, 0, 0, tp.x / pScreen->GetWidth(), tp.y / pScreen->GetHeight());
		pInput->SendEventPointerPress(&ev);
		/*
		iphTouchBuff[i].iTaps = [touch tapCount];
		iphTouchBuff[i].bStatus = 1;
		iphTouchBuff[i].fRelX = tp.x - iphTouchBuff[i].fPosX;
		iphTouchBuff[i].fRelY = tp.y - iphTouchBuff[i].fPosY;
		iphTouchBuff[i].fRelX = iphTouchBuff[i].fPosX;
		iphTouchBuff[i].fRelY = iphTouchBuff[i].fPosY;
		iphTouchBuff[i].fPosX = tp.x;
		iphTouchBuff[i].fPosY = tp.y;
		i++;
		
		if (i == PLATFORM_MAX_INPUT)
			break;
		*/
    }
	
	//pInput->Update(1.0f / 60.0f);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{  
	(void)event;
	NSUInteger i = 0;

	for (UITouch *touch in touches)
	{
		CGPoint p = [touch locationInView:self];
		CGPoint tp = p;
		
		if (pScreen->GetMode() == Seed::Video_iPhoneLandscape)
		{
			tp.x = p.y;
			tp.y = pScreen->GetHeight() - p.x;
		}
		else if (pScreen->GetMode() == Seed::Video_iPhoneLandscapeGoofy)
		{
			tp.x = pScreen->GetWidth() - p.y;
			tp.y = p.x;
		}
				
		iphTouchBuff[i].iTaps = [touch tapCount];
		iphTouchBuff[i].bStatus = 2;
		iphTouchBuff[i].fPosX = tp.x;
		iphTouchBuff[i].fPosY = tp.y;	
		i++;
		
		if (i == PLATFORM_MAX_INPUT)
			break;		
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	(void)event;
	NSUInteger i = 0;
	
	for (UITouch *touch in touches)
	{
		CGPoint p = [touch locationInView:self];
		CGPoint tp = p;
		
		if (pScreen->GetMode() == Seed::Video_iPhoneLandscape)
		{
			tp.x = p.y;
			tp.y = pScreen->GetHeight() - p.x;
		}
		else if (pScreen->GetMode() == Seed::Video_iPhoneLandscapeGoofy)
		{
			tp.x = pScreen->GetWidth() - p.y;
			tp.y = p.x;
		}
		
		i++;
		//Info("Release %d %f,%f", i, tp.x, tp.y);
		EventInputPointer ev(0, Seed::Button0, 0, 1, tp.x / pScreen->GetWidth(), tp.y / pScreen->GetHeight());
		pInput->SendEventPointerRelease(&ev);
		
		/*
		iphTouchBuff[i].iTaps = [touch tapCount];
		iphTouchBuff[i].bStatus = 3;
		iphTouchBuff[i].fPosX = tp.x;
		iphTouchBuff[i].fPosY = tp.y;
		i++;
				
		if (i == PLATFORM_MAX_INPUT)
			break;
		*/
	}
	
	//pInput->Update(1.0f / 60.0f);
}

@end

/*
Wrappers Objective-C -> C para comunicacao com C++.
*/

void iphSetUpdateRate(double rate)
{
	[__view SetUpdateRate:rate];
}

void iphPrepareGLContext()
{
	[__view PrepareContext];
}

void iphSetContext()
{
	[__view SetContext];
}

void iphContextPresentRenderBuffer()
{
	[__view ContextPresentRenderBuffer];
}

EAGLContext *iphGetContext()
{
	return [__view GetContext];
}

void iphSetContext(EAGLContext *c)
{
	[__view SetContext:c];
}

const FilePath *iphGetRootPath()
{
	CFStringRef fileString;
	fileString = (CFStringRef)[[NSBundle mainBundle] resourcePath];
	
	CFStringGetCString(fileString, _defaultRootPathA, MAX_PATH_SIZE, kCFStringEncodingASCII);
	//fprintf(stdout, "%s", _defaultRootPath);
	
	return _defaultRootPathA;
}

const FilePath *iphGetHomePath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask,YES);
	NSString *documentsDirectory= [paths objectAtIndex: 0];
	
	CFStringRef fileString;
	fileString = (CFStringRef)documentsDirectory;
	
	CFStringGetCString(fileString, _defaultHomePathA, MAX_PATH_SIZE, kCFStringEncodingASCII);
	//fprintf(stdout, "%s", _defaultHomePath);
	
	return _defaultHomePathA;
}

#endif // _IPHONE_

