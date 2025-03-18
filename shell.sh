#!/bin/bash
echo "Running Memory Usage Analysis..."
free -m | awk 'NR==2{print "Memory Used: "$3"MB / "$2"MB"}' > memory.log
cat memory.log

echo "Plotting Cache Performance Data..."
python3 python.py
