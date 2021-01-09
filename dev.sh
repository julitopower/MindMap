#!/bin/bash
ROOT="$( cd "$(dirname "$0")" ; pwd -P)"

docker run -it --gpus all --rm -v $ROOT:/opt/src julitopower/dockercppdevemacsclang:gcc9 /bin/bash
