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

