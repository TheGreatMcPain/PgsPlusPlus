# libpgs

A C++ library for reading Presentation Graphics Stream (PGS) subtitles.

Check out the documentation [here](https://iamsomeone2.github.io/libpgs/html/index.html).

## Table of Contents
 - [How to Build](#how-to-build)
   - [Dependencies](#dependencies)
     - [Library](#library)
     - [Library tests](#library-tests)
   - [Build](#build)
     - [Library only](#library-only)
     - [Library with tests](#library-with-tests)
   - [A note about the tests](#a-note-about-the-tests)
   - [Acquiring test data](#acquiring-test-data)

## How to Build

### Dependencies

#### Library

- CMake 3.15 or newer
- C++ compiler with native C++14 support
  * GCC 5 or newer
  * Clang 3.4 or newer
  * Others not tested but potentially usable: https://en.cppreference.com/w/cpp/compiler_support#cpp14

#### Library tests

- Everything in [previous section](#library)
- GTest
- GraphicsMagick with Magick++
- OpenMP

### Build

- Navigate to project directory.
- Make a build directory and change directory into it.
  * `mkdir build`
  * `cd build`

#### Library only

``` sh
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
```

#### Library with tests

``` sh
    cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=on ..
    make
```

### A note about the tests

The library tests rely on having 2 valid, binary PGS files in the `./test/res` directory. One should be named `subs.sup`, and the other should be named `subs_short.sup`. As the names imply, `subs_short.sup` is an abbreviated version of `subs.sup`.

### Acquiring test data

Getting valid PGS data requires having a legally-obtained copy of a Blu-ray disc ripped to your computer storage. You can use [MakeMKV](https://www.makemkv.com/) to create an MKV file containing any part of the Blu-ray data you want including the subtitles (as PGS data streams). <strong>To make the rest of the process easier, it's best to select only one subtitle stream.</strong>

After MakeMKV has finished processing the file, you can use [FFmpeg](https://ffmpeg.org/) to exract the subtitle stream into its own file.

For example:

``` sh
    ffmpeg -i '<extracted_file>.mkv' -vn -an -c:s copy '<subtitle_file>.sup'
```

Copy or move the extracted file to the project's `./test/res/` directory and rename it to `subs.sup`.

From here, you may either make a copy of `subs.sup` and name it `subs_short.sup` or use a hex editor to create a shorter version of the file that is properly formatted. The main consideration to keep in mind is that the library currently expects proper formatting for the end of the file.

The end of any PGS stream or section will be denoted as such:

| Magic Number (2 bytes) | Decode timestamp (4 bytes) | Presentation timestamp (4 bytes) | Segment type flag (1 byte) | Segment Size (2 bytes) |
| :----------: | :--------------: | :--------------------: | :---------------: | :----------: |
| "PG"         | 32-bit value     | 32-bit value           | 0x80 (END flag)   | 0x00 0x00        |

It is best to seek to between 2-3 megabytes into the file, find the closest `END` section as described above, and remove all data after the `END` section.

Save this new file as `<project_dir>/test/res/subs_short.sup`.
