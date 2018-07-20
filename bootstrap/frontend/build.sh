#!/bin/sh

cd $(dirname $0)

[ -f duskc.jar ] && rm duskc.jar
kotlinc src/**.kt -include-runtime -d duskc.jar
