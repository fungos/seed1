/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file Enum.h
	\author Danny Angelo Carminati Grein
	\brief Enumerations
*/

#ifndef __ENUM_H__
#define __ENUM_H__


namespace Seed {

enum eObjectType
{
	// Assets
	ObjectTexture = 0,
	ObjectImage,
	ObjectSprite,
	ObjectText,
	ObjectFont,
	ObjectSound,
	ObjectMusic,
	ObjectCollisionMask,
	ObjectPackage,
	ObjectEvent,
	ObjectMovie,
	ObjectSoundSource,
	ObjectSoundListener,
	ObjectDictionary,
	ObjectParticle,
	ObjectParticleEmitter,
	ObjectParticleEmitterObject,
	ObjectParticleManager,
	ObjectViewManager,
	ObjectRendererManager,

	// System
	ObjectConfiguration,
	ObjectInterfaceModule,
	ObjectInterfaceRenderable,
	ObjectInterfaceGameApp,

	// GUI Elements
	ObjectGuiManager,
	ObjectGuiWidget,
	ObjectGuiButton,
	ObjectGuiLabel,
	ObjectGuiTextArea,
	ObjectGuiWidgetContainer,

	// From here just user defines types
	ObjectUser
};

enum eLanguage
{
	en_US,
	pt_BR,
	es_ES,
	de_DE,
	ja_JP,
	fr_FR,
	cn_CN,
	MaximumLanguage
};

//! Basic frame rate values.
/*!
	Some platform may not support nativelly some frame rates values.
*/
enum eSystemFrameRate
{
	FrameRateLockAt60 = 60, /*!< 60 Frames per second. */
	FrameRateLockAt30 = 30, /*!< 30 Frames per second. */
	FrameRateLockAt20 = 20, /*!< 20 Frames per second. */
	FrameRateLockAt15 = 15, /*!< 15 Frames per second. */
	FrameRateUnlocked = 10000 /*!< 100 or more frames per second. */
};

//! Platform Simulation
/*!
	Sets the platform simulation "profile".
*/
enum ePlatformSimulation
{
	SimulateNothing,
	SimulateWii,
	SimulatePsp,
	SimulateN900
};

enum eRendererDeviceType
{
	RendererDeviceWii,
	RendererDeviceOpenGLES,
	RendererDeviceOpenGL14,
	RendererDeviceOpenGL20,
	RendererDeviceOpenGL30,
	RendererDeviceOpenGL40,
	RendererDeviceDirectX8,
	RendererDeviceDirectX9,
	RendererDeviceDirectX10,
	RendererDeviceDirectX11
};

enum eVideoMode
{
	Video_AutoDetect,
	Video_320x240,
	Video_480x272,
	Video_480x320,
	Video_iPhone,
	Video_iPhoneLandscape,
	Video_iPhonePortrait,
	Video_640x480,
	Video_NintendoWii,
	Video_800x600,
	Video_1024x768,
};

/// Widget Alignment
enum eHorizontalAlignment
{
	HorizontalAlignNone,
	HorizontalAlignLeft,
	HorizontalAlignRight,
	HorizontalAlignCenter
};

enum eVerticalAlignment
{
	VerticalAlignNone,
	VerticalAlignTop,
	VerticalAlignCenter,
	VerticalAlignBottom
};

enum eModifier
{
	ModifierNone			= 0x00,
	ModifierShiftLeft		= 0x0001,
	ModifierShiftRight		= 0x0002,
	ModifierShift			= (ModifierShiftLeft | ModifierShiftRight),
	ModifierControlLeft		= 0x0040,
	ModifierControlRight	= 0x0080,
	ModifierControl			= (ModifierControlLeft | ModifierControlRight),
	ModifierAltLeft			= 0x0100,
	ModifierAltRight		= 0x0200,
	ModifierAlt				= (ModifierAltLeft | ModifierAltRight),
	ModifierMetaLeft		= 0x0400,
	ModifierMetaRight		= 0x0800,
	ModifierMeta			= (ModifierMetaLeft | ModifierMetaRight),
	ModifierNumLock			= 0x1000,
	ModifierCapsLock		= 0x2000,
	ModifierKeypad			= 0x4000
};

/* Yes, it is entirely based on SDL, until now it is the most complete Keyboard mapping I ever found in an API. So, get used to it. */
enum eKey
{
	KeyNone				= 0,
	KeyFirst			= 0,

