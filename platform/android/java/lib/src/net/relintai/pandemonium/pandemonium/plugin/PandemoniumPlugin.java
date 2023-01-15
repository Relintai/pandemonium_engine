/*************************************************************************/
/*  PandemoniumPlugin.java                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

package net.relintai.pandemonium.pandemonium.plugin;

import net.relintai.pandemonium.pandemonium.BuildConfig;
import net.relintai.pandemonium.pandemonium.Pandemonium;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Base class for the Pandemonium Android plugins.
 * <p>
 * A Pandemonium Android plugin is a regular Android library packaged as an aar archive file with the following caveats:
 * <p>
 * - The library must have a dependency on the Pandemonium Android library (pandemonium-lib.aar).
 * A stable version is available for each release.
 * <p>
 * - The library must include a <meta-data> tag in its manifest file setup as follow:
 * <meta-data android:name="net.relintai.pandemonium.plugin.v1.[PluginName]" android:value="[plugin.init.ClassFullName]" />
 * Where:
 * - 'PluginName' is the name of the plugin.
 * - 'plugin.init.ClassFullName' is the full name (package + class name) of the plugin class
 * extending {@link PandemoniumPlugin}.
 *
 * A plugin can also define and provide c/c++ gdnative libraries and nativescripts for the target
 * app/game to leverage.
 * The shared library for the gdnative library will be automatically bundled by the aar build
 * system.
 * Pandemonium '*.gdnlib' and '*.gdns' resource files must however be manually defined in the project
 * 'assets' directory. The recommended path for these resources in the 'assets' directory should be:
 * 'pandemonium/plugin/v1/[PluginName]/'
 */
public abstract class PandemoniumPlugin {
	private static final String TAG = PandemoniumPlugin.class.getSimpleName();

	private final Pandemonium pandemonium;
	private final ConcurrentHashMap<String, SignalInfo> registeredSignals = new ConcurrentHashMap<>();

	public PandemoniumPlugin(Pandemonium pandemonium) {
		this.pandemonium = pandemonium;
	}

	/**
	 * Provides access to the Pandemonium engine.
	 */
	protected Pandemonium getPandemonium() {
		return pandemonium;
	}

	/**
	 * Provides access to the underlying {@link Activity}.
	 */
	@Nullable
	protected Activity getActivity() {
		return pandemonium.getActivity();
	}

	/**
	 * Register the plugin with Pandemonium native code.
	 *
	 * This method is invoked on the render thread.
	 */
	public final void onRegisterPluginWithPandemoniumNative() {
		registeredSignals.putAll(
				registerPluginWithPandemoniumNative(this, getPluginName(), getPluginMethods(), getPluginSignals(),
						getPluginGDNativeLibrariesPaths()));
	}

	/**
	 * Register the plugin with Pandemonium native code.
	 *
	 * This method must be invoked on the render thread.
	 */
	public static void registerPluginWithPandemoniumNative(Object pluginObject,
			PandemoniumPluginInfoProvider pluginInfoProvider) {
		registerPluginWithPandemoniumNative(pluginObject, pluginInfoProvider.getPluginName(),
				Collections.emptyList(), pluginInfoProvider.getPluginSignals(),
				pluginInfoProvider.getPluginGDNativeLibrariesPaths());

		// Notify that registration is complete.
		pluginInfoProvider.onPluginRegistered();
	}

	private static RBMap<String, SignalInfo> registerPluginWithPandemoniumNative(Object pluginObject,
			String pluginName, List<String> pluginMethods, Set<SignalInfo> pluginSignals,
			Set<String> pluginGDNativeLibrariesPaths) {
		nativeRegisterSingleton(pluginName, pluginObject);

		Set<Method> filteredMethods = new HashSet<>();
		Class<?> clazz = pluginObject.getClass();

		Method[] methods = clazz.getDeclaredMethods();
		for (Method method : methods) {
			// Check if the method is annotated with {@link UsedByPandemonium}.
			if (method.getAnnotation(UsedByPandemonium.class) != null) {
				filteredMethods.add(method);
			} else {
				// For backward compatibility, process the methods from the given <pluginMethods> argument.
				for (String methodName : pluginMethods) {
					if (methodName.equals(method.getName())) {
						filteredMethods.add(method);
						break;
					}
				}
			}
		}

		for (Method method : filteredMethods) {
			List<String> ptr = new ArrayList<>();

			Class<?>[] paramTypes = method.getParameterTypes();
			for (Class<?> c : paramTypes) {
				ptr.add(c.getName());
			}

			String[] pt = new String[ptr.size()];
			ptr.toArray(pt);

			nativeRegisterMethod(pluginName, method.getName(), method.getReturnType().getName(), pt);
		}

		// Register the signals for this plugin.
		RBMap<String, SignalInfo> registeredSignals = new HashMap<>();
		for (SignalInfo signalInfo : pluginSignals) {
			String signalName = signalInfo.getName();
			nativeRegisterSignal(pluginName, signalName, signalInfo.getParamTypesNames());
			registeredSignals.put(signalName, signalInfo);
		}

		// Get the list of gdnative libraries to register.
		if (!pluginGDNativeLibrariesPaths.isEmpty()) {
			nativeRegisterGDNativeLibraries(pluginGDNativeLibrariesPaths.toArray(new String[0]));
		}

		return registeredSignals;
	}

