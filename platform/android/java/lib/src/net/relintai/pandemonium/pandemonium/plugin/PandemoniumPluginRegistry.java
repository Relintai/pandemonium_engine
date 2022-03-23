/*************************************************************************/
/*  PandemoniumPluginRegistry.java                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           PANDEMONIUM ENGINE                                */
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

import net.relintai.pandemonium.pandemonium.Pandemonium;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Registry used to load and access the registered Pandemonium Android plugins.
 */
public final class PandemoniumPluginRegistry {
	private static final String TAG = PandemoniumPluginRegistry.class.getSimpleName();

	private static final String PANDEMONIUM_PLUGIN_V1_NAME_PREFIX = "net.relintai.pandemonium.plugin.v1.";

	private static PandemoniumPluginRegistry instance;
	private final ConcurrentHashMap<String, PandemoniumPlugin> registry;

	private PandemoniumPluginRegistry(Pandemonium pandemonium) {
		registry = new ConcurrentHashMap<>();
		loadPlugins(pandemonium);
	}

	/**
	 * Retrieve the plugin tied to the given plugin name.
	 * @param pluginName Name of the plugin
	 * @return {@link PandemoniumPlugin} handle if it exists, null otherwise.
	 */
	@Nullable
	public PandemoniumPlugin getPlugin(String pluginName) {
		return registry.get(pluginName);
	}

	/**
	 * Retrieve the full set of loaded plugins.
	 */
	public Collection<PandemoniumPlugin> getAllPlugins() {
		return registry.values();
	}

	/**
	 * Parse the manifest file and load all included Pandemonium Android plugins.
	 * <p>
	 * A plugin manifest entry is a '<meta-data>' tag setup as described in the {@link PandemoniumPlugin}
	 * documentation.
	 *
	 * @param pandemonium Pandemonium instance
	 * @return A singleton instance of {@link PandemoniumPluginRegistry}. This ensures that only one instance
	 * of each Pandemonium Android plugins is available at runtime.
	 */
	public static PandemoniumPluginRegistry initializePluginRegistry(Pandemonium pandemonium) {
		if (instance == null) {
			instance = new PandemoniumPluginRegistry(pandemonium);
		}

		return instance;
	}

	/**
	 * Return the plugin registry if it's initialized.
	 * Throws a {@link IllegalStateException} exception if not.
	 *
	 * @throws IllegalStateException if {@link PandemoniumPluginRegistry#initializePluginRegistry(Pandemonium)} has not been called prior to calling this method.
	 */
	public static PandemoniumPluginRegistry getPluginRegistry() throws IllegalStateException {
		if (instance == null) {
			throw new IllegalStateException("Plugin registry hasn't been initialized.");
		}

		return instance;
	}

	private void loadPlugins(Pandemonium pandemonium) {
		try {
			final Activity activity = pandemonium.getActivity();
			ApplicationInfo appInfo = activity
											  .getPackageManager()
											  .getApplicationInfo(activity.getPackageName(),
													  PackageManager.GET_META_DATA);
			Bundle metaData = appInfo.metaData;
			if (metaData == null || metaData.isEmpty()) {
				return;
			}

			int pandemoniumPluginV1NamePrefixLength = PANDEMONIUM_PLUGIN_V1_NAME_PREFIX.length();
			for (String metaDataName : metaData.keySet()) {
				// Parse the meta-data looking for entry with the Pandemonium plugin name prefix.
				if (metaDataName.startsWith(PANDEMONIUM_PLUGIN_V1_NAME_PREFIX)) {
					String pluginName = metaDataName.substring(pandemoniumPluginV1NamePrefixLength).trim();
					Log.i(TAG, "Initializing Pandemonium plugin " + pluginName);

					// Retrieve the plugin class full name.
					String pluginHandleClassFullName = metaData.getString(metaDataName);
					if (!TextUtils.isEmpty(pluginHandleClassFullName)) {
						try {
							// Attempt to create the plugin init class via reflection.
							@SuppressWarnings("unchecked")
							Class<PandemoniumPlugin> pluginClass = (Class<PandemoniumPlugin>)Class
																	 .forName(pluginHandleClassFullName);
							Constructor<PandemoniumPlugin> pluginConstructor = pluginClass
																				 .getConstructor(Pandemonium.class);
							PandemoniumPlugin pluginHandle = pluginConstructor.newInstance(pandemonium);

							// Load the plugin initializer into the registry using the plugin name as key.
							if (!pluginName.equals(pluginHandle.getPluginName())) {
								Log.w(TAG,
										"Meta-data plugin name does not match the value returned by the plugin handle: " + pluginName + " =/= " + pluginHandle.getPluginName());
							}
							registry.put(pluginName, pluginHandle);
							Log.i(TAG, "Completed initialization for Pandemonium plugin " + pluginHandle.getPluginName());
						} catch (ClassNotFoundException e) {
							Log.w(TAG, "Unable to load Pandemonium plugin " + pluginName, e);
						} catch (IllegalAccessException e) {
							Log.w(TAG, "Unable to load Pandemonium plugin " + pluginName, e);
						} catch (InstantiationException e) {
							Log.w(TAG, "Unable to load Pandemonium plugin " + pluginName, e);
						} catch (NoSuchMethodException e) {
							Log.w(TAG, "Unable to load Pandemonium plugin " + pluginName, e);
						} catch (InvocationTargetException e) {
							Log.w(TAG, "Unable to load Pandemonium plugin " + pluginName, e);
						}
					} else {
						Log.w(TAG, "Invalid plugin loader class for " + pluginName);
					}
				}
			}
		} catch (PackageManager.NameNotFoundException e) {
			Log.e(TAG, "Unable load Pandemonium Android plugins from the manifest file.", e);
		}
	}
}
