#!/bin/bash

CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# sizes=(64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 1000000, 10000000, 100000000, 1000000000)
sizes=(64 128)
arraySize=${#sizes[@]}

mkdir results > /dev/null 2>&1
echo "#" > ./results/time_O.dat
echo "#" > ./results/flops_O.dat
echo "#" > ./results/flops_avx_O.dat
echo "#" > ./results/bandwidth_O.dat
echo "#" > ./results/missl2_O.dat

for size in "${sizes[@]}"; do
    echo "Testing size $size"
    ./geraEntrada $size | likwid-perfctr -C ${CPU} -g L3 -m ./ajustePol > output.txt
    echo "$size $(grep "OpTime: " output.txt | awk '{print $2}')" >> ./results/time_O.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/bandwidth_O.dat
    ./geraEntrada $size | likwid-perfctr -C ${CPU} -g L2CACHE -m ./ajustePol > output.txt
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/missl2_O.dat
    ./geraEntrada $size | likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./ajustePol > output.txt
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==1')" >> ./results/flops_O.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==1')" >> ./results/flops_avx_O.dat
done

rm output.txt

echo "Generating plots"
gnuplot -c plot.gp "Time" "results/time_O.png" "results/time_O.dat" "Optimized" > /dev/null 2>&1
gnuplot -c plot.gp "Flops" "results/flops_O.png" "results/flops_O.dat" "Optimized" > /dev/null 2>&1
gnuplot -c plot.gp "Flops" "results/flops_avx_O.png" "results/flops_avx_O.dat" "Optimized" > /dev/null 2>&1
gnuplot -c plot.gp "Bandwidth" "results/bandwidth_O.png" "results/bandwidth_O.dat" "Optimized" > /dev/null 2>&1
gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_O.png" "results/missl2_O.dat" "Optimized" > /dev/null 2>&1

echo "Removing temporary files"
# rm results/*.dat

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor