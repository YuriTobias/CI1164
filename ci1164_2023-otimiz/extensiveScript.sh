#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

xe="$1"
input="$2"

time_array=()
bandwidth_array=()
missl2_array=()
energy_array=()
flops_array=()
flops_array_avx=()
sizes=(64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000)
arraySize=${#sizes[@]}

for size in "${sizes[@]}"; do
    time_array+=($(grep "multMatVetTime" output.txt | awk '{print $2}'))
    time_array+=($(grep "multMatMatTime" output.txt | awk '{print $2}'))
    time_array+=($(grep "multMatVetTimeOpt" output.txt | awk '{print $2}'))
    time_array+=($(grep "multMatMatTimeOpt" output.txt | awk '{print $2}'))
    likwid-perfctr -C ${CPU} -g L3 -m ./matmult $size > output.txt
    bandwidth_array+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}'))
    likwid-perfctr -C ${CPU} -g L2CACHE -m ./matmult $size > output.txt
    missl2_array+=($(grep "L2 miss ratio" output.txt | awk '{print $6}'))
    likwid-perfctr -C ${CPU} -g ENERGY -m ./matmult $size > output.txt
    energy_array+=($(grep "Energy \[J\]" output.txt | awk '{print $5}'))
    likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./matmult $size > output.txt
    flops_array+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}'))
    flops_array_avx+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}'))
done

echo "multMatVet time"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 0))
    echo ${sizes[$i]} ${time_array[$index]}
done
echo "multMatVet bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 0))
    echo ${sizes[$i]} ${bandwidth_array[$index]}
done
echo "multMatVet missl2"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 0))
    echo ${sizes[$i]} ${missl2_array[$index]}
done
echo "multMatVet energy"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 0))
    echo ${sizes[$i]} ${energy_array[$index]}
done
echo "multMatVet flops"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 0))
    echo ${sizes[$i]} ${flops_array[$index]}
done
echo "multMatVet flops_avx"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 0))
    echo ${sizes[$i]} ${flops_array_avx[$index]}
done
echo
echo "multMatMat time"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 1))
    echo ${sizes[$i]} ${time_array[$index]}
done
echo "multMatMat bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 1))
    echo ${sizes[$i]} ${bandwidth_array[$index]}
done
echo "multMatMat missl2"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 1))
    echo ${sizes[$i]} ${missl2_array[$index]}
done
echo "multMatMat energy"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 1))
    echo ${sizes[$i]} ${energy_array[$index]}
done
echo "multMatMat flops"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 1))
    echo ${sizes[$i]} ${flops_array[$index]}
done
echo "multMatMat flops_avx"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 1))
    echo ${sizes[$i]} ${flops_array_avx[$index]}
done
echo
echo "multMatVetOpt time"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 2))
    echo ${sizes[$i]} ${time_array[$index]}
done
echo "multMatVetOpt bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 2))
    echo ${sizes[$i]} ${bandwidth_array[$index]}
done
echo "multMatVetOpt missl2"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 2))
    echo ${sizes[$i]} ${missl2_array[$index]}
done
echo "multMatVetOpt energy"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 2))
    echo ${sizes[$i]} ${energy_array[$index]}
done
echo "multMatVetOpt flops"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 2))
    echo ${sizes[$i]} ${flops_array[$index]}
done
echo "multMatVetOpt flops_avx"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 2))
    echo ${sizes[$i]} ${flops_array_avx[$index]}
done
echo
echo "multMatMatOpt time"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 3))
    echo ${sizes[$i]} ${time_array[$index]}
done
echo "multMatMatOpt bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 3))
    echo ${sizes[$i]} ${bandwidth_array[$index]}
done
echo "multMatMatOpt missl2"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 3))
    echo ${sizes[$i]} ${missl2_array[$index]}
done
echo "multMatMatOpt energy"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 3))
    echo ${sizes[$i]} ${energy_array[$index]}
done
echo "multMatMatOpt flops"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 3))
    echo ${sizes[$i]} ${flops_array[$index]}
done
echo "multMatMatOpt flops_avx"
for (( i=0; i<${arraySize}; i++ )); do
    index=$((i * 4 + 3))
    echo ${sizes[$i]} ${flops_array_avx[$index]}
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor