#!/bin/bash
# curl to get for example the "Temperature" value :
curl --max-time 5 -s http://test.home/json | grep -Po '"Temperature":"\K[^"]*'
