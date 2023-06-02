#!bin/bash

mkfifo pipe1 pipe2

./bin/1048.exe < pipe1 > pipe2 & ./bin/oj.exe ./res/input.txt ./res/output.txt ./res/log.txt < pipe2 > pipe1