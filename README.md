running the shell 

make
./main

# Description
warp - acts like cd
peek - ls 
pastevents 
foreground / background 
proclore
seek

# Assumptions
all the functions of warp as specified in the document are applied.

peek -a <path/name>
peek -l <path/name>
peek -a -l <path/name>
peek -l -a <path/name>
peek -la <path/name>
peek -al <path/name>

in peek, the <path/name> is not working rest all flags are working. 

in pastevents all the commands are working except execute. 

In forground background if the command takes longer than only the last background running command will give msg of completion. 

proclore wroking perfectly

seek not working as of 26th AUG
