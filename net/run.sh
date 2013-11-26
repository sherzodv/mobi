#!/bin/sh
make

rm *log
fastmqr/fastmqr

grep --color --binary-files=text 'chan A' *log
echo
grep --color --binary-files=text 'chan B' *log
echo
grep --color --binary-files=text 'chan S' *log
echo
grep --color --binary-files=text 'error' *log

