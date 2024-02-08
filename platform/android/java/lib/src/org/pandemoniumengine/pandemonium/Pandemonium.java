/*************************************************************************/
/*  Pandemonium.java                                                     */
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

import static android.content.Context.MODE_PRIVATE;
import static android.content.Context.WINDOW_SERVICE;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Resources;
import android.graphics.Rect;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Messenger;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.util.Log;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsAnimation;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.annotation.CallSuper;
import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.fragment.app.Fragment;

import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;

import javax.microedition.khronos.opengles.GL10;

import org.pandemoniumengine.pandemonium.input.PandemoniumEditText;
import org.pandemoniumengine.pandemonium.input.PandemoniumInputHandler;
import org.pandemoniumengine.pandemonium.io.directory.DirectoryAccessHandler;
import org.pandemoniumengine.pandemonium.io.file.FileAccessHandler;
import org.pandemoniumengine.pandemonium.plugin.PandemoniumPlugin;
import org.pandemoniumengine.pandemonium.plugin.PandemoniumPluginRegistry;
import org.pandemoniumengine.pandemonium.utils.BenchmarkUtils;
import org.pandemoniumengine.pandemonium.utils.PandemoniumNetUtils;
import org.pandemoniumengine.pandemonium.utils.PermissionsUtil;

public class Pandemonium extends Fragment implements SensorEventListener, IDownloaderClient {
	private static final String TAG = Pandemonium.class.getSimpleName();

	static final int MAX_SINGLETONS = 64;
	private IStub mDownloaderClientStub;
	private TextView mStatusText;
	private TextView mProgressFraction;
	private TextView mProgressPercent;
	private TextView mAverageSpeed;
	private TextView mTimeRemaining;
	private ProgressBar mPB;
	private ClipboardManager mClipboard;

	private View mDashboard;
	private View mCellMessage;

	private Button mPauseButton;
	private Button mWiFiSettingsButton;

	private boolean use_immersive = false;
	private boolean use_debug_opengl = false;
	private boolean translucent = false;
	private boolean mStatePaused;
	private boolean activityResumed;
	private int mState;

	// Used to dispatch events to the main thread.
	private final Handler mainThreadHandler = new Handler(Looper.getMainLooper());

	private PandemoniumHost pandemoniumHost;
	private PandemoniumPluginRegistry pluginRegistry;

	static private Intent mCurrentIntent;

	public void onNewIntent(Intent intent) {
		mCurrentIntent = intent;
	}

	static public Intent getCurrentIntent() {
		return mCurrentIntent;
	}

	private void setState(int newState) {
		if (mState != newState) {
			mState = newState;
			mStatusText.setText(Helpers.getDownloaderStringResourceIDFromState(newState));
		}
	}

	private void setButtonPausedState(boolean paused) {
		mStatePaused = paused;
		int stringResourceID = paused ? R.string.text_button_resume : R.string.text_button_pause;
		mPauseButton.setText(stringResourceID);
	}

	static public class SingletonBase {
		protected void registerClass(String p_name, String[] p_methods) {
			PandemoniumPlugin.nativeRegisterSingleton(p_name, this);

			Class clazz = getClass();
			Method[] methods = clazz.getDeclaredMethods();
			for (Method method : methods) {
				boolean found = false;

				for (String s : p_methods) {
					if (s.equals(method.getName())) {
						found = true;
						break;
					}
				}
				if (!found)
					continue;

				List<String> ptr = new ArrayList<String>();

				Class[] paramTypes = method.getParameterTypes();
				for (Class c : paramTypes) {
					ptr.add(c.getName());
				}

				String[] pt = new String[ptr.size()];
				ptr.toArray(pt);

				PandemoniumPlugin.nativeRegisterMethod(p_name, method.getName(), method.getReturnType().getName(), pt);
			}

			Pandemonium.singletons[Pandemonium.singleton_count++] = this;
		}

		/**
		 * Invoked once during the Pandemonium Android initialization process after creation of the
		 * {@link PandemoniumView} view.
		 * <p>
		 * This method should be overridden by descendants of this class that would like to add
		 * their view/layout to the Pandemonium view hierarchy.
		 *
		 * @return the view to be included; null if no views should be included.
		 */
		@Nullable
		protected View onMainCreateView(Activity activity) {
			return null;
		}

		protected void onMainActivityResult(int requestCode, int resultCode, Intent data) {
		}

		protected void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		}

		protected void onMainPause() {}
		protected void onMainResume() {}
		protected void onMainDestroy() {}
		protected boolean onMainBackPressed() { return false; }

		protected void onGLDrawFrame(GL10 gl) {}
		protected void onGLSurfaceChanged(GL10 gl, int width, int height) {} // singletons will always miss first onGLSurfaceChanged call
		// protected void onGLSurfaceCreated(GL10 gl, EGLConfig config) {} // singletons won't be ready until first PandemoniumLib.step()

