# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y cmake clang g++

## Add source code to the build stage.

ADD . /
WORKDIR /

## TODO: ADD YOUR BUILD INSTRUCTIONS HERE.
# RUN cmake loguru_example

RUN mv loguru_example e1 # very dumb but was necessary
RUN mkdir build
RUN cd build
RUN CC=clang CXX=clang++ cmake ../e1
RUN make

# Package Stage
FROM --platform=linux/amd64 ubuntu:20.04

## TODO: Change <Path in Builder Stage>
COPY --from=builder /loguru_example /

