## UID: 605691503
(IMPORTANT: Only replace the above numbers with your true UID, do not modify spacing and newlines, otherwise your tarfile might not be created correctly)

## Pipe Up

A program that implements the Linux piping feature, ```|```. It pipes together various programs such that the output of one command is used as the input to the next.


## Building

Build the executable by running ```make```.

## Running

Run the program using ```./pipe```, followed by commands you would like to pipe together. For example, running ```./pipe ls wc``` should give you the word count information (lines, words, and characters) of the output of ```ls```. This should be the same output as running ```ls | wc```.

## Cleaning up

Clean up all binary files by running ```make clean```.