#!/bin/sh

PSN00BSDK_LIBS=/usr/local/lib/libpsn00b/
cmake --build ./build $@ && pcsxr -cdfile build/template.cue
