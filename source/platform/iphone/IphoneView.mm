//  Created by Danny Grein on 10/13/08.
//  Copyright TechFront Studios 2008. All rights reserved.


#include <QuartzCore/QuartzCore.h>
#include <OpenGLES/EAGLDrawable.h>

#include "IphoneView.h"
#include "System.h"
#include "SeedInit.h"
#include "IphoneSystemStub.h"

using namespace Seed;

#define MAX_PATH_SIZE	1024

iphTouchInfo iphTouchBuff[PLATFORM_MAX_INPUT];

char _defaultRootPath[MAX_PATH_SIZE];
char _defaultHomePath[MAX_PATH_SIZE];
AppView *__view;


@implementation AppController

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
}

@end


// A class extension to declare private methods
@interface AppView()
	@property (nonatomic, retain) EAGLContext *context;
	@property (nonatomic, assign) NSTimer *updateTimer;
@end


@implementation AppView

@synthesize context;
@synthesize updateTimer;


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
        if (!context || ![EAGLContext setCurrentContext:context]) 
		{
            [self release];
            return nil;
        }
    }
    return self;
}


- (void)Update
{
	Seed::Update();
}


- (void)layoutSubviews 
{
    [EAGLContext setCurrentContext:context];

/*
	pScreen->Setup(Graphic::Screen::LANDSCAPE);
	pSeed->Initialize();
	pSystem->SetFrameRate(ISystem::RATE_60FPS);

	pRenderer = new Renderer2D();
	pInputPointer->Initialize();
	
	pGameApp->Initialize();
*/
	Seed::Initialize();
}


- (void)PrepareContext
{
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
}


- (void)SetContext
{
	[EAGLContext setCurrentContext:context];
}


- (void)ContextPresentRenderBuffer
{
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
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
	/*
	if (pRenderer)
		delete pRenderer;
	pRenderer = NULL;
	*/
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
	NSUInteger i = 0;
	for (UITouch *touch in touches) 
	{
		CGPoint p = [touch locationInView:self];
		
		iphTouchBuff[i].iTaps = [touch tapCount];
		iphTouchBuff[i].bStatus = 1;
		iphTouchBuff[i].fRelX = p.x - iphTouchBuff[i].fPosX;
		iphTouchBuff[i].fRelY = p.y - iphTouchBuff[i].fPosY;
		iphTouchBuff[i].fRelX = iphTouchBuff[i].fPosX;
		iphTouchBuff[i].fRelY = iphTouchBuff[i].fPosY;
		iphTouchBuff[i].fPosX = p.x;
		iphTouchBuff[i].fPosY = p.y;
		i++;
		
		if (i == PLATFORM_MAX_INPUT)
			break;
    }    
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{  
	NSUInteger i = 0;

	for (UITouch *touch in touches)
	{
		CGPoint p = [touch locationInView:self];
		
		iphTouchBuff[i].iTaps = [touch tapCount];
		iphTouchBuff[i].bStatus = 2;
		iphTouchBuff[i].fPosX = p.x;
		iphTouchBuff[i].fPosY = p.y;	
		i++;
		
		if (i == PLATFORM_MAX_INPUT)
			break;		
	}
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSUInteger i = 0;
	
	for (UITouch *touch in touches)
	{
		CGPoint p = [touch locationInView:self];
		
		iphTouchBuff[i].iTaps = [touch tapCount];
		iphTouchBuff[i].bStatus = 3;
		iphTouchBuff[i].fPosX = p.x;
		iphTouchBuff[i].fPosY = p.y;
		i++;
		
		if (i == PLATFORM_MAX_INPUT)
			break;		
	}
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


const char *iphGetRootPath()
{
	CFStringRef fileString;
	fileString = (CFStringRef)[[NSBundle mainBundle] resourcePath];
	
	CFStringGetCString(fileString, _defaultRootPath, MAX_PATH_SIZE, kCFStringEncodingASCII);
	//fprintf(stdout, "%s", _defaultRootPath);
	
	return _defaultRootPath;
}


const char *iphGetHomePath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask,YES);
	NSString *documentsDirectory= [paths objectAtIndex: 0];
	
	CFStringRef fileString;
	fileString = (CFStringRef)documentsDirectory;
	
	CFStringGetCString(fileString, _defaultHomePath, MAX_PATH_SIZE, kCFStringEncodingASCII);
	//fprintf(stdout, "%s", _defaultHomePath);
	
	return _defaultHomePath;
}

