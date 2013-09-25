#!/bin/bash

# Dependencies for cinekinect

cd ../

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}neilmendoza/ofxPostProcessing.git