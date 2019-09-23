# Intro to Git & Homework Mechanics

Hopefully, everyone is on Campuswire? If not, you should have an email
message that has the link so you can join.

All of the handins will be via git this year in IPD. To see how this
is going to work, lets do a fake homework.

But first, let me explain git to you.

The way to think of git is that it tracks the history of some code for
you. One way to think of this is that it is an entire set of tools
whose only job is "undo". Yes, undo has left the editor and become its
own thing!

You can think of the history of your code as a series of discrete
moments in time:

```
  o -- o -- o -- o -- o
```

each moment is called a "commit" and you have to explicitly tell the
tool "okay, this is a moment". You do that with "git commit". Each of
these commits represents the entire state of the project at that
moment. It isn't just a diff or something -- think of each bubble as
its own entire copy of everything (of course, things are stored more
efficiently internally but we don't have to care about that). The
commits also have a special code associated with them, called a
"sha". It looks like a long base16 number, e.g.
e8f9386132c99b5b390ad9cb45a2b11758e18bfd.

You can go back and forth and examine the history of your project
using git. And, of course, if you go back to an earlier version and
then do some commits, then you end up with a history that looks like
this:

```
  o -- o -- o -- o -- o
        \
         \
          o -- o -- o
```

or even this:

```
               o -- o -- o
              /
             /
  o -- o -- o -- o -- o
        \
         \
          o -- o -- o
                \
                 \
                  o -- o -- o

```

Note that there is NO WAY to modify this tree. You can only add things
to it. There is also a notion of garbage collection, where you can
"forget" parts of the tree. But that's a detail. Morally, the tree is
immutable. It just exists.

In order to help you keep track of all of this history, you can
associate names with specific spots in the history. The most common
name is "master". These names are kept in a table, telling you which
commit goes to the name. These names are mutable. So you can change
which commit a particular name refers to. Eg on one day you might have
something like this:

```
  o -- o -- o -- o -- o
                     master
```

where "master" is referring to the latest version of the code. Then,
you do some more work and make some more commits and, as part of that,
you move the master "pointer" to the (new) latest commit:

```
  o -- o -- o -- o -- o -- o -- o -- o
                                    master
```

You can also share your commits with others. You do that by
"push"ing. A push means: "take the state as it currently exists on my
machine and send it out to another place".

Sometimes this will be illegal and the machine on the other end will
reject your attempt. For example, you and someone else at your company
might both start at the same place and do some work
independently. Then you colleague pushes and then you try to push. At
that point, if the remote machine you are pushing to were to accept
your work, then your colleague's would get lost. So, instead, it
rejects it. Here's where those forked histories come into play.

```
 server has:
   o -- o -- o -- o -- o
                      master

 you have:
   o -- o -- o -- o 
                master
```

and lets say that the first three commits are the same on both, but
your colleague added one more on the end. Instead of losing your work,
you can ask the server to bring your work down to your machine and it
will do so, producing a history like this one:


```
   o -- o -- o -- o
              \  master
               \
                o -- o
                    origin/master
```

That "origin" in there is another naming convention. It could be
anything, but typically we all use a single machine to share stuff and
that machine has the symbolic name (in gitland) "origin".

Once you have that you do various different things to resolve the
problem. The one I'll just mention now is that you can ask git to
please take the diff of your changes and then apply it to the state
your colleague pushed. If the changes you made and the ones they made
didn't conflict then you'll get a history that looks like this:


```
   o -- o -- o -- o
              \  
               \         master
                o -- o -- o
                 origin/master
```

where "master" has moved over to be one past where it was before and
now the "origin" server will let you push your changes. Probably.

It doesn't have to allow you at this point, of course. There are other
rules that might be in play.

For example, for our homeworks, you will not be allowed to push to
master at all. The server won't accept any attempted changes to
origin/master. Instead you have to push to the "develop" branch.

Okay, enough overview. Lets walk through how you'll work with git to
do your homework.

Open up [this link](https://classroom.github.com/a/ud3U6eXy).

You should have a new repository now. Click on the green "clone or
download" button and you'll see a url. Copy it.

Go to your shell and run the command:

```
git clone <paste the url here>
```

You should get a copy of some files in a directory. Open the file
`good.rkt` in DrRacket. Hit run and see it works. Open the file
bad.rkt in DrRacket and hit. See it doesn't work.

Okay, lets see it fail in CI (continuous integration). To do that, we
need to push to a new branch called `develop`. Go back to your shell
and use `cd` to get into the directory that you checked out. Then run
this command:

```
git checkout -b develop
```

This creates a new local branch whose name is `develop` and switches
your git around so that you're using that branch now by default
(instead of using master).

Next, lets push that branch back to the origin (i.e. github). Here's
the command to do that.

```
git push --set-upstream origin develop
    ## only have to add the arguments this first time
```

Github will notice the push and will start a continuous integration
run of your code. That will download and install racket on some server
somewhere and then run the tests in your code. It will just take a
minute (even less actually).

To see the results, go back to the webpage that had the green button
you clicked on earlier. Look for the button labeled `branch:
master`. Click on it and you should be able to choose the branch
develop. After that, click on the actions tab (up and to the right of
the "branch" button). Click on "develop" (the name of the branch).
Click on "run tests" to see the test failure. It should look similar
to how it looked in DrRacket.

Before we fix it, lets look at the test coverage. Click the
"artifacts" thingy in the upper right (ish -- just above the black
region). You should be able to downlaod coverage.zip.  Inside it,
you'll see a lot of green. That means that those portions of the code
were run.

Okay _now_ lets fix the bug. After you fix it, you should run this
command (same place where you ran the `git push` before):

```
git commit .
```

it should pop up an editor to let you write a commit message saying
what you did. Write something and then save the file and exit the
editor. After that:

```
git push
```

(you don't need the extra arguments this time; git remembers the name
of the default branch, but locally and on the server for you). You
should see a second push on the server, this time with all tests
passing.

