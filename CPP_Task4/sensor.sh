#!/bin/bash

while true; do
    value=$((RANDOM % 1500 + 2000))   # 2000–3499
    echo "$(bc <<< "scale=2; $value/100")" > sensor.txt
    sleep 1
done
