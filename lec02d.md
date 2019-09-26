# Git, Part Two: Pushing and Pulling

## Review

Last time we saw how to clone a Git repository, make changes, and push
back to the server. This time we’ll see more about how to collaborate
with Git.

First, a command for checking the status of your working copy. This will
tell you which files, if any, have changed:

```console
$ git status
```

When you are satisfied with a change, you need to commit. You should do
this frequently—every time you solve a problem, or even make significant
progress on one. This command will commit all your changes at once:

```console
$ git commit . -m "What did I do??"
```

Then, you know how to upload your changes to the GitHub server:

```console
$ git push
```

## Collaboration

But what if you want to work on more than one computer? Like, what if
you do some work (with your partner, pair programming) on your computer,
and then you want to switch (still pair programming) to using their
computer. To do this, first you need to `git push` your changes on your
computer. Then, to download the changes to your partner’s computer, they
will run:

```console
$ git pull
```

That grabs any changes that have been pushed to GitHub and applies them
to your copy of the repo.

## Concurrent updates

What if you edit the code on two different computers, without a push and
pull in between? That is, what if you have two local copies that differ
from the original on GitHub, but in different ways? You need to find
a way to merge your changes. Here’s how you do it.

Suppose the two computers are A and B. First, push your changes from
computer A:

```console
A$ git push
```

At this point, the copy on GitHub is now *ahead* of the copy on computer
B. So if computer B tries to push, it will be an error.

```console
B$ git push
To https://github.com/nu-ipd/hw01-tov.git
 ! [rejected]        HEAD -> develop (non-fast-forward)
error: failed to push some refs to 'https://github.com/nu-ipd/hw01-tov.git'
```

Instead, on computer B you first need to pull to get the changes that
A made, and then you can push:

```console
B$ git pull
B$ git push
```

Finally, pull again on computer A to get B’s changes:

```console
A$ git pull
```
