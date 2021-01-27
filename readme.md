# yaul #
![Windows Build](https://github.com/WattsUp/yaul/workflows/Windows%20Build/badge.svg)
![Linux Build](https://github.com/WattsUp/yaul/workflows/Linux%20Build/badge.svg)
yaul (IPA: /jɔːl/, homophone: y'all) is yet another UI library for cross-platform C++ aimed for desktop applications. yaul was designed with the following considerations:
1. It should be Free and Open Source Software.
2. It should be fast to compile for debug builds.
3. It should have automatic UI reloading to allow graphical updates to be made without a RAD tool.
4. It should be resolution independent.
5. It should use modern C++.
6. It should be simple to use with many examples.
7. It should be bloat-free with an emphasis on memory and disk usage.
8. It should be have the same non-native look and feel on all platforms, customizable with CSS (or similar).
9. It should be event based to minimize power consumption.
10. It should be statically or dynamically linked

## Prerequisites ##
* Modern C/C++ compiler
* [git 2.17+](https://git-scm.com/downloads) installed
* [CMake 3.13+](https://cmake.org/download/) installed
* [Clang tools 7.0+](http://releases.llvm.org/download.html) namely clang-format and clang-tidy
* [Python 3.6+](https://www.python.org/downloads/) for tools
* [Doxygen 1.8.17+](http://www.doxygen.nl/download.html) for documentation generation

**Recommended IDE** is [VSCode](https://code.visualstudio.com/) with the following extensions
* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) language support
* [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) integration
* [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake) language support
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)
* [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen)

## Building ##
### Dependencies ###
On Windows I recommend using [vcpkg](https://github.com/Microsoft/vcpkg)
* [spdlog](https://github.com/gabime/spdlog) logging library
* [Google Test](https://github.com/google/googletest) framework
* [Tiny-utf8](https://github.com/DuffsDevice/tiny-utf8) unicode support >4.3
* [TinyXML-2](https://github.com/leethomason/tinyxml2) XML parsing

### Manually Building ###
Configure the project with default compiler and compile
```bash
> cmake . -B build
> cmake --build build
```

### Building in VSCode ###
1. Open the project folder in VSCode with the recommended extensions
2. Open the CMake Tools tab
3. Select `Configure All Projects`, select appropriate compiler
4. Click `Build All Projects`

## Folder Structure ##
* `common`    Common code shared amongst projects: logging, utilities, etc.
* `docs`      Documentation folder
* `docs\www`  Documentation webpage root folder, ignored, clone of repository's gh-pages branch
* `examples`  Collection of examples that use yaul
* `include`   Public include folder for libraries
* `libraries` Third party libraries
* `res`       Runtime resources here (icons, etc.)
* `tools`     Helper code such as check coding conventions script

### Projects ###
* `installer-win` Self-extracting installer executable to install examples
* `project-yaul`  Core library functions for creating a basic application with minimal size
* `project-media` Add-on library for recording/playing audio and recording/playing video
* `project-plot`  Add-on library for plotting data: line, scatter, pie, bar, etc.

### Projects ###
* `examples/3d-scene`       A 3D demo scene with UI overlay
* `examples/calculator`     A calculator with basic, scientific, and programming options
* `examples/character-map`  A utility to show all glyphs of a font
* `examples/clock`          A clock to display the current time with selectable interfaces
* `examples/code-editor`    A text editor with syntactical highlighting
* `examples/hello-world`    The minimum implementation of a yaul GUI
* `examples/hex-editor`     A file editor on a byte level
* `examples/synthesizer`    A digital audio workstation to synthesize sounds using emulated analog circuits

## Documentation branch ##
The `docs/www` folder contains the documentation branch of this repo (`gh-pages` for GitHub). Once the repository has a remote, clone that branch into that folder and use it to build a documentation website.
```bash
git clone https://github.com/WattsUp/yaul.git -b gh-pages docs/www
```

## Known issues ##
* Updates to resource files are not detected by the generator. Perform a clean build if those change. Always perform a clean build for release.