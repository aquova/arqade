#!/bin/bash

# Generates the compile_commands.json file for clangd
# Requires 'bear' to be installed
make clean;
bear -- make;
