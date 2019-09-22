# IPD Setup: Racket, Git, and GitHub

Before we can get started we need to install our development
environment. You’ll need Racket 7.4 for programming and Git for version
control.

[Racket download]:
    https://download.racket-lang.org/

[Git download]:
    https://git-scm.com/download/win/

[CLT]:
    https://developer.apple.com/downloads/

[GitHub sign-up]:
    https://github.com/join

## Downloads

For all platforms, you will need:

  - [Racket download]

Additional downloads vary by platform.

## GitHub Registration

### Mac OS

You most likely already have Git, but you may need to enable it (below).

### Windows

You will need to download:

   - [Git download]

### Linux, etc.

Make sure you have a recent Git.

## Setup

### All platforms

 1. You need to have a GitHub account. If you don’t already have one
    then you should [sign up here][GitHub sign-up].

### Mac OS

Mac OS automatically installs its toolchain, including Git, when you
attempt to use it from the command line for the first time.

 1. Thus, to install developer tools, run the *Terminal* program (from
    `/Applications/Utilities` or `/System/Applications/Utilities`) to
    get a command prompt. At the prompt, type `git version` and press
    return. If you haven’t used the command-line developer tools before
    then a dialog box will pop up and offer to install them for you. Say
    yes.
    
    Once you have Git set up, you should run the command again. The
    result should look like this (but possibly with different names and
    Git version):
    
    ```console
    [mannequin:~] tov% git version
    git version 2.21.0 (Apple Git-122)
    [mannequin:~] tov% 
    ```
    
    If it prints `git: Command not found.` then you may have a problem.
    You can try installing the latest version of *Command Line Tools for
    XCode* manually [from Apple][CLT], or install *XCode* from the App
    Store.)

 2. For Racket, follow the standard Mac OS `.dmg` process: open the
    `.dmg` file; drag the `Racket v7.4` folder inside into the
    `Applications` folder linked inside. Then eject the disk image and
    delete it.
    
    Make sure you can run DrRacket, which should be installed as
    `Applications/Racket v7.4/DrRacket.app`.

### Windows

On Windows, you need to install Git and Racket separately:

 1. Run the [Git installer][Git download]. Click “Next” for each
    question until “Adjusting your PATH environment” appears. Select
    “Use Git from the Windows Command Prompt” instead of the default
    option. Then continue clicking “Next” until the installation
    completes.

    To test that Git is installed properly, open a command prompt,
    type the command `git version` and press return. Git should reply
    with version information:

    ```PowerShell
    PS C:\Users\jesse> git version
    git version 2.20.1.windows.1
    PS C:\Users\jesse> 
    ```

 2. Run the [Racket installer][Racket download]. Then make sure you can
    run DrRacket.

