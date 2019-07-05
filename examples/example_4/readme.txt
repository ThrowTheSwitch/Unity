Example 4
=========

Close to the simplest possible example of Unity, using only basic features.
to build this example run "meson setup <build dir name>".

Meson uses the Ninja build system to actually build the code. To start the 
build, simply type the following command.  

"ninja -C <build dir name>"

Meson provides native support for running tests. The command to do that is simple.

"meson test -C <build dir name>".
 