	KeyBackspace		= 8,
	KeyTab				= 9,
	KeyClear			= 12,
	KeyReturn			= 13,
	KeyEnter			= 13,
	KeyPause			= 19,
	KeyEscape			= 27,

	// Ascii table 1 to 1
	KeySpace			= ' ',
	KeyExclam			= '!',
	KeyQuoteDouble		= '"',
	KeyNumberSign		= '#',
	KeyDollar			= '$',
	KeyPercent			= '%',
	KeyAmpersand		= '&',
	KeyApostrophe		= 39,
	KeyParenLeft		= '(',
	KeyParenRight		= ')',
	KeyAsterisk			= '*',
	KeyPlus				= '+',
	KeyComma			= ',',
	KeyMinus			= '-',
	KeyPeriod			= '.',
	KeySlash			= '/',
	Key0				= '0',
	Key1				= '1',
	Key2				= '2',
	Key3				= '3',
	Key4				= '4',
	Key5				= '5',
	Key6				= '6',
	Key7				= '7',
	Key8				= '8',
	Key9				= '9',
	KeyColon			= ':',
	KeySemicolon		= ';',
	KeyLess				= '<',
	KeyEqual			= '=',
	KeyGreater			= '>',
	KeyQuestion			= '?',
	KeyAt				= '@',
	// Both capital and lowercase chars are valid
	KeyA				= 'A',
	KeyB				= 'B',
	KeyC				= 'C',
	KeyD				= 'D',
	KeyE				= 'E',
	KeyF				= 'F',
	KeyG				= 'G',
	KeyH				= 'H',
	KeyI				= 'I',
	KeyJ				= 'J',
	KeyK				= 'K',
	KeyL				= 'L',
	KeyM				= 'M',
	KeyN				= 'N',
	KeyO				= 'O',
	KeyP				= 'P',
	KeyQ				= 'Q',
	KeyR				= 'R',
	KeyS				= 'S',
	KeyT				= 'T',
	KeyU				= 'U',
	KeyV				= 'V',
	KeyW				= 'W',
	KeyX				= 'X',
	KeyY				= 'Y',
	KeyZ				= 'Z',
	KeyBracketLeft		= '[',
	KeyBackslash		= 92,
	KeyBracketRight		= ']',
	KeyCircum			= '^',
	KeyUnderscore		= '_',
	KeyQuoteLeft		= '`',
	// a-z 97-122
	KeyConsole			= KeyQuoteLeft,
	KeyBraceLeft		= '{',
	KeyBar				= '|',
	KeyPipe				= KeyBar,
	KeyBraceRight		= '}',
	KeyTilde			= '~',

	KeyDelete			= 127,

	KeyPad0				= 256,
	KeyPad1				= 257,
	KeyPad2				= 258,
	KeyPad3				= 259,
	KeyPad4				= 260,
	KeyPad5				= 261,
	KeyPad6				= 262,
	KeyPad7				= 263,
	KeyPad8				= 264,
	KeyPad9				= 265,
	KeyPadPeriod		= 266,
	KeyPadDivide		= 267,
	KeyPadMultiply		= 268,
	KeyPadMinus			= 269,
	KeyPadPlus			= 270,
	KeyPadEnter			= 271,
	KeyPadEquals		= 272,

	KeyUp				= 273,
	KeyDown				= 274,
	KeyRight			= 275,
	KeyLeft				= 276,
	KeyInsert			= 277,
	KeyHome				= 278,
	KeyEnd				= 279,
	KeyPageUp			= 280,
	KeyPageDown			= 281,

	KeyF1				= 282,
	KeyF2				= 283,
	KeyF3				= 284,
	KeyF4				= 285,
	KeyF5				= 286,
	KeyF6				= 287,
	KeyF7				= 288,
	KeyF8				= 289,
	KeyF9				= 290,
	KeyF10				= 291,
	KeyF11				= 292,
	KeyF12				= 293,
	KeyF13				= 294,
	KeyF14				= 295,
	KeyF15				= 296,

