#!/bin/bash

function test_command () {
  cmd_result=$(eval $1)
  if [ $cmd_result = $2 ]; then
    echo -e "\e[32m[success]\e[m $1 = $2 ($cmd_result)"
  else
    echo -e "\e[31m[fail]\e[m $1 = $2 ($cmd_result)"
    exit 1
  fi
}
