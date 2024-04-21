
import os
import sys


import version

yes = True
no = False


def has_wrapper_for(lib):
	return True


def is_active():

	return True


def get_name():

	return 'FRT'


def can_build():

	if os.name != 'posix':
		return False
	return True


def get_opts():
	from SCons.Variables import BoolVariable

	return [
		('frt_arch', 'Architecture (no/arm32v6/arm32v7/arm64v8)', 'no'),
		('frt_cross', 'Cross compilation (no/auto/<triple>)', 'no'),
		BoolVariable('use_llvm', 'Use llvm compiler', no),
		BoolVariable('use_lto', 'Use link time optimization', no),
		BoolVariable('use_static_cpp', 'Link libgcc and libstdc++ statically', yes),
		BoolVariable('pulseaudio', 'Detect and use pulseaudio', yes),
	]


def get_flags():

	return [
		('tools', no),
	]

def check(env, key):

	if not (key in env):
		return False

	return env[key]

def configure(env):

	import methods

	env.Append(CPPPATH=['#platform/frt'])

	if check(env, 'use_llvm'):
		if 'clang++' not in env['CXX']:
			env['CC'] = 'clang'
			env['CXX'] = 'clang++'
			env['LD'] = 'clang++'

		env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
		env.extra_suffix += '.llvm'

	if check(env, 'use_lto'):
		if check(env, 'use_llvm'):
			env.Append(CCFLAGS=['-flto=thin'])
			env.Append(LINKFLAGS=['-fuse-ld=lld', '-flto=thin'])
			env['AR'] = 'llvm-ar'
			env['RANLIB'] = 'llvm-ranlib'
		else:
			env.Append(CCFLAGS=['-flto'])
			env.Append(LINKFLAGS=['-flto'])

		env.extra_suffix += '.lto'

	if env['frt_cross'] != 'no':
		if env['frt_cross'] == 'auto':
			triple = {
				'arm32v6': 'arm-linux-gnueabihf',
				'arm32v7': 'arm-linux-gnueabihf',
				'arm64v8': 'aarch64-linux-gnu',
			}[env['frt_arch']]
		else:
			triple = env['frt_cross']
		if check(env, 'use_llvm'):
			env.Append(CCFLAGS=['-target', triple])
			env.Append(LINKFLAGS=['-target', triple])
		else:
			env['CC'] = triple + '-gcc'
			env['CXX'] = triple + '-g++'
		pkg_config = triple + '-pkg-config'
	else:
		pkg_config = 'pkg-config'

	env.ParseConfig(pkg_config + " x11 --cflags --libs")

	env.Append(CCFLAGS=['-pipe'])
	env.Append(LINKFLAGS=['-pipe'])

	if os.system(pkg_config + ' --exists alsa') == 0:
		print('Enabling ALSA')
		env.Append(CPPFLAGS=['-DALSA_ENABLED'])
		if has_wrapper_for('alsa'):
			env['alsa'] = True
		else:
			env.ParseConfig(pkg_config + ' alsa --cflags --libs')
	else:
		print('ALSA libraries not found, disabling driver')

	if check(env, 'pulseaudio'):
            if os.system(pkg_config + ' --exists libpulse') == 0:
                    print('Enabling PulseAudio')
                    env.Append(CPPDEFINES=['PULSEAUDIO_ENABLED'])

                    if has_wrapper_for('libpulse'):
                            env.ParseConfig(pkg_config + ' --cflags libpulse')
                    else:
                            env.ParseConfig(pkg_config + ' --cflags --libs libpulse')
            else:
                    print('PulseAudio libraries not found, disabling driver')

	# pkg-config returns 0 when the lib exists...
	found_udev = not os.system(pkg_config + ' --exists libudev')
	if found_udev:
		print('Enabling udev support')
		env.Append(CPPFLAGS=['-DUDEV_ENABLED'])
		if has_wrapper_for('udev'):
			env.Append(FRT_MODULES=['include/libudev-so_wrap.c'])
		else:
			env.ParseConfig(pkg_config + ' libudev --cflags --libs')
		env.Append(CPPFLAGS=['-DJOYDEV_ENABLED'])
		env.Append(FRT_MODULES=['include/joypad_linux.cpp'])
	else:
		print('libudev development libraries not found, disabling udev support')

	if env['target'] == 'release':
		env.Append(CCFLAGS=['-O3', '-fomit-frame-pointer'])
	elif env['target'] == 'release_debug':
		env.Append(CCFLAGS=['-O3', '-DDEBUG_ENABLED'])
	elif env['target'] == 'debug':
		env.Append(CCFLAGS=['-g2', '-DDEBUG_ENABLED', '-DDEBUG_MEMORY_ENABLED'])

	#if env['target'].startswith('release'):
	#	if version.major == 2 or (version.major == 3 and version.minor == 0):
	#		env.Append(CCFLAGS=['-ffast-math'])

	if env['frt_arch'] == 'arm32v6':
		#env.Append(CCFLAGS=['-march=armv6', '-mfpu=vfp', '-mfloat-abi=hard'])
		env.Append(CCFLAGS=['-marm', '-mfpu=vfp', '-mfloat-abi=hard'])
		env.extra_suffix += '.arm32v6'
	elif env['frt_arch'] == 'arm32v7':
		env.Append(CCFLAGS=['-march=armv7-a', '-mfpu=neon-vfpv4', '-mfloat-abi=hard'])
		env.extra_suffix += '.arm32v7'
	elif env['frt_arch'] == 'arm64v8':
		env.Append(CCFLAGS=['-march=armv8-a'])
		env.extra_suffix += '.arm64v8'

	#env.Append(CFLAGS=['-std=gnu11']) # for libwebp (maybe more in the future)
	env.Append(CPPFLAGS=['-DFRT_ENABLED', '-DUNIX_ENABLED', '-DGLES2_ENABLED', '-DGLES_ENABLED'])
	env.Append(LIBS=['pthread'])

	if env['frt_arch'] == 'arm32v6':
		env.Append(LIBS=['atomic'])

	env.Append(FRT_MODULES=['envprobe.cpp'])
	env.Append(FRT_MODULES=['video_fbdev.cpp', 'keyboard_linux_input.cpp', 'mouse_linux_input.cpp'])
	env.Append(FRT_MODULES=['video_x11.cpp', 'keyboard_x11.cpp', 'mouse_x11.cpp'])

	env.Append(FRT_MODULES=['import/key_mapping_x11_3.cpp'])

	env.Append(FRT_MODULES=['dl/x11.gen.cpp', 'dl/egl.gen.cpp'])
	if os.path.isfile('/opt/vc/include/bcm_host.h'):
		env.Append(FRT_MODULES=['video_bcm.cpp', 'dl/bcm.gen.cpp'])
	if os.path.isfile('/usr/include/gbm.h'):
		env.Append(FRT_MODULES=['video_kmsdrm.cpp', 'dl/gbm.gen.cpp', 'dl/drm.gen.cpp'])

	env.Append(FRT_MODULES=['dl/gles2.gen.cpp'])

	env.Append(LIBS=['dl'])
