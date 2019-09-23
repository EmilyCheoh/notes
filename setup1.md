# IPD Setup: GitHub, Git, and Racket

Before we can get started we need to install our development
environment. You’ll need Racket 7.4 for programming and Git for version
control.

[CLT]:
    https://developer.apple.com/downloads/

[Git installer]:
    https://git-scm.com/download/win/

[GitHub sign-up]:
    https://github.com/join

[Homebrew]:
    https://brew.sh/

[Racket installer]:
    https://download.racket-lang.org/

## GitHub Registration

You need to have a GitHub account. If you don’t already have one then
you should [sign up here][GitHub sign-up]. You might want to do it while
files are downloading (below).

## Software Setup

Setup instructions vary by platform:

  - [Linux, etc.](#linux-etc)

  - [Mac OS](#mac-os)

  - [Windows](#windows)


## Windows

You will need to download:

  - [Git installer]

  - [Racket installer]

### Installing Git

Run the [Git installer]. Click “Next” for each
question until “Adjusting your PATH environment” appears. Select
“Use Git from the Windows Command Prompt” instead of the default
option. Then continue clicking “Next” until the installation
completes.

To test that Git is installed properly, open a command prompt,
type the command `git version` and press return. Git should reply
with version information:

```
C:\Users\jesse> git version
git version 2.20.1.windows.1
C:\Users\jesse> 
```

### Installing Racket

Run the [Racket installer]. Then make sure you can run
DrRacket.


## Mac OS

### Enabling Git

Mac OS automatically installs its toolchain, including Git, when you
attempt to use it from the command line for the first time.

Thus, to install Git, run the *Terminal* program (from
`/Applications/Utilities` or `/System/Applications/Utilities`) to get a
command prompt. At the prompt, type `git version` and press return. If
you haven’t used the command-line developer tools before then a dialog
box will pop up and offer to install them for you. Say yes.

Once you have Git set up, you should run the command again. The
result should look like this (but possibly with different names and
Git version):

```console
[mannequin:~] tov$ git version
git version 2.21.0 (Apple Git-122)
[mannequin:~] tov$ 
```

If it prints `git: Command not found.` then you may have a problem.
You can try installing the latest version of *Command Line Tools for
XCode* manually [from Apple][CLT], or install *XCode* from the App
Store.)

### Installing Racket via Homebrew

We are going to install Racket via [Homebrew], a Mac OS package manager
that we’ll use to install other software that we need later in the
course.

To install Homebrew, (re)open the terminal and run the command below
(from `ruby` to the end of the line):

```console
[mannequin:~] tov$ ruby <(curl -fsSL https://bit.ly/1LEgSWs)
==> This script will install:
/usr/local/bin/brew
/usr/local/share/doc/homebrew
⋮
- Run `brew help` to get started
- Further documentation: 
    https://docs.brew.sh
[mannequin:~] tov$ 
```

Then use Homebrew to install Racket:

```console
[mannequin:~] tov$ brew cask install racket
Updating Homebrew...
==> Auto-updated Homebrew!
⋮
🍺  racket was successfully installed!
[mannequin:~] tov$ 
```

Make sure you can run DrRacket, which should be installed as
`/Applications/Racket v7.4/DrRacket.app`.


## Linux, etc.

Make sure you have a recent Git.

Install Racket 7.4 either via your preferred package manager or from
[the Racket website][Racket installer].

