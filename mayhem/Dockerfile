# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y cmake clang g++

## Add source code to the build stage.
ADD . /loguru
WORKDIR /loguru

## TODO: ADD YOUR BUILD INSTRUCTIONS HERE.
RUN mkdir build && \
    cd build/ && \
    CC=clang CXX=clang++ cmake ../loguru_example && \
    make

# Package Stage
FROM --platform=linux/amd64 ubuntu:20.04

## TODO: Change <Path in Builder Stage>
COPY --from=builder /loguru/build/loguru_example /

