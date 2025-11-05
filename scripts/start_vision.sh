#!/bin/bash

cd `dirname $0`
cd ..

export FASTRTPS_DEFAULT_PROFILES_FILE=./configs/fastdds.xml

ros2 launch vision launch.py