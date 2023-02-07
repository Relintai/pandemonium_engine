/*************************************************************************/
/*  PandemoniumHost.java                                                       */
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

package net.relintai.pandemonium.pandemonium;

import java.util.Collections;
import java.util.List;

/**
 * Denotate a component (e.g: Activity, Fragment) that hosts the {@link Pandemonium} fragment.
 */
public interface PandemoniumHost {
	/**
	 * Provides a set of command line parameters to setup the engine.
	 */
	default List<String> getCommandLine() {
		return Collections.emptyList();
	}

	/**
	 * Invoked on the render thread when the Pandemonium setup is complete.
	 */
	default void onPandemoniumSetupCompleted() {}

	/**
	 * Invoked on the render thread when the Pandemonium main loop has started.
	 */
	default void onPandemoniumMainLoopStarted() {}

	/**
	 * Invoked on the render thread to terminate the given Pandemonium instance.
	 */
	default void onPandemoniumForceQuit(Pandemonium instance) {}

	/**
 * Invoked on the render thread to terminate the Pandemonium instance with the given id.
	 * @param pandemoniumInstanceId id of the Pandemonium instance to terminate. See {@code onNewPandemoniumInstanceRequested}
	 *
	 * @return true if successful, false otherwise.
	 */
	default boolean onPandemoniumForceQuit(int pandemoniumInstanceId) {
		return false;
	}

	/**
	 * Invoked on the render thread when the Pandemonium instance wants to be restarted. It's up to the host
	 * to perform the appropriate action(s).
	 */
	default void onPandemoniumRestartRequested(Pandemonium instance) {}

	/**
	 * Invoked on the render thread when a new Pandemonium instance is requested. It's up to the host to
	 * perform the appropriate action(s).
	 *
	 * @param args Arguments used to initialize the new instance.
	 *
	 * @return the id of the new instance. See {@code onPandemoniumForceQuit}
	 */
	default void onNewPandemoniumInstanceRequested(String[] args) {
		return 0;
	}
}
