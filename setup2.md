# IPD C++ Setup

Before we can get started we need to install our development
environment. You’ll need a C++ compiler, the CLion IDE, the SDL2
graphics libraries, and Git for version control.

[SDL2 Mac]:
    https://users.eecs.northwestern.edu/~jesse/course/eecs211/files/SDL2-all.dmg

[MinGW]:
    https://users.eecs.northwestern.edu/~jesse/course/eecs211/files/MinGW-SDL2.exe

[CLion download]:
    https://www.jetbrains.com/clion/download

[Git download]:
    https://git-scm.com/download/win

[CLT]:
    https://developer.apple.com/downloads/

[JetBrains student]:
    https://www.jetbrains.com/shop/eform/students

## Downloads

For all platforms, you will need:

  - [CLion installer][CLion download]

Additional installation varies by platform.

### Mac OS

You will need to download:

  - Our custom [SDL2 disk image][SDL2 Mac]

### Windows

You will need to download:

   - Our custom [installer for MinGW-w64 with SDL2][MinGW]

   - [Git installer][Git download]

### Linux, etc.

Make sure you have Git and a working C++14 toolchain installed. You
should also install the development packages for SDL2, SDL2_image,
SDL2_ttf, and SDL2_mixer.

## Setup

### Mac OS

Mac OS automatically installs its toolchain, including Git, when you
attempt to use it from the command line for the first time; you will
still have to install the SDL2 libraries yourself.

 1. Thus, to install developer tools, run the *Terminal* program (from
    `Applications/Utilities`) to get a command prompt. At the prompt,
    type

        clang

    and press return. If it prints `clang: error: no input files` then you
    have it installed already. Otherwise, a dialog box will pop up and offer
    to install the command-line developer tools for you. Say yes.

    (Alternatively, you can install the latest version of *Command Line
    Tools for OS X* manually [from Apple][CLT], or install *XCode* from
    the App Store.)

 2. Once you have the developer tools installed, you need to install
    the SDL2 libraries. Open the [`SDL2-all.dmg`][SDL2 Mac] disk image
    and drag the four frameworks into the linked `/Library/Frameworks`
    directory. You may have to authenticate as an administrator.

### Windows

On Windows, you need to install MinGW-w64 (the C++ compiler) and Git
separately:

 1. Run the [`MinGW-SDL2.exe` installer][MinGW] and follow the prompts
    to install MinGW—w64. You should usually install it to `C:\MinGW`,
    but wherever you install it, take note, as you will have to
    configure CLion to find it later.

 2. Run the [Git installer][Git download]. Click “Next” for each
    question until “Adjusting your PATH environment” appears. Select
    “Use Git from the Windows Command Prompt” instead of the default
    option. Then continue clicking “Next” until the installation
    completes.

### All platforms

On all platforms you will need to follow these steps to set up the CLion
IDE:

 1. Register for a student JetBrains account [on their
    website][JetBrains student].

 2. Follow the instructions in your email to activate your account.

 3. Run the [CLion installer][CLion download]—defaults should be fine.
    (Windows: Check all of the “Create associations” boxes when they
    appear.)

 4. Windows only: Set the toolchain in CLion to the location where you
    installed MinGW. The folder you select should contain subfolders
    with names like `bin` and `lib`. Ignore any warnings about version
    numbers.

