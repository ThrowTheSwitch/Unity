# Unity Fixtures

This Framework is an optional add-on to Unity.  By including unity_framework.h in place of unity.h,
you may now work with Unity in a manner similar to CppUTest.  This framework adds the concepts of 
test groups and gives finer control of your tests over the command line.

This framework is primarily supplied for those working through James Grenning's book on Embedded
Test Driven Development, or those coming to Unity from CppUTest. We should note that using this
framework glosses over some of the features of Unity, and makes it more difficult
to integrate with other testing tools like Ceedling and CMock.

# Dependency Notification

Fixtures, by default, uses the Memory addon as well. This is to make it simple for those trying to
follow along with James' book. Using them together is completely optional. You may choose to use 
Fixtures without Memory handling by defining `UNITY_FIXTURE_NO_EXTRAS`. It will then stop automatically
pulling in extras and leave you to do it as desired.
