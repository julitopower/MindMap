#!/bin/bash
ROOT="$( cd "$(dirname "$0")" ; pwd -P)"

docker run -it --rm -v $ROOT:/opt/src -w /opt/src julitopower/dockercppdevemacsclang:gcc9 /bin/bash
