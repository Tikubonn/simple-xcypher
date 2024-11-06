#!/bin/bash

cd `dirname $0`
source ./test.bash

test_command 'echo -n "Hello." | simple-xcypher -k 0x123 | simple-xcypher -d -s 6 -k 0x123' 'Hello.'
test_command 'echo -n "Hello." | simple-xcypher -e -k 0x123 | simple-xcypher -d -s 6 -k 0x123' 'Hello.'
test_command 'echo -n "Hello." | simple-xcypher -e -k 0x123 | simple-xcypher -d -s 3 -k 0x123' 'Hel'
test_command 'echo -n "Hello." | simple-xcypher -e -k 0x123 | simple-xcypher -d -p 3 -s 3 -k 0x123' 'lo.'
