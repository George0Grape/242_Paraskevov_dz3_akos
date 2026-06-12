#!/bin/bash

sum() {
    local sum=0
    for arg; do
        num=$(echo "$arg" | bc -l 2>/dev/null)
        if [ $? -eq 0 ]; then
            sum=$(echo "$sum + $num" | bc -l)
        else
            return 1
        fi
    done
    echo "$sum"
}

read -r line1
read -r line2
sum1=$(sum $line1 2>/dev/null)
sum1_exit=$?
sum2=$(sum $line2 2>/dev/null)
sum2_exit=$?
if [ "$sum1" = "$sum2" ] || [ \( "$sum1_exit" -ne 0 \) -a \( "$sum2_exit" -ne 0 \) ]; then
    echo "Equal"
else
    echo "Not equal"
fi
