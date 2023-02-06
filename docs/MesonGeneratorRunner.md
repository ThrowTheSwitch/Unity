# Meson Generator - Test Runner

One of the really nice things about using Unity with Ceedling is that Ceedling takes care of generating all of the test runners automatically. If you're not using Ceedling though, you'll need to do this yourself.

The way this is done in Unity is via a Ruby script called `generate_test_runner.rb`. When given a test file such as `test_example.c`, the script will generate `test_example_Runner.c`, which provides the `main` method and some other useful plumbing.

So that you don't have to run this by hand, a Meson generator is provided to generate the runner automatically for you. Generally with Meson, you would use Unity as a subproject and you'd want to access the generator from the parent.

For example, to get the generator you can use:

    unity_proj = subproject('unity')
    runner_gen = unity_proj.get_variable('gen_test_runner')

Once you have the generator you need to pass it the absolute path of your test file. This seems to be a bug in how the paths work with subprojects in Meson. You can get the full path with `meson.source_root()`, so you could do:

    test_runner = meson.source_root() / 'test/test_example.c'

You can then include `test_runner` as a normal dependency to your builds. Meson will create the test runner in a private directory for each build target. It's only meant to be used as part of the build, so if you need to refer to the runner after the build, you won't be able to use the generator.