		public void registerMethods() {}
	}

	/*
	protected List<SingletonBase> singletons = new ArrayList<SingletonBase>();
	protected void instanceSingleton(SingletonBase s) {

		s.registerMethods();
		singletons.add(s);
	}
	*/

	private String[] command_line;
	private boolean use_apk_expansion;

	private ViewGroup containerLayout;
	public PandemoniumView mView;
	private boolean pandemonium_initialized = false;

	private SensorManager mSensorManager;
	private Sensor mAccelerometer;
	private Sensor mGravity;
	private Sensor mMagnetometer;
	private Sensor mGyroscope;

	public PandemoniumIO io;
	public PandemoniumNetUtils netUtils;
	private DirectoryAccessHandler directoryAccessHandler;
	private FileAccessHandler fileAccessHandler;

	static SingletonBase[] singletons = new SingletonBase[MAX_SINGLETONS];
	static int singleton_count = 0;

	public interface ResultCallback {
		void callback(int requestCode, int resultCode, Intent data);
	}
	public ResultCallback result_callback;

	@Override
	public void onAttach(Context context) {
		super.onAttach(context);
		if (getParentFragment() instanceof PandemoniumHost) {
			pandemoniumHost = (PandemoniumHost)getParentFragment();
		} else if (getActivity() instanceof PandemoniumHost) {
			pandemoniumHost = (PandemoniumHost)getActivity();
		}
	}

	@Override
	public void onDetach() {
		super.onDetach();
		pandemoniumHost = null;
	}

	@CallSuper
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (result_callback != null) {
			result_callback.callback(requestCode, resultCode, data);
			result_callback = null;
		}

		for (int i = 0; i < singleton_count; i++) {
			singletons[i].onMainActivityResult(requestCode, resultCode, data);
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onMainActivityResult(requestCode, resultCode, data);
		}
	}

	@CallSuper
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		for (int i = 0; i < singleton_count; i++) {
			singletons[i].onMainRequestPermissionsResult(requestCode, permissions, grantResults);
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onMainRequestPermissionsResult(requestCode, permissions, grantResults);
		}

		for (int i = 0; i < permissions.length; i++) {
			PandemoniumLib.requestPermissionResult(permissions[i], grantResults[i] == PackageManager.PERMISSION_GRANTED);
		}
	}

	/**
	 * Invoked on the render thread when the Pandemonium setup is complete.
	 */
	@CallSuper
	protected void onPandemoniumSetupCompleted() {
		Log.d(TAG, "onPandemoniumSetupCompleted");

		// These properties are defined after Godot setup completion, so we retrieve them here.
		boolean longPressEnabled = Boolean.parseBoolean(PandemoniumLib.getGlobal("input_devices/pointing/android/enable_long_press_as_right_click"));
		boolean panScaleEnabled = Boolean.parseBoolean(PandemoniumLib.getGlobal("input_devices/pointing/android/enable_pan_and_scale_gestures"));

		runOnUiThread(() -> {
			PandemoniumView renderView = getRenderView();
			PandemoniumInputHandler inputHandler = renderView != null ? renderView.getInputHandler() : null;
			if (inputHandler != null) {
				inputHandler.enableLongPress(longPressEnabled);
				inputHandler.enablePanningAndScalingGestures(panScaleEnabled);
			}
		});

		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onPandemoniumSetupCompleted();
		}

		if (pandemoniumHost != null) {
			pandemoniumHost.onPandemoniumSetupCompleted();
		}
	}

	/**
	 * Invoked on the render thread when the Pandemonium main loop has started.
	 */
	@CallSuper
	protected void onPandemoniumMainLoopStarted() {
		Log.d(TAG, "onPandemoniumMainLoopStarted");

		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onPandemoniumMainLoopStarted();
		}

		if (pandemoniumHost != null) {
			pandemoniumHost.onPandemoniumMainLoopStarted();
		}
	}

	/**
	 * Used by the native code (java_pandemonium_lib_jni.cpp) to complete initialization of the GLSurfaceView view and renderer.
	 */
	@Keep
	private void onVideoInit() {
		boolean use_gl3 = getGLESVersionCode() >= 0x00030000;

		final Activity activity = getActivity();
		containerLayout = new FrameLayout(activity);
		containerLayout.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

		// PandemoniumEditText layout
		PandemoniumEditText edittext = new PandemoniumEditText(activity);
		edittext.setLayoutParams(new ViewGroup.LayoutParams(LayoutParams.MATCH_PARENT,
				(int)getResources().getDimension(R.dimen.text_edit_height)));
		// ...add to FrameLayout
		containerLayout.addView(edittext);

		mView = new PandemoniumView(activity, this, use_gl3, use_debug_opengl, translucent);
		containerLayout.addView(mView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		edittext.setView(mView);
		io.setEdit(edittext);

		// Listeners for keyboard height.
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
			// Report the height of virtual keyboard as it changes during the animation.
			final View decorView = activity.getWindow().getDecorView();
			decorView.setWindowInsetsAnimationCallback(new WindowInsetsAnimation.Callback(WindowInsetsAnimation.Callback.DISPATCH_MODE_STOP) {
				int startBottom, endBottom;
				@Override
				public void onPrepare(@NonNull WindowInsetsAnimation animation) {
					startBottom = decorView.getRootWindowInsets().getInsets(WindowInsets.Type.ime()).bottom;
				}

				@NonNull
				@Override
				public WindowInsetsAnimation.Bounds onStart(@NonNull WindowInsetsAnimation animation, @NonNull WindowInsetsAnimation.Bounds bounds) {
					endBottom = decorView.getRootWindowInsets().getInsets(WindowInsets.Type.ime()).bottom;
					return bounds;
				}

				@NonNull
				@Override
				public WindowInsets onProgress(@NonNull WindowInsets windowInsets, @NonNull List<WindowInsetsAnimation> list) {
					// Find the IME animation.
					WindowInsetsAnimation imeAnimation = null;
					for (WindowInsetsAnimation animation : list) {
						if ((animation.getTypeMask() & WindowInsets.Type.ime()) != 0) {
							imeAnimation = animation;
							break;
						}
					}
					// Update keyboard height based on IME animation.
					if (imeAnimation != null) {
						float interpolatedFraction = imeAnimation.getInterpolatedFraction();
						// Linear interpolation between start and end values.
						float keyboardHeight = startBottom * (1.0f - interpolatedFraction) + endBottom * interpolatedFraction;
						PandemoniumLib.setVirtualKeyboardHeight((int)keyboardHeight);
					}
					return windowInsets;
				}

				@Override
				public void onEnd(@NonNull WindowInsetsAnimation animation) {
				}
			});
		} else {
			// Infer the virtual keyboard height using visible area.
			mView.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
				// Don't allocate a new Rect every time the callback is called.
				final Rect visibleSize = new Rect();

				@Override
				public void onGlobalLayout() {
					mView.getWindowVisibleDisplayFrame(visibleSize);
					final int keyboardHeight = mView.getHeight() - visibleSize.bottom;
					PandemoniumLib.setVirtualKeyboardHeight(keyboardHeight);
				}
			});
		}

		final String[] current_command_line = command_line;
		mView.queueEvent(() -> {
			if (!PandemoniumLib.setup(current_command_line)) {
				pandemonium_initialized = false;
				Log.e(TAG, "Unable to setup the Pandemonium engine! Aborting...");
				alert(R.string.error_engine_setup_message, R.string.text_error_title, this::forceQuit);
				return;
			}

			// Must occur after PandemoniumLib.setup has completed.
			for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
				plugin.onRegisterPluginWithPandemoniumNative();
			}

			setKeepScreenOn(Boolean.parseBoolean(PandemoniumLib.getGlobal("display/window/energy_saving/keep_screen_on")));

			// The Pandemonium Android plugins are setup on completion of PandemoniumLib.setup
			mainThreadHandler.post(() -> {
				// Include the non-null views returned in the Pandemonium view hierarchy.
				for (int i = 0; i < singleton_count; i++) {
					View view = singletons[i].onMainCreateView(activity);
					if (view != null) {
						containerLayout.addView(view);
					}
				}
			});
		});

		// Include the returned non-null views in the Pandemonium view hierarchy.
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			View pluginView = plugin.onMainCreate(activity);
			if (pluginView != null) {
				if (plugin.shouldBeOnTop()) {
					containerLayout.addView(pluginView);
				} else {
					containerLayout.addView(pluginView, 0);
				}
			}
		}
	}

	public boolean createOffscreenGL() {
		return mView.createOffscreenGL();
	}

	public void destroyOffscreenGL() {
		mView.destroyOffscreenGL();
	}

	public void setOffscreenGLCurrent(boolean p_current) {
		mView.setOffscreenGLCurrent(p_current);
	}

	public void setKeepScreenOn(final boolean p_enabled) {
		runOnUiThread(() -> {
			if (p_enabled) {
				getActivity().getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			} else {
				getActivity().getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			}
		});
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h) to vibrate the device.
	 * @param durationMs
	 */
	@SuppressLint("MissingPermission")
	@Keep
	private void vibrate(int durationMs) {
		if (durationMs > 0 && requestPermission("VIBRATE")) {
			Vibrator v = (Vibrator)getContext().getSystemService(Context.VIBRATOR_SERVICE);
			if (v != null) {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
					v.vibrate(VibrationEffect.createOneShot(durationMs, VibrationEffect.DEFAULT_AMPLITUDE));
				} else {
					// deprecated in API 26
					v.vibrate(durationMs);
				}
			}
		}
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h) to swap framebuffers.
	 */
	@Keep
	public void requestFramebufferSwap() {
		mView.requestFramebufferSwap();
	}

	public void restart() {
		if (pandemoniumHost != null) {
			pandemoniumHost.onPandemoniumRestartRequested(this);
		}
	}

	public void alert(final String message, final String title) {
		alert(message, title, null);
	}

	private void alert(@StringRes int messageResId, @StringRes int titleResId, @Nullable Runnable okCallback) {
		Resources res = getResources();
		alert(res.getString(messageResId), res.getString(titleResId), okCallback);
	}

	private void alert(final String message, final String title, @Nullable Runnable okCallback) {
		final Activity activity = getActivity();
		runOnUiThread(() -> {
			AlertDialog.Builder builder = new AlertDialog.Builder(activity);
			builder.setMessage(message).setTitle(title);
			builder.setPositiveButton(
					"OK",
					(dialog, id) -> {
						if (okCallback != null) {
							okCallback.run();
						}
						dialog.cancel();
					});
			AlertDialog dialog = builder.create();
			dialog.show();
		});
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h).
	 */
	@Keep
	public void enableForStealingFocus(int processId) {
		if (pandemoniumHost != null) {
			pandemoniumHost.enableForStealingFocus(processId);
		}
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h).
	 */
	@Keep
	public void moveWindowToForeground() {
		if (pandemoniumHost != null) {
			pandemoniumHost.moveWindowToForeground();
		}
	}

	public int getGLESVersionCode() {
		ActivityManager am = (ActivityManager)getContext().getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo deviceInfo = am.getDeviceConfigurationInfo();
		return deviceInfo.reqGlEsVersion;
	}

	@CallSuper
	protected String[] getCommandLine() {
		String[] original = parseCommandLine();
		String[] updated;

		List<String> hostCommandLine = pandemoniumHost != null ? pandemoniumHost.getCommandLine() : null;
		if (hostCommandLine == null || hostCommandLine.isEmpty()) {
			updated = original;
		} else {
			updated = Arrays.copyOf(original, original.length + hostCommandLine.size());
			for (int i = 0; i < hostCommandLine.size(); i++) {
				updated[original.length + i] = hostCommandLine.get(i);
			}
		}

		return updated;
	}

	private String[] parseCommandLine() {
		InputStream is;
		try {
			is = getActivity().getAssets().open("_cl_");
			byte[] len = new byte[4];
			int r = is.read(len);
			if (r < 4) {
				return new String[0];
			}
			int argc = ((int)(len[3] & 0xFF) << 24) | ((int)(len[2] & 0xFF) << 16) | ((int)(len[1] & 0xFF) << 8) | ((int)(len[0] & 0xFF));
			String[] cmdline = new String[argc];

			for (int i = 0; i < argc; i++) {
				r = is.read(len);
				if (r < 4) {
					return new String[0];
				}
				int strlen = ((int)(len[3] & 0xFF) << 24) | ((int)(len[2] & 0xFF) << 16) | ((int)(len[1] & 0xFF) << 8) | ((int)(len[0] & 0xFF));
				if (strlen > 65535) {
					return new String[0];
				}
				byte[] arg = new byte[strlen];
				r = is.read(arg);
				if (r == strlen) {
					cmdline[i] = new String(arg, "UTF-8");
				}
			}
			return cmdline;
		} catch (Exception e) {
			// The _cl_ file can be missing with no adverse effect
			return new String[0];
		}
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h) to check whether the activity is resumed or paused.
	 */
	@Keep
	private boolean isActivityResumed() {
		return activityResumed;
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h) to access the Android surface.
	 */
	@Keep
	private Surface getSurface() {
		return mView.getHolder().getSurface();
	}

	@Keep
	public PandemoniumView getRenderView() { // used by native side to get renderView
		return mView;
	}

	/**
	 * Used by the native code (java_pandemonium_wrapper.h) to access the input fallback mapping.
	 * @return The input fallback mapping for the current XR mode.
	 */
	@Keep
	private String getInputFallbackMapping() {
		return "Default Android Gamepad";
	}

	String expansion_pack_path;

	private void initializePandemonium() {
		if (expansion_pack_path != null) {
			String[] new_cmdline;
			int cll = 0;
			if (command_line != null) {
				new_cmdline = new String[command_line.length + 2];
				cll = command_line.length;
				for (int i = 0; i < command_line.length; i++) {
					new_cmdline[i] = command_line[i];
				}
			} else {
				new_cmdline = new String[2];
			}

			new_cmdline[cll] = "--main-pack";
			new_cmdline[cll + 1] = expansion_pack_path;
			command_line = new_cmdline;
		}

		final Activity activity = getActivity();
		io = new PandemoniumIO(activity);
		netUtils = new PandemoniumNetUtils(activity);

		Context context = getContext();
		directoryAccessHandler = new DirectoryAccessHandler(context);
		fileAccessHandler = new FileAccessHandler(context);

		mSensorManager = (SensorManager)activity.getSystemService(Context.SENSOR_SERVICE);
		mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
		mMagnetometer = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
		mGyroscope = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

		PandemoniumLib.initialize(activity, this, activity.getAssets(), io, netUtils, directoryAccessHandler, fileAccessHandler, use_apk_expansion);

		result_callback = null;

		pandemonium_initialized = true;
	}

	@Override
	public void onServiceConnected(Messenger m) {
		IDownloaderService remoteService = DownloaderServiceMarshaller.CreateProxy(m);
		remoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
	}

	@Override
	public void onCreate(Bundle icicle) {
		BenchmarkUtils.beginBenchmarkMeasure("Pandemonium::onCreate");

		super.onCreate(icicle);

		final Activity activity = getActivity();
		Window window = activity.getWindow();
		window.addFlags(WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON);
		mClipboard = (ClipboardManager)activity.getSystemService(Context.CLIPBOARD_SERVICE);
		pluginRegistry = PandemoniumPluginRegistry.initializePluginRegistry(this);

		// check for apk expansion API
		boolean md5mismatch = false;
		command_line = getCommandLine();
		String main_pack_md5 = null;
		String main_pack_key = null;

		List<String> new_args = new LinkedList<>();

		for (int i = 0; i < command_line.length; i++) {
			boolean has_extra = i < command_line.length - 1;
			if (command_line[i].equals("--debug_opengl")) {
				use_debug_opengl = true;
			} else if (command_line[i].equals("--translucent")) {
				translucent = true;
			} else if (command_line[i].equals("--use_immersive")) {
				use_immersive = true;
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) { // check if the application runs on an android 4.4+
					window.getDecorView().setSystemUiVisibility(
							View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
							View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
							View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
							View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | // hide nav bar
							View.SYSTEM_UI_FLAG_FULLSCREEN | // hide status bar
							View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

					UiChangeListener();
				}
			} else if (command_line[i].equals("--use_apk_expansion")) {
				use_apk_expansion = true;
			} else if (has_extra && command_line[i].equals("--apk_expansion_md5")) {
				main_pack_md5 = command_line[i + 1];
				i++;
			} else if (has_extra && command_line[i].equals("--apk_expansion_key")) {
				main_pack_key = command_line[i + 1];
				SharedPreferences prefs = activity.getSharedPreferences("app_data_keys",
						MODE_PRIVATE);
				Editor editor = prefs.edit();
				editor.putString("store_public_key", main_pack_key);

				editor.apply();
				i++;
			} else if (command_line[i].equals("--benchmark")) {
				BenchmarkUtils.setUseBenchmark(true);
				new_args.add(command_line[i]);
			} else if (has_extra && command_line[i].equals("--benchmark-file")) {
				BenchmarkUtils.setUseBenchmark(true);
				new_args.add(command_line[i]);

				// Retrieve the filepath
				BenchmarkUtils.setBenchmarkFile(command_line[i + 1]);
				new_args.add(command_line[i + 1]);
				i++;
			} else if (command_line[i].trim().length() != 0) {
				new_args.add(command_line[i]);
			}
		}

		if (new_args.isEmpty()) {
			command_line = null;
		} else {
			command_line = new_args.toArray(new String[new_args.size()]);
		}
		if (use_apk_expansion && main_pack_md5 != null && main_pack_key != null) {
			// check that environment is ok!
			if (!Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
				// show popup and die
			}

			// Build the full path to the app's expansion files
			try {
				expansion_pack_path = Helpers.getSaveFilePath(getContext());
				expansion_pack_path += "/main." + activity.getPackageManager().getPackageInfo(activity.getPackageName(), 0).versionCode + "." + activity.getPackageName() + ".obb";
			} catch (Exception e) {
				e.printStackTrace();
			}

			File f = new File(expansion_pack_path);

			boolean pack_valid = true;

			if (!f.exists()) {
				pack_valid = false;

			} else if (obbIsCorrupted(expansion_pack_path, main_pack_md5)) {
				pack_valid = false;
				try {
					f.delete();
				} catch (Exception e) {
				}
			}

			if (!pack_valid) {
				Intent notifierIntent = new Intent(activity, activity.getClass());
				notifierIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);

				PendingIntent pendingIntent = PendingIntent.getActivity(activity, 0,
						notifierIntent, PendingIntent.FLAG_UPDATE_CURRENT);

				int startResult;
				try {
					startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(
							getContext(),
							pendingIntent,
							PandemoniumDownloaderService.class);

					if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED) {
						// This is where you do set up to display the download
						// progress (next step in onCreateView)
						mDownloaderClientStub = DownloaderClientMarshaller.CreateStub(this,
								PandemoniumDownloaderService.class);

						return;
					}
				} catch (NameNotFoundException e) {
					// TODO Auto-generated catch block
				}
			}
		}

		mCurrentIntent = activity.getIntent();

		initializePandemonium();
		BenchmarkUtils.endBenchmarkMeasure("Pandemonium::onCreate");
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle icicle) {
		if (mDownloaderClientStub != null) {
			View downloadingExpansionView =
					inflater.inflate(R.layout.downloading_expansion, container, false);
			mPB = (ProgressBar)downloadingExpansionView.findViewById(R.id.progressBar);
			mStatusText = (TextView)downloadingExpansionView.findViewById(R.id.statusText);
			mProgressFraction = (TextView)downloadingExpansionView.findViewById(R.id.progressAsFraction);
			mProgressPercent = (TextView)downloadingExpansionView.findViewById(R.id.progressAsPercentage);
			mAverageSpeed = (TextView)downloadingExpansionView.findViewById(R.id.progressAverageSpeed);
			mTimeRemaining = (TextView)downloadingExpansionView.findViewById(R.id.progressTimeRemaining);
			mDashboard = downloadingExpansionView.findViewById(R.id.downloaderDashboard);
			mCellMessage = downloadingExpansionView.findViewById(R.id.approveCellular);
			mPauseButton = (Button)downloadingExpansionView.findViewById(R.id.pauseButton);
			mWiFiSettingsButton = (Button)downloadingExpansionView.findViewById(R.id.wifiSettingsButton);

			return downloadingExpansionView;
		}

		return containerLayout;
	}

	@Override
	public void onDestroy() {
		for (int i = 0; i < singleton_count; i++) {
			singletons[i].onMainDestroy();
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onMainDestroy();
		}

		PandemoniumLib.ondestroy();

		super.onDestroy();

		forceQuit();
	}

	@Override
	public void onPause() {
		super.onPause();
		activityResumed = false;

		if (!pandemonium_initialized) {
			if (null != mDownloaderClientStub) {
				mDownloaderClientStub.disconnect(getActivity());
			}
			return;
		}

		mView.onActivityPaused();

		mSensorManager.unregisterListener(this);

		for (int i = 0; i < singleton_count; i++) {
			singletons[i].onMainPause();
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onMainPause();
		}
	}

	@Override
	public void onStop() {
		super.onStop();
		if (!pandemonium_initialized) {
			if (null != mDownloaderClientStub) {
				mDownloaderClientStub.disconnect(getActivity());
			}
			return;
		}
		mView.onActivityStopped();
	}

	public boolean hasClipboard() {
		return mClipboard.hasPrimaryClip();
	}

	public String getClipboard() {
		ClipData clipData = mClipboard.getPrimaryClip();
		if (clipData == null)
			return "";
		CharSequence text = clipData.getItemAt(0).getText();
		if (text == null)
			return "";
		return text.toString();
	}

	public void setClipboard(String p_text) {
		ClipData clip = ClipData.newPlainText("myLabel", p_text);
		mClipboard.setPrimaryClip(clip);
	}

	@Override
	public void onStart() {
		super.onStart();
		if (!pandemonium_initialized) {
			if (null != mDownloaderClientStub) {
				mDownloaderClientStub.connect(getActivity());
			}
			return;
		}

		mView.onActivityStarted();
	}

	@Override
	public void onResume() {
		super.onResume();
		activityResumed = true;
		if (!pandemonium_initialized) {
			if (null != mDownloaderClientStub) {
				mDownloaderClientStub.connect(getActivity());
			}
			return;
		}

		mView.onActivityResumed();

		mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_GAME);
		mSensorManager.registerListener(this, mGravity, SensorManager.SENSOR_DELAY_GAME);
		mSensorManager.registerListener(this, mMagnetometer, SensorManager.SENSOR_DELAY_GAME);
		mSensorManager.registerListener(this, mGyroscope, SensorManager.SENSOR_DELAY_GAME);

		if (use_immersive && Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) { // check if the application runs on an android 4.4+
			Window window = getActivity().getWindow();
			window.getDecorView().setSystemUiVisibility(
					View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
					View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
					View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
					View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | // hide nav bar
					View.SYSTEM_UI_FLAG_FULLSCREEN | // hide status bar
					View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
		}

		for (int i = 0; i < singleton_count; i++) {
			singletons[i].onMainResume();
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onMainResume();
		}
	}

	public void UiChangeListener() {
		final View decorView = getActivity().getWindow().getDecorView();
		decorView.setOnSystemUiVisibilityChangeListener(visibility -> {
			if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
					decorView.setSystemUiVisibility(
							View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
							View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
							View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
							View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
							View.SYSTEM_UI_FLAG_FULLSCREEN |
							View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
				}
			}
		});
	}

	public float[] getRotatedValues(float values[]) {
		if (values == null || values.length != 3) {
			return values;
		}

		Display display =
				((WindowManager)getActivity().getSystemService(WINDOW_SERVICE)).getDefaultDisplay();
		int displayRotation = display.getRotation();

		float[] rotatedValues = new float[3];
		switch (displayRotation) {
			case Surface.ROTATION_0:
				rotatedValues[0] = values[0];
				rotatedValues[1] = values[1];
				rotatedValues[2] = values[2];
				break;
			case Surface.ROTATION_90:
				rotatedValues[0] = -values[1];
				rotatedValues[1] = values[0];
				rotatedValues[2] = values[2];
				break;
			case Surface.ROTATION_180:
				rotatedValues[0] = -values[0];
				rotatedValues[1] = -values[1];
				rotatedValues[2] = values[2];
				break;
			case Surface.ROTATION_270:
				rotatedValues[0] = values[1];
				rotatedValues[1] = -values[0];
				rotatedValues[2] = values[2];
				break;
		}

		return rotatedValues;
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		if (mView == null) {
			return;
		}

		final int typeOfSensor = event.sensor.getType();
		switch (typeOfSensor) {
			case Sensor.TYPE_ACCELEROMETER: {
				float[] rotatedValues = getRotatedValues(event.values);
				mView.queueEvent(() -> {
					PandemoniumLib.accelerometer(-rotatedValues[0], -rotatedValues[1], -rotatedValues[2]);
				});
				break;
			}
			case Sensor.TYPE_GRAVITY: {
				float[] rotatedValues = getRotatedValues(event.values);
				mView.queueEvent(() -> {
					PandemoniumLib.gravity(-rotatedValues[0], -rotatedValues[1], -rotatedValues[2]);
				});
				break;
			}
			case Sensor.TYPE_MAGNETIC_FIELD: {
				float[] rotatedValues = getRotatedValues(event.values);
				mView.queueEvent(() -> {
					PandemoniumLib.magnetometer(-rotatedValues[0], -rotatedValues[1], -rotatedValues[2]);
				});
				break;
			}
			case Sensor.TYPE_GYROSCOPE: {
				float[] rotatedValues = getRotatedValues(event.values);
				mView.queueEvent(() -> {
					PandemoniumLib.gyroscope(rotatedValues[0], rotatedValues[1], rotatedValues[2]);
				});
				break;
			}
		}
	}

	@Override
	public final void onAccuracyChanged(Sensor sensor, int accuracy) {
		// Do something here if sensor accuracy changes.
	}

	public void onBackPressed() {
		boolean shouldQuit = true;

		for (int i = 0; i < singleton_count; i++) {
			if (singletons[i].onMainBackPressed()) {
				shouldQuit = false;
			}
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			if (plugin.onMainBackPressed()) {
				shouldQuit = false;
			}
		}

		if (shouldQuit && mView != null) {
			mView.queueEvent(PandemoniumLib::back);
		}
	}

	/**
	 * Queue a runnable to be run on the render thread.
	 * <p>
	 * This must be called after the render thread has started.
	 */
	public final void runOnRenderThread(@NonNull Runnable action) {
		if (mView != null) {
			mView.queueEvent(action);
		}
	}

	public final void runOnUiThread(@NonNull Runnable action) {
		if (getActivity() != null) {
			getActivity().runOnUiThread(action);
		}
	}

	private void forceQuit() {
		// TODO: This is a temp solution. The proper fix will involve tracking down and properly shutting down each
		// native Pandemonium components that is started in Pandemonium#onVideoInit.
		forceQuit(0);
	}

	@Keep
	private boolean forceQuit(int instanceId) {
		if (pandemoniumHost == null) {
			return false;
		}
		if (instanceId == 0) {
			pandemoniumHost.onPandemoniumForceQuit(this);
			return true;
		} else {
			return pandemoniumHost.onPandemoniumForceQuit(instanceId);
		}
	}

	private boolean obbIsCorrupted(String f, String main_pack_md5) {
		try {
			InputStream fis = new FileInputStream(f);

			// Create MD5 Hash
			byte[] buffer = new byte[16384];

			MessageDigest complete = MessageDigest.getInstance("MD5");
			int numRead;
			do {
				numRead = fis.read(buffer);
				if (numRead > 0) {
					complete.update(buffer, 0, numRead);
				}
			} while (numRead != -1);

			fis.close();
			byte[] messageDigest = complete.digest();

			// Create Hex String
			StringBuilder hexString = new StringBuilder();
			for (int i = 0; i < messageDigest.length; i++) {
				String s = Integer.toHexString(0xFF & messageDigest[i]);

				if (s.length() == 1) {
					s = "0" + s;
				}
				hexString.append(s);
			}
			String md5str = hexString.toString();

			if (!md5str.equals(main_pack_md5)) {
				return true;
			}
			return false;
		} catch (Exception e) {
			e.printStackTrace();
			return true;
		}
	}

	public boolean requestPermission(String p_name) {
		return PermissionsUtil.requestPermission(p_name, getActivity());
	}

	public boolean requestPermissions() {
		return PermissionsUtil.requestManifestPermissions(getActivity());
	}

	public String[] getGrantedPermissions() {
		return PermissionsUtil.getGrantedPermissions(getActivity());
	}

	/**
	 * The download state should trigger changes in the UI --- it may be useful
	 * to show the state as being indeterminate at times. This sample can be
	 * considered a guideline.
	 */
	@Override
	public void onDownloadStateChanged(int newState) {
		setState(newState);
		boolean showDashboard = true;
		boolean showCellMessage = false;
		boolean paused;
		boolean indeterminate;
		switch (newState) {
			case IDownloaderClient.STATE_IDLE:
				// STATE_IDLE means the service is listening, so it's
				// safe to start making remote service calls.
				paused = false;
				indeterminate = true;
				break;
			case IDownloaderClient.STATE_CONNECTING:
			case IDownloaderClient.STATE_FETCHING_URL:
				showDashboard = true;
				paused = false;
				indeterminate = true;
				break;
			case IDownloaderClient.STATE_DOWNLOADING:
				paused = false;
				showDashboard = true;
				indeterminate = false;
				break;

			case IDownloaderClient.STATE_FAILED_CANCELED:
			case IDownloaderClient.STATE_FAILED:
			case IDownloaderClient.STATE_FAILED_FETCHING_URL:
			case IDownloaderClient.STATE_FAILED_UNLICENSED:
				paused = true;
				showDashboard = false;
				indeterminate = false;
				break;
			case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
			case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
				showDashboard = false;
				paused = true;
				indeterminate = false;
				showCellMessage = true;
				break;

			case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
				paused = true;
				indeterminate = false;
				break;
			case IDownloaderClient.STATE_PAUSED_ROAMING:
			case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
				paused = true;
				indeterminate = false;
				break;
			case IDownloaderClient.STATE_COMPLETED:
				showDashboard = false;
				paused = false;
				indeterminate = false;
				initializePandemonium();
				return;
			default:
				paused = true;
				indeterminate = true;
				showDashboard = true;
		}
		int newDashboardVisibility = showDashboard ? View.VISIBLE : View.GONE;
		if (mDashboard.getVisibility() != newDashboardVisibility) {
			mDashboard.setVisibility(newDashboardVisibility);
		}
		int cellMessageVisibility = showCellMessage ? View.VISIBLE : View.GONE;
		if (mCellMessage.getVisibility() != cellMessageVisibility) {
			mCellMessage.setVisibility(cellMessageVisibility);
		}

		mPB.setIndeterminate(indeterminate);
		setButtonPausedState(paused);
	}

	@Override
	public void onDownloadProgress(DownloadProgressInfo progress) {
		mAverageSpeed.setText(getString(R.string.kilobytes_per_second,
				Helpers.getSpeedString(progress.mCurrentSpeed)));
		mTimeRemaining.setText(getString(R.string.time_remaining,
				Helpers.getTimeRemaining(progress.mTimeRemaining)));

		mPB.setMax((int)(progress.mOverallTotal >> 8));
		mPB.setProgress((int)(progress.mOverallProgress >> 8));
		mProgressPercent.setText(String.format(Locale.ENGLISH, "%d %%", progress.mOverallProgress * 100 / progress.mOverallTotal));
		mProgressFraction.setText(Helpers.getDownloadProgressString(progress.mOverallProgress,
				progress.mOverallTotal));
	}

	public void initInputDevices() {
		mView.initInputDevices();
	}

	@Keep
	public DirectoryAccessHandler getDirectoryAccessHandler() {
		return directoryAccessHandler;
	}

	@Keep
	public FileAccessHandler getFileAccessHandler() {
		return fileAccessHandler;
	}

	@Keep
	private int createNewPandemoniumInstance(String[] args) {
		if (pandemoniumHost != null) {
			return pandemoniumHost.onNewPandemoniumInstanceRequested(args);
		}

		return 0;
	}

	@Keep
	private void beginBenchmarkMeasure(String label) {
		BenchmarkUtils.beginBenchmarkMeasure(label);
	}

	@Keep
	private void endBenchmarkMeasure(String label) {
		BenchmarkUtils.endBenchmarkMeasure(label);
	}

	@Keep
	private void dumpBenchmark(String benchmarkFile) {
		BenchmarkUtils.dumpBenchmark(fileAccessHandler, benchmarkFile);
	}
}
