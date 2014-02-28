Example 3
=========

This example project gives an example of some passing, ignored, and failing tests.
It's simple and meant for you to look over and get an idea for what all of this stuff does.

You can build and test using the makefile if you have gcc installed (you may need to tweak
the locations of some tools in the makefile).  Otherwise, the rake version will let you
test with gcc or a couple versions of IAR.  You can tweak the yaml files to get those versions
running.

Ruby is required if you're using the rake version (obviously).  This version shows off most of
Unity's advanced features (automatically creating test runners, fancy summaries, etc.)

The makefile version doesn't require anything outside of your normal build tools, but won't do the
extras for you.  So that you can test right away, we've written the test runners for you and
put them in the test\no_ruby subdirectory.  If you make changes to the tests or source, you might
need to update these (like when you add or remove tests).  Do that for a while and you'll learn
why you really want to start using the Ruby tools.