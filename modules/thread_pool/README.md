# Thread pool module

A c++ Godot engine module, that will help you with threading.

It can also work if threads are not available (like on the javascript backend), in this case it runs jobs on the 
main thread. Jobs themselves can also distribute their work onto multiple frames, and you can set how much time 
is allocated per frame.

You can access it's setting from the `Project->Project Settings...` menu, in the `ThreadPool` category.

# Pre-built binaries

You can grab a pre-built editor binary from the [Broken Seals](https://github.com/Relintai/broken_seals/releases) 
repo, should you want to. It contains all my modules.

# ThreadPoolJob

Contains a job that can run on different threads.

A job is only considered finished, if you set the 'complete' property to 'true'. If multiple threads are available, 
the system will not check for this though, because there is no need.

If you want to support envioronments that doesn't have threading, you can use:

```
bool should_do(const bool just_check = false);
bool should_return();
```

For example:

```
func _execute():
    # On the first run this will return true, on subsequest runs it will return false
    if should_do():
        thing1()

    # if you ran out the allocated timeframe in a frame, this will return true
    if should_return():
        return

    if should_do():
        thing2()

    if should_return():
        return

    thing3()

    complete = true

```

`should_do`'s optional parameter will let you just query the system, whether you finished a step, without
incrementing internal couters. This is useful for example to distribute algorithms onto multiple frames.

For example:

```
func _execute():
    if should_do(true):
        while current <= elements.size():
            #do heavy calculations

            current += 1

            if should_return():
                return

        #The heavy calculation finished, increment counters
        should_do()
    
    if should_return():
        return

    if should_do():
        thing2()

    if should_return():
        return

    thing3()

    complete = true

```

This class will need litle tweaks, hopefully I can get to is soon.

# ThreadPoolExecuteJob

This will let you run a method uin an another thread, without creating your own jobs.

Use it through the ThreadPool Singleton. Like:

```
ThreadPool.create_execute_job(self, "method", arg1, arg2, ...)

#or
ThreadPool.create_execute_job_simple(self, "method")
```

This class will need litle tweaks, hopefully I can get to is soon.

# ThreadPool singleton

The ThreadPool singleton handles jobs.

If you have a job, submit it using `add_job`:

```
MyJob job = MyJob.new()
ThreadPool.add_job(job)
```

It's api is still a bit messy, it will be cleaned up (hopefully very soon).

# Building

1. Get the source code for the engine.

If you want Godot 3.2:
```git clone -b 3.2 https://github.com/godotengine/godot.git godot```

If you want Godot 4.0:
```git clone https://github.com/godotengine/godot.git godot```

[last tested commit](https://github.com/godotengine/godot/commit/b7e10141197fdd9b0dbc4cfa7890329510d36540)

2. Go into Godot's modules directory.

```
cd ./godot/modules/
```

3. Clone this repository

```
git clone https://github.com/Relintai/thread_pool thread_pool
```

4. Build Godot. [Tutorial](https://docs.godotengine.org/en/latest/development/compiling/index.html)