	/**
	 * Invoked once during the Pandemonium Android initialization process after creation of the
	 * {@link net.relintai.pandemonium.pandemonium.PandemoniumView} view.
	 * <p>
	 * The plugin can return a non-null {@link View} layout in order to add it to the Pandemonium view
	 * hierarchy.
	 *
	 * Use shouldBeOnTop() to set whether the plugin's {@link View} should be added on top or behind
	 * the main Pandemonium view.
	 *
	 * @see Activity#onCreate(Bundle)
	 * @return the plugin's view to be included; null if no views should be included.
	 */
	@Nullable
	public View onMainCreate(Activity activity) {
		return null;
	}

	/**
	 * @see Activity#onActivityResult(int, int, Intent)
	 */
	public void onMainActivityResult(int requestCode, int resultCode, Intent data) {
	}

	/**
	 * @see Activity#onRequestPermissionsResult(int, String[], int[])
	 */
	public void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
	}

	/**
	 * @see Activity#onPause()
	 */
	public void onMainPause() {}

	/**
	 * @see Activity#onResume()
	 */
	public void onMainResume() {}

	/**
	 * @see Activity#onDestroy()
	 */
	public void onMainDestroy() {}

	/**
	 * @see Activity#onBackPressed()
	 */
	public boolean onMainBackPressed() { return false; }

	/**
	 * Invoked on the render thread when the Pandemonium setup is complete.
	 */
	public void onPandemoniumSetupCompleted() {}

	/**
	 * Invoked on the render thread when the Pandemonium main loop has started.
	 */
	public void onPandemoniumMainLoopStarted() {}

	/**
	 * Invoked once per frame on the GL thread after the frame is drawn.
	 */
	public void onGLDrawFrame(GL10 gl) {}

	/**
	 * Called on the GL thread after the surface is created and whenever the OpenGL ES surface size
	 * changes.
	 */
	public void onGLSurfaceChanged(GL10 gl, int width, int height) {}

	/**
	 * Called on the GL thread when the surface is created or recreated.
	 */
	public void onGLSurfaceCreated(GL10 gl, EGLConfig config) {}

	/**
	 * Returns the name of the plugin.
	 * <p>
	 * This value must match the one listed in the plugin '<meta-data>' manifest entry.
	 */
	@NonNull
	public abstract String getPluginName();

	/**
	 * Returns the list of methods to be exposed to Pandemonium.
	 *
	 * @deprecated Used the {@link UsedByPandemonium} annotation instead.
	 */
	@NonNull
	@Deprecated
	public List<String> getPluginMethods() {
		return Collections.emptyList();
	}

	/**
	 * Returns the list of signals to be exposed to Pandemonium.
	 */
	@NonNull
	public Set<SignalInfo> getPluginSignals() {
		return Collections.emptySet();
	}

	/**
	 * Returns the paths for the plugin's gdnative libraries.
	 *
	 * The paths must be relative to the 'assets' directory and point to a '*.gdnlib' file.
	 */
	@NonNull
	protected Set<String> getPluginGDNativeLibrariesPaths() {
		return Collections.emptySet();
	}

	/**
	 * Returns whether the plugin's {@link View} returned in onMainCreate() should be placed on
	 * top of the main Pandemonium view.
	 *
	 * Returning false causes the plugin's {@link View} to be placed behind, which can be useful
	 * when used with transparency in order to let the Pandemonium view handle inputs.
	 */
	public boolean shouldBeOnTop() {
		return true;
	}

	/**
	 * Runs the specified action on the UI thread. If the current thread is the UI
	 * thread, then the action is executed immediately. If the current thread is
	 * not the UI thread, the action is posted to the event queue of the UI thread.
	 *
	 * @param action the action to run on the UI thread
	 */
	protected void runOnUiThread(Runnable action) {
		pandemonium.runOnUiThread(action);
	}

	/**
	 * Queue the specified action to be run on the render thread.
	 *
	 * @param action the action to run on the render thread
	 */
	protected void runOnRenderThread(Runnable action) {
		pandemonium.runOnRenderThread(action);
	}

	/**
	 * Emit a registered Pandemonium signal.
	 * @param signalName Name of the signal to emit. It will be validated against the set of registered signals.
	 * @param signalArgs Arguments used to populate the emitted signal. The arguments will be validated against the {@link SignalInfo} matching the registered signalName parameter.
	 */
	protected void emitSignal(final String signalName, final Object... signalArgs) {
		try {
			// Check that the given signal is among the registered set.
			SignalInfo signalInfo = registeredSignals.get(signalName);
			if (signalInfo == null) {
				throw new IllegalArgumentException(
						"Signal " + signalName + " is not registered for this plugin.");
			}
			emitSignal(getPandemonium(), getPluginName(), signalInfo, signalArgs);
		} catch (IllegalArgumentException exception) {
			Log.w(TAG, exception.getMessage());
			if (BuildConfig.DEBUG) {
				throw exception;
			}
		}
	}

	/**
	 * Emit a Pandemonium signal.
	 * @param pandemonium
	 * @param pluginName Name of the Pandemonium plugin the signal will be emitted from. The plugin must already be registered with the Pandemonium engine.
	 * @param signalInfo Information about the signal to emit.
	 * @param signalArgs Arguments used to populate the emitted signal. The arguments will be validated against the given {@link SignalInfo} parameter.
	 */
	public static void emitSignal(Pandemonium pandemonium, String pluginName, SignalInfo signalInfo, final Object... signalArgs) {
		try {
			if (signalInfo == null) {
				throw new IllegalArgumentException("Signal must be non null.");
			}

			// Validate the arguments count.
			Class<?>[] signalParamTypes = signalInfo.getParamTypes();
			if (signalArgs.length != signalParamTypes.length) {
				throw new IllegalArgumentException(
						"Invalid arguments count. Should be " + signalParamTypes.length + "  but is " + signalArgs.length);
			}

			// Validate the argument's types.
			for (int i = 0; i < signalParamTypes.length; i++) {
				if (!signalParamTypes[i].isInstance(signalArgs[i])) {
					throw new IllegalArgumentException(
							"Invalid type for argument #" + i + ". Should be of type " + signalParamTypes[i].getName());
				}
			}

			pandemonium.runOnRenderThread(() -> nativeEmitSignal(pluginName, signalInfo.getName(), signalArgs));

		} catch (IllegalArgumentException exception) {
			Log.w(TAG, exception.getMessage());
			if (BuildConfig.DEBUG) {
				throw exception;
			}
		}
	}

	/**
	 * Used to setup a {@link PandemoniumPlugin} instance.
	 * @param p_name Name of the instance.
	 */
	public static native void nativeRegisterSingleton(String p_name, Object object);

	/**
	 * Used to complete registration of the {@link PandemoniumPlugin} instance's methods.
	 * @param p_sname Name of the instance
	 * @param p_name Name of the method to register
	 * @param p_ret Return type of the registered method
	 * @param p_params Method parameters types
	 */
	public static native void nativeRegisterMethod(String p_sname, String p_name, String p_ret, String[] p_params);

	/**
	 * Used to register gdnative libraries bundled by the plugin.
	 * @param gdnlibPaths Paths to the libraries relative to the 'assets' directory.
	 */
	private static native void nativeRegisterGDNativeLibraries(String[] gdnlibPaths);

	/**
	 * Used to complete registration of the {@link PandemoniumPlugin} instance's methods.
	 * @param pluginName Name of the plugin
	 * @param signalName Name of the signal to register
	 * @param signalParamTypes Signal parameters types
	 */
	private static native void nativeRegisterSignal(String pluginName, String signalName, String[] signalParamTypes);

	/**
	 * Used to emit signal by {@link PandemoniumPlugin} instance.
	 * @param pluginName Name of the plugin
	 * @param signalName Name of the signal to emit
	 * @param signalParams Signal parameters
	 */
	private static native void nativeEmitSignal(String pluginName, String signalName, Object[] signalParams);
}
