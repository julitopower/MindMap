# MindMap
Application to graphical MindMaps  from descriptions written in org-mode inspired syntax

# Features

* Read org-syntax

```org
* Machine Learning
** Categories
*** Supervised
*** Unsupervised
*** Reinforcement Learning
*** Others
**** Semi-supervised
**** Transfer learning
** Classic
*** Logistic regression
** Deep Learning
*** Computer vision
*** Natural language understanding
*** Deep refinforcement learning
```

* Output png
* Nodes have shape, color
* Edges have line style, color
* Generate debian package
* Publish to conda
* Publish to apt

# Build

## Requirements

* CMake-3.2
* graphviz-2.40
* gtest-1.10

A fully function build environment can be built using Conda and the provided ```dev.yml``` Conda environment definition file:

```shell
conda create -f=dev.yml
```

## How to build

Execute the following commands on the package root folder (the one that contains this README.md file):

```shell
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
```

Run the tests:

```shell
make test
```

Generate a debian package:

```shell
make package
```

## Build options

* BUILD_TEST: Default ON. Build tests?

For instace, to not build the tests:

```shell
cmake .. -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release
```
