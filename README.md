# TreeProcExplorer

## What is it? 

It's a process explorer which visually displays the processes in their actual tree hierarchy.

It's meant to display all active processes in the current state of the OS and lead to a better graphical representation of this list, as opposed to the classical process explorer.

## How does it look?

TODO: Add a video here

## How was it made?

It has been made using pure C language, and a litte bit of GNU Make - to package it all up.

Tested inside Ubuntu 24.04.4 LTS.

## How does it work? 

The project is separated in 4 big components:

  1. The process_monitor directory - which determines all active processes
  2. The translation_between_forms directory - which adapts the obtained "parent array" to a "dynamic N-way tree" data structure
  3. The graphics directory - which uses said "dynamic N-way tree" to illustrate the Process Forest onto the screen
  4. The main.c file - which runs the raylib runtime loop and continuously fetches the active processes

The graphics directory contains a compiled version of the Raylib library.


## How can I use it? 

Just clone the repo by pasting 

```bash

  git clone https://github.com/DariusSasarman/TreeProcExplorer

```

And then 

```bash

  cd /TreeProcExplorer

  make

  ./TreeProcExplorer

```

You should see it run! 

If you encounter problems running it, several causes could be:
  1. The operating system - can't help you there
  2. The compiled raylib library - just replace the contents of "raylib_files" with a compiled version of https://github.com/raysan5/raylib

## Why?

I wanted to add a twist to the classical "pretty print" process explorer.

It was a great occasion to make meaningful software using the C programming language

and an awesome opportunity to explore the raylib ecosystem.
