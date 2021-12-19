#!/bin/sh
cmake --build ./build $@ && pcsxr -cdfile build/template.cue
