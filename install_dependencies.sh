#!/bin/bash

echo "Installing dependencies"

sudo apt-get install build-essential -y; # gcc, make, etc
sudo apt-get install bison flex -y; # lex

echo "Done!"
