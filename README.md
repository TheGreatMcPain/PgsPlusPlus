[![iAmSomeone2](https://circleci.com/gh/iAmSomeone2/PgsPlusPlus/tree/main.svg?style=svg)](https://app.circleci.com/pipelines/github/iAmSomeone2/PgsPlusPlus)
# Pgs++

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
  - [Generate Packages](#generate-packages)
    - [RPM](#rpm)
      - [Configuration](#configuration)
      - [Packaging](#packaging)

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

## Generate Packages

Pgs++ can be packed for various OSes to make system and project integration easier.

### RPM

Currently, Pgs++ has support for building native RPM packages on any architecture provided it has the following packages along with the minimum requirements listed [here](#library):

- rpm-build
- rpmlint
- python
- coreutils
- diffutils
- patch
- rpmdevtools

To install all base requirements on Fedora, CentOS 8, and RedHat Enterprise Linux 8, use the following command:

``` sh
dnf install gcc cmake rpm-build rpm-devel rpmlint python bash coreutils diffutils patch rpmdevtools
```

#### Configuration

Pgs++ uses CMake to generate an RPM SPEC file based on the template located in `<project_dir>/dist/redhat`. To have CMake generate the file, simply create a fresh build directory or use one configured for a release build, enter it, and run CMake from there.

<strong>Example:</strong>
``` sh
mkdir build-rpm && cd build-rpm
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=off -DEXPORT_HEADERS=on ..
```
or
``` sh
cd build-release
cmake ..
```

At this point a new SPEC file will have been created at `<project_dir>/dist/redhat/pgs++.spec`. Take note of this location for the next step.

#### Packaging

If you have not created RPM packages before, use the following command to generate the required directories in your `HOME` directory:

``` sh
rpmdev-setuptree
```

From here, either copy the generated SPEC file from the previous step to `$HOME/rpmbuild/SPECS/` or create a symlink in the same directory.

Open a terminal in `$HOME/rpmbuild/SPECS/` and enter the following command to download the required sources to `$HOME/rpmbuild/SOURCES/`:

``` sh
spectool -g -R pgs++.spec
```

Next, run the following command to build the code and generate the RPM packages:

``` sh
rpmbuild -bb pgs++.spec
```

If the command is successful, a freshly-generated set of Pgs++ packages will be waiting at `$HOME/rpmbuild/RPMS/`
