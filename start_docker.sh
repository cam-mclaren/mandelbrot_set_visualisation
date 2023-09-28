#!/usr/bin/bash


docker run -it --rm --user 1000:1000  -v ${PWD}:/usr/project --name image_builder image_builder
