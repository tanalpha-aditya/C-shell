
[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description
Mostly same as mentioned in the documents if different mentioned in assumptions.
## Running the Shell

    1. run `make` 
    2. run `./main`

# Assumptions

-> The prompt is colour coded as green

-> peek -l -a can be executed with any number spaces between them but there should be only 1 space between l and a.

-> If pastevents execute calls an index which is more than elements in history it returns an error

-> The pid of background process is printed when spawned and again when it ends(so it might get between the prompts sometime)

-> The '&' for background process should be passed, without spaces, it may not work sometimes with space.

-> havent tested ping for any other signal than 9.

-> calling iman will also print a bit of other data than what was given in assignment.

