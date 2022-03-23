# Fish completion for the Pandemonium editor
# To use it, install this file in `~/.config/fish/completions` then restart your shell.
# You can also `source` this file directly in your shell startup file.
#
# Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.
# Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

function pandemonium_video_driver_args
    # Use a function instead of a fixed string to customize the argument descriptions.
    echo -e "GLES3\tOpenGL ES 2.0 renderer"
    echo -e "GLES2\tOpenGL ES 2.0 renderer"
end

# Erase existing completions for Pandemonium.
complete -c pandemonium -e

# General options:
complete -c pandemonium -s h -l help -d "Display the full help message"
complete -c pandemonium -l version -d "Display the version string"
complete -c pandemonium -s v -l verbose -d "Use verbose stdout mode"
complete -c pandemonium -l quiet -d "Quiet mode, silences stdout messages (errors are still displayed)"

# Run options:
complete -c pandemonium -s e -l editor -d "Start the editor instead of running the scene"
complete -c pandemonium -s p -l project-manager -d "Start the project manager, even if a project is auto-detected"
complete -c pandemonium -s q -l quit -d "Quit after the first iteration"
complete -c pandemonium -s l -l language -d "Use a specific locale (<locale> being a two-letter code)" -x
complete -c pandemonium -l path -d "Path to a project (<directory> must contain a 'project.pandemonium' file)" -r
complete -c pandemonium -s u -l upwards -d "Scan folders upwards for project.pandemonium file"
complete -c pandemonium -l main-pack -d "Path to a pack (.pck) file to load" -r
complete -c pandemonium -l render-thread -d "Set the render thread mode" -x -a "unsafe safe separate"
complete -c pandemonium -l remote-fs -d "Use a remote filesystem (<host/IP>[:<port>] address)" -x
complete -c pandemonium -l remote-fs-password -d "Password for remote filesystem" -x
complete -c pandemonium -l audio-driver -d "Set the audio driver" -x
complete -c pandemonium -l video-driver -d "Set the video driver" -x -a "(pandemonium_video_driver_args)"

# Display options:
complete -c pandemonium -s f -l fullscreen -d "Request fullscreen mode"
complete -c pandemonium -s m -l maximized -d "Request a maximized window"
complete -c pandemonium -s w -l windowed -d "Request windowed mode"
complete -c pandemonium -s t -l always-on-top -d "Request an always-on-top window"
complete -c pandemonium -l resolution -d "Request window resolution" -x
complete -c pandemonium -l position -d "Request window position" -x
complete -c pandemonium -l low-dpi -d "Force low-DPI mode (macOS and Windows only)"
complete -c pandemonium -l no-window -d "Disable window creation (Windows only), useful together with --script"
complete -c pandemonium -l enable-vsync-via-compositor -d "When Vsync is enabled, Vsync via the OS' window compositor (Windows only)"
complete -c pandemonium -l disable-vsync-via-compositor -d "Disable Vsync via the OS' window compositor (Windows only)"

# Debug options:
complete -c pandemonium -s d -l debug -d "Debug (local stdout debugger)"
complete -c pandemonium -s b -l breakpoints -d "Specify the breakpoint list as source::line comma-separated pairs, no spaces (use %20 instead)" -x
complete -c pandemonium -l profiling -d "Enable profiling in the script debugger"
complete -c pandemonium -l remote-debug -d "Enable remote debugging"
complete -c pandemonium -l debug-collisions -d "Show collision shapes when running the scene"
complete -c pandemonium -l debug-navigation -d "Show navigation polygons when running the scene"
complete -c pandemonium -l frame-delay -d "Simulate high CPU load (delay each frame by the given number of milliseconds)" -x
complete -c pandemonium -l time-scale -d "Force time scale (higher values are faster, 1.0 is normal speed)" -x
complete -c pandemonium -l disable-render-loop -d "Disable render loop so rendering only occurs when called explicitly from script"
complete -c pandemonium -l disable-crash-handler -d "Disable crash handler when supported by the platform code"
complete -c pandemonium -l fixed-fps -d "Force a fixed number of frames per second (this setting disables real-time synchronization)" -x
complete -c pandemonium -l print-fps -d "Print the frames per second to the stdout"

# Standalone tools:
complete -c pandemonium -s s -l script -d "Run a script" -r
complete -c pandemonium -l check-only -d "Only parse for errors and quit (use with --script)"
complete -c pandemonium -l export -d "Export the project using the given preset and matching release template" -x
complete -c pandemonium -l export-debug -d "Same as --export, but using the debug template" -x
complete -c pandemonium -l export-pack -d "Same as --export, but only export the game pack for the given preset" -x
complete -c pandemonium -l doctool -d "Dump the engine API reference to the given path in XML format, merging if existing files are found" -r
complete -c pandemonium -l no-docbase -d "Disallow dumping the base types (used with --doctool)"
complete -c pandemonium -l build-solutions -d "Build the scripting solutions (e.g. for C# projects)"
complete -c pandemonium -l gdnative-generate-json-api -d "Generate JSON dump of the Godot API for GDNative bindings"
complete -c pandemonium -l test -d "Run a unit test" -x
