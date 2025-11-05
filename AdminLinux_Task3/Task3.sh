#!/bin/bash

echo 'export HELLO=$HOSTNAME' >> ~/.bashrc
echo 'LOCAL=$(whoami)' >> ~/.bashrc
gnome-terminal &

