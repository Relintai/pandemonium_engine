/*************************************************************************/
/*  app_delegate.mm                                                      */
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

#import "app_delegate.h"

#include "core/config/project_settings.h"
#include "drivers/coreaudio/audio_driver_coreaudio.h"
#include "main/main.h"
#include "os_iphone.h"
#import "pandemonium_view.h"
#import "view_controller.h"

#import <AudioToolbox/AudioServices.h>

#define kRenderingFrequency 60

extern int gargc;
extern char **gargv;

extern int iphone_main(int, char **, String, String);
extern void iphone_finish();

@implementation AppDelegate

enum {
	SESSION_CATEGORY_AMBIENT,
	SESSION_CATEGORY_MULTI_ROUTE,
	SESSION_CATEGORY_PLAY_AND_RECORD,
	SESSION_CATEGORY_PLAYBACK,
	SESSION_CATEGORY_RECORD,
	SESSION_CATEGORY_SOLO_AMBIENT,
};

static ViewController *mainViewController = nil;

+ (ViewController *)viewController {
	return mainViewController;
}

static AppDelegate *delegate_singleton = nil;

+ (AppDelegate *)getSingleton {
	if (!delegate_singleton) {
		delegate_singleton = [AppDelegate new];
	}
	return delegate_singleton;
}

- (void)createViewController {
	ViewController *viewController = [[ViewController alloc] init];
	viewController.godotView.useCADisplayLink = bool(GLOBAL_DEF("display.iOS/use_cadisplaylink", true)) ? YES : NO;
	viewController.godotView.renderingInterval = 1.0 / kRenderingFrequency;

	self.window.rootViewController = viewController;

	// Show the window
	[self.window makeKeyAndVisible];

	mainViewController = viewController;
}

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions API_AVAILABLE(ios(13.0), tvos(13.0), visionos(1.0)) {
	if ([scene isKindOfClass:[UIWindowScene class]]) {
		UIWindowScene *window_scene = (UIWindowScene *)scene;
		self.window = [[UIWindow alloc] initWithWindowScene:window_scene];
		[self createViewController];
	}
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
			NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory,
			NSUserDomainMask, YES);
	NSString *cacheDirectory = [paths objectAtIndex:0];

	int err = iphone_main(gargc, gargv, String::utf8([documentsDirectory UTF8String]), String::utf8([cacheDirectory UTF8String]));
	if (err != 0) {
		// bail, things did not go very well for us, should probably output a message on screen with our error code...
		exit(0);
		return FALSE;
	}

	if (@available(iOS 13, tvOS 13, *)) {
		// NOP
	} else {
		// Create a full-screen window
		CGRect windowBounds = [[UIScreen mainScreen] bounds];
		self.window = [[UIWindow alloc] initWithFrame:windowBounds];
		[self createViewController];
	}

	[[NSNotificationCenter defaultCenter]
			addObserver:self
			   selector:@selector(onAudioInterruption:)
				   name:AVAudioSessionInterruptionNotification
				 object:[AVAudioSession sharedInstance]];

	int sessionCategorySetting = GLOBAL_GET("audio/general/ios/session_category");

	// Initialize with default Ambient category.
	AVAudioSessionCategory category = AVAudioSessionCategoryAmbient;

	if (sessionCategorySetting == SESSION_CATEGORY_MULTI_ROUTE) {
		category = AVAudioSessionCategoryMultiRoute;
	} else if (sessionCategorySetting == SESSION_CATEGORY_PLAY_AND_RECORD) {
		category = AVAudioSessionCategoryPlayAndRecord;
	} else if (sessionCategorySetting == SESSION_CATEGORY_PLAYBACK) {
		category = AVAudioSessionCategoryPlayback;
	} else if (sessionCategorySetting == SESSION_CATEGORY_RECORD) {
		category = AVAudioSessionCategoryRecord;
	} else if (sessionCategorySetting == SESSION_CATEGORY_SOLO_AMBIENT) {
		category = AVAudioSessionCategorySoloAmbient;
	}

	if (GLOBAL_GET("audio/general/ios/mix_with_others")) {
		[[AVAudioSession sharedInstance] setCategory:category withOptions:AVAudioSessionCategoryOptionMixWithOthers error:nil];
	} else {
		[[AVAudioSession sharedInstance] setCategory:category error:nil];
	}

	bool keep_screen_on = bool(GLOBAL_DEF("display/window/energy_saving/keep_screen_on", true));
	OSIPhone::get_singleton()->set_keep_screen_on(keep_screen_on);

	return TRUE;
}

- (void)onAudioInterruption:(NSNotification *)notification {
	if ([notification.name isEqualToString:AVAudioSessionInterruptionNotification]) {
		if ([[notification.userInfo valueForKey:AVAudioSessionInterruptionTypeKey] isEqualToNumber:[NSNumber numberWithInt:AVAudioSessionInterruptionTypeBegan]]) {
			NSLog(@"Audio interruption began");
			OSIPhone::get_singleton()->on_focus_out();
		} else if ([[notification.userInfo valueForKey:AVAudioSessionInterruptionTypeKey] isEqualToNumber:[NSNumber numberWithInt:AVAudioSessionInterruptionTypeEnded]]) {
			NSLog(@"Audio interruption ended");
			OSIPhone::get_singleton()->on_focus_in();
		}
	}
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
	if (OS::get_singleton()->get_main_loop()) {
		OS::get_singleton()->get_main_loop()->notification(
				MainLoop::NOTIFICATION_OS_MEMORY_WARNING);
	}
}

- (void)applicationWillTerminate:(UIApplication *)application {
	iphone_finish();
}

// When application goes to background (e.g. user switches to another app or presses Home),
// then applicationWillResignActive -> applicationDidEnterBackground are called.
// When user opens the inactive app again,
// applicationWillEnterForeground -> applicationDidBecomeActive are called.

// There are cases when applicationWillResignActive -> applicationDidBecomeActive
// sequence is called without the app going to background. For example, that happens
// if you open the app list without switching to another app or open/close the
// notification panel by swiping from the upper part of the screen.

- (void)sceneDidDisconnect:(UIScene *)scene API_AVAILABLE(ios(13.0), tvos(13.0), visionos(1.0)) {
	OSIPhone::get_singleton()->on_focus_out();
}

- (void)applicationWillResignActive:(UIApplication *)application {
	OSIPhone::get_singleton()->on_focus_out();
}

- (void)sceneWillResignActive:(UIScene *)scene API_AVAILABLE(ios(13.0), tvos(13.0), visionos(1.0)) {
	OSIPhone::get_singleton()->on_focus_out();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	OSIPhone::get_singleton()->on_focus_in();
}

- (void)sceneDidBecomeActive:(UIScene *)scene API_AVAILABLE(ios(13.0), tvos(13.0), visionos(1.0)) {
	OSIPhone::get_singleton()->on_focus_in();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	OSIPhone::get_singleton()->on_enter_background();
}

- (void)sceneDidEnterBackground:(UIScene *)scene API_AVAILABLE(ios(13.0), tvos(13.0), visionos(1.0)) {
	OSIPhone::get_singleton()->on_enter_background();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	OSIPhone::get_singleton()->on_exit_background();
}

- (void)sceneWillEnterForeground:(UIScene *)scene API_AVAILABLE(ios(13.0), tvos(13.0), visionos(1.0)) {
	OSIPhone::get_singleton()->on_exit_background();
}

- (void)dealloc {
	self.window = nil;
}

@end