	KeyNumLock			= 300,
	KeyCapsLock			= 301,
	KeyScrollLock		= 302,

	KeyShiftRight		= 303,
	KeyShiftLeft		= 304,
	KeyControlRight		= 305,
	KeyControlLeft		= 306,
	KeyAltRight			= 307,
	KeyAltLeft			= 308,
	KeyMetaRight		= 309,
	KeyMetaLeft			= 310,
	KeySuperLeft		= 311,		/* Left "Windows" key */
	KeySuperRight		= 312,		/* Right "Windows" key */
	KeyAltGr			= 313,		/* "Mode" key */
	KeyMenu				= 314,		/* Menu key */

	KeyPrintScreen		= 316,

	KeyLast
};

enum eInputButton
{
	Button0 = 0x01,
	Button1 = 0x02,
	Button2 = 0x04,
	Button3 = 0x08,
	Button4 = 0x10,
	Button5 = 0x20,
	Button6 = 0x40,
	Button7 = 0x80,
	Button8 = 0x100,
	Button9 = 0x200,
	Button10 = 0x400,
	Button11 = 0x800,
	Button12 = 0x1000,
	Button13 = 0x2000,
	Button14 = 0x4000,
	Button15 = 0x8000,
	ButtonInvalid = 0x0,
	ButtonAll = 0xFFFF,

	// From here it is just helper enums, may not be portable.
	// Mouse specific mapping
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2,
	ButtonUp = Button3,
	ButtonDown = Button4,

