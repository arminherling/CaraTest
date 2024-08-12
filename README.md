# AalTest

## Description
Very basic test framework that I made for [Aalenian](https://github.com/arminherling/Aalenian), because QTest didn't support printing the combined results of multiple TestSuits without ugly hacks.

## Features
- Pretty console output
- Parameterized test
- Combined result of multiple test suits

## Downside
- Currently only supports windows

## Commandline parameters
The library currently has 2 commandline parameters to make debugging tests easier.

- ``break_on_fail`` 

This parameter triggers a debug break inside the AreEqual function. This allows us to inspect the parameters and we can go up the callstack into the failing test.
- ``break_and_rerun_on_fail``

This parameter triggers a debug break in the catch statement of the failed function and runs the test again. This allows us to step through the whole test again. To make this less of a pain, it is adviced to enable "only my code" while debugging.
This can be enabled by ``set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)`` in the CMakeLists.txt file.
