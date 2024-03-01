/*************************************************************************/
/*  FullScreenPandemoniumApp.java                                        */
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

package org.pandemoniumengine.pandemonium;

import org.pandemoniumengine.pandemonium.utils.ProcessPhoenix;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.CallSuper;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;

/**
 * Base activity for Android apps intending to use Pandemonium as the primary and only screen.
 *
 * It's also a reference implementation for how to setup and use the {@link Pandemonium} fragment
 * within an Android app.
 */
public abstract class FullScreenPandemoniumApp extends FragmentActivity implements PandemoniumHost {
	private static final String TAG = FullScreenPandemoniumApp.class.getSimpleName();

	protected static final String EXTRA_FORCE_QUIT = "force_quit_requested";
	protected static final String EXTRA_NEW_LAUNCH = "new_launch_requested";

	@Nullable
	private Pandemonium pandemoniumFragment;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pandemonium_app_layout);

		handleStartIntent(getIntent(), true);

		Fragment currentFragment = getSupportFragmentManager().findFragmentById(R.id.pandemonium_fragment_container);

		if (currentFragment instanceof Pandemonium) {
			Log.v(TAG, "Reusing existing Pandemonium fragment instance.");
			pandemoniumFragment = (Pandemonium)currentFragment;
		} else {
			Log.v(TAG, "Creating new Pandemonium fragment instance.");
			pandemoniumFragment = initPandemoniumInstance();
			getSupportFragmentManager().beginTransaction().replace(R.id.pandemonium_fragment_container, pandemoniumFragment).setPrimaryNavigationFragment(pandemoniumFragment).commitNowAllowingStateLoss();
		}
	}

	@Override
	public void onDestroy() {
		Log.v(TAG, "Destroying Pandemonium app...");
		super.onDestroy();
		terminatePandemoniumInstance(pandemoniumFragment);
	}

	@Override
	public final void onPandemoniumForceQuit(Pandemonium instance) {
		runOnUiThread(() -> {
			terminatePandemoniumInstance(instance);
		});
	}

	private void terminatePandemoniumInstance(Pandemonium instance) {
		if (instance == pandemoniumFragment) {
			Log.v(TAG, "Force quitting Pandemonium instance");
			ProcessPhoenix.forceQuit(FullScreenPandemoniumApp.this);
		}
	}

	@Override
	public final void onPandemoniumRestartRequested(Pandemonium instance) {
		runOnUiThread(() -> {
			if (instance == pandemoniumFragment) {
				// It's very hard to properly de-initialize Pandemonium on Android to restart the game
				// from scratch. Therefore, we need to kill the whole app process and relaunch it.
				//
				// Restarting only the activity, wouldn't be enough unless it did proper cleanup (including
				// releasing and reloading native libs or resetting their state somehow and clearing statics).
				Log.v(TAG, "Restarting Pandemonium instance...");
				ProcessPhoenix.triggerRebirth(FullScreenPandemoniumApp.this);
			}
		});
	}

	@Override
	public void onNewIntent(Intent intent) {
		super.onNewIntent(intent);

		setIntent(intent);

		handleStartIntent(intent, false);

		if (pandemoniumFragment != null) {
			pandemoniumFragment.onNewIntent(intent);
		}
	}

	private void handleStartIntent(Intent intent, boolean newLaunch) {
		boolean forceQuitRequested = intent.getBooleanExtra(EXTRA_FORCE_QUIT, false);
		if (forceQuitRequested) {
			Log.d(TAG, "Force quit requested, terminating..");
			ProcessPhoenix.forceQuit(this);
			return;
		}

		if (!newLaunch) {
			boolean newLaunchRequested = intent.getBooleanExtra(EXTRA_NEW_LAUNCH, false);
			if (newLaunchRequested) {
				Log.d(TAG, "New launch requested, restarting..");

				Intent restartIntent = new Intent(intent).putExtra(EXTRA_NEW_LAUNCH, false);
				ProcessPhoenix.triggerRebirth(this, restartIntent);
				return;
			}
		}
	}

	@CallSuper
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (pandemoniumFragment != null) {
			pandemoniumFragment.onActivityResult(requestCode, resultCode, data);
		}
	}

	@CallSuper
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		if (pandemoniumFragment != null) {
			pandemoniumFragment.onRequestPermissionsResult(requestCode, permissions, grantResults);
		}
	}

	@Override
	public void onBackPressed() {
		if (pandemoniumFragment != null) {
			pandemoniumFragment.onBackPressed();
		} else {
			super.onBackPressed();
		}
	}

	/**
	 * Used to initialize the Pandemonium fragment instance in {@link FullScreenPandemoniumApp#onCreate(Bundle)}.
	 */
	@NonNull
	protected Pandemonium initPandemoniumInstance() {
		return new Pandemonium();
	}

	@Nullable
	protected final Pandemonium getPandemoniumFragment() {
		return pandemoniumFragment;
	}
}