	// Wiimote specific mapping
	ButtonA = Button0,
	ButtonB = Button1,
	ButtonDpadDown = Button2,
	ButtonDpadUp = Button3,
	ButtonDpadLeft = Button4,
	ButtonDpadRight = Button5,
	ButtonZ = Button6,
	ButtonNum2 = Button7,
	ButtonNum1 = Button8,
	ButtonC = Button9,
	ButtonPlus = Button10,
	ButtonMinus = Button11,
	ButtonHome = Button12
};

/// Maximum size of the cartridge
/**
Its possible that some platforms does not support some cartridge sizes.
*/
enum eCartridgeSize
{
	Cartridge512b,		/*!< Cartridge has up to 512 bytes of data. */
	Cartridge8192b,		/*!< Cartridge has up to 8Kb of data. */
	Cartridge32768b,	/*!< Cartridge has up to 32Kb of data. */
	Cartridge65536b,	/*!< Cartridge has up to 64Kb of data. */
	Cartridge262144b,	/*!< Cartridge has up to 256Kb bytes of data. */
	CartridgeUnlimited	/*!< Cartridge has no limit size */
};

/// Possible cartridge return codes
/**
These are the possible error codes returned by the cartridge
*/
enum eCartridgeError
{
	ErrorNone = 0,				/*!< No error ocurred. */
	ErrorNoCard,				/*!< Card is not present, can also mean that the device is not present in case of a removable device. */
	ErrorNotFormatted,			/*!< The card or device is not formatted. */
	ErrorDataCorrupt,			/*!< The data is corrupted. */
	ErrorFilesystemCorrupt,		/*!< The file system is corrupted, this is a fatal error. */
	ErrorDeviceFull,			/*!< Not enough space left on device. */
	ErrorInodeFull,				/*!< Not enough inodes left on device. */
	ErrorAccessDenied,			/*!< Failed to open a file, it could be busy or the user may not have permissions to open the file. */
	ErrorNotInitialized,		/*!< A function was called before the system was prepared or initialized to receive that call. */
	ErrorInvalidArgument		/*!< Invalid parameter supplied to a method. */
};

enum eAnimationFlag
{
	FlagNone 		= 0x00,
	FlagLooped 		= 0x01,
	FlagAnimated 	= 0x02
};

enum eTextureFilterType
{
	TextureFilterTypeMin,
	TextureFilterTypeMag
};

enum eTextureFilter
{
	TextureFilterLinear,
	TextureFilterNearest
};

enum eMeshType
{
	TriangleStrip,
	Triangles,
	LineStrip,
	Quads
};

/*
TODO: Rewrite blending logic to use Blending classes and use http://www.opengl.org/sdk/docs/man/xhtml/glBlendFuncSeparate.xml
TODO: Blending based on http://www.w3.org/TR/2009/WD-SVGCompositing-20090430/ and http://kobalicek.com/data/articles/Fog-Graphics.html (Compositing)

All operations following this will use these variables:

Variables:
	- C is the color component (RGB);
	- A is the alpha component;

Subscripts:
	- r is the rasterized output (result);
	- t is the texture source component input (pixel);
	- d is the texture destiny component input (pixel);
	- f is the color fragment intput (glColor*);

Base formula:
	Cr = Ct * paramSrc + Cd * paramDst
	Ar = At * paramSrc + Ad * paramDst

* REPLACE operations *

Just will replace the rasterized fragment with the newly computed one.

BlendNone

Will blend the incoming texture with the background respecting the texture alpha
and not the background alfa.

Cr = Ct * At + Cr * 0
Ar = At * At + Ar * 0

* DECAL operations

BlendDecalOverlay

Cr = Ct * Cr + Cf * 1.0f
Ar = At * Ar + Af * 1.0f

* BLEND operations *

All operations from here are blending between two textures:
- the source texture (currently selected texture);
- the destiny texture or rasterized texture (whatever is in the framebuffer / background);


BlendDefault

Cr = Ct * 1.0f + Cd * 1.0f = Ct + Cd
Ar = At * 1.0f + Ad * 1.0f = At + Ad


BlendMerge

It's an average between source and destiny (Morpho's screen):

Cr = Ct * At + Cd * Ad
Ar = At * At + Ad * Ad (this is correct?)


BlendScreen

This is based in this article:
http://gmc.yoyogames.com/index.php?s=321b708f77c5e17d0fca772ef7dcd6f9&showtopic=254433&st=0

Where it try to simulate photo shop screen blending.

Cr = Ct + Cd * (1.0f - Ct)
Ar = At + Ad * (1.0f - At)


* MODULATE operations *

Modulate is applied based in a Texture Pixel with a Color Fragment (glColor*) based
in the following formula:


BlendOverlay

Cr = Ct * Cr + Cf * 1.0f
Ar = At * Ar + Af * 1.0f


BlendLighten

Cr = Ct * (1.0f - At) + Cf * (1.0f - At)
Ar = At * (1.0f - At) + Af * (1.0f - At)


BlendColorDodge

Cr = Ct + Cf
Ar = At + Af


BlendModulateAlpha

Cr = Ct * At +  1 * (1.0f - At)
Ar = At * At + Af * (1.0f - At)


BlendModulate

Cr = Ct * At + Cf * (1.0f - At)
Ar = At * At + Af * (1.0f - At)


BlendAdditive

Cr = Ct * At + Cd
Ar = At * At + Ad

*/
enum eBlendMode
{
	BlendNone = 0,
	BlendDefault,
	BlendMerge,
	BlendScreen,
	BlendOverlay,
	BlendLighten,
	BlendColorDodge,
	BlendDecalOverlay,
	BlendModulateAlpha,
	BlendModulate,
	BlendAdditive
};

enum ePlayableState
{
	PlayableStopped,
	PlayablePaused,
	PlayablePlaying
};

enum eWidgetEventType
{
	WidgetEventNone,
	WidgetEventFocusLost,
	WidgetEventFocusReceived,
	WidgetEventOver,
	WidgetEventOut,
	WidgetEventPressedOut,
	WidgetEventReleasedOut,
	WidgetEventPressed,
	WidgetEventReleased,
	WidgetEventDrag,
	WidgetEventDrop
};

enum eWidgetState
{
	WidgetStateNone,
	WidgetStateFocused,
	WidgetStateOver,
	WidgetStatePressed,
	WidgetStatePressedOver,
	WidgetStatePressedOut,
	WidgetStateDrag
};

} // namespace


#endif // __ENUM_H__
