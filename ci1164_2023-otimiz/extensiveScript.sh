#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

xe="$1"
input="$2"

sizes=(64 100)
#sizes=(64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000)
arraySize=${#sizes[@]}

echo > ./results/time_MV.txt
echo > ./results/bandwidth_MV.txt
echo > ./results/missl2_MV.txt
echo > ./results/energy_MV.txt
echo > ./results/flops_MV.txt
echo > ./results/flops_avx_MV.txt
echo > ./results/time_MV_UJ.txt
echo > ./results/bandwidth_MV_UJ.txt
echo > ./results/missl2_MV_UJ.txt
echo > ./results/energy_MV_UJ.txt
echo > ./results/flops_MV_UJ.txt
echo > ./results/flops_avx_MV_UJ.txt
echo > ./results/time_MV_UJB.txt
echo > ./results/bandwidth_MV_UJB.txt
echo > ./results/missl2_MV_UJB.txt
echo > ./results/energy_MV_UJB.txt
echo > ./results/flops_MV_UJB.txt
echo > ./results/flops_avx_MV_UJB.txt
echo > ./results/time_MM.txt
echo > ./results/bandwidth_MM.txt
echo > ./results/missl2_MM.txt
echo > ./results/energy_MM.txt
echo > ./results/flops_MM.txt
echo > ./results/flops_avx_MM.txt
echo > ./results/time_MM_UJ.txt
echo > ./results/bandwidth_MM_UJ.txt
echo > ./results/missl2_MM_UJ.txt
echo > ./results/energy_MM_UJ.txt
echo > ./results/flops_MM_UJ.txt
echo > ./results/flops_avx_MM_UJ.txt
echo > ./results/time_MM_UJB.txt
echo > ./results/bandwidth_MM_UJB.txt
echo > ./results/missl2_MM_UJB.txt
echo > ./results/energy_MM_UJB.txt
echo > ./results/flops_MM_UJB.txt
echo > ./results/flops_avx_MM_UJB.txt

for size in "${sizes[@]}"; do
    echo "Testing size $size"
    likwid-perfctr -C ${CPU} -g L3 -m ./matmult $size > output.txt
    echo "$(grep "multMatVetTime:" output.txt | awk '{print $2}')" >> ./results/time_MV.txt
    echo "$(grep "multMatMatTime:" output.txt | awk '{print $2}')" >> ./results/time_MM.txt
    echo "$(grep "multMatVetTimeUJ:" output.txt | awk '{print $2}')" >> ./results/time_MV_UJ.txt
    echo "$(grep "multMatMatTimeUJ:" output.txt | awk '{print $2}')" >> ./results/time_MM_UJ.txt
    echo "$(grep "multMatVetTimeUJB:" output.txt | awk '{print $2}')" >> ./results/time_MV_UJB.txt
    echo "$(grep "multMatMatTimeUJB:" output.txt | awk '{print $2}')" >> ./results/time_MM_UJB.txt
    echo "$(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/bandwidth_MV.txt
    echo "$(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/bandwidth_MM.txt
    echo "$(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==3')" >> ./results/bandwidth_MV_UJ.txt
    echo "$(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==4')" >> ./results/bandwidth_MM_UJ.txt
    echo "$(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==5')" >> ./results/bandwidth_MV_UJB.txt
    echo "$(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==6')" >> ./results/bandwidth_MM_UJB.txt
    likwid-perfctr -C ${CPU} -g L2CACHE -m ./matmult $size > output.txt
    echo "$(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/missl2_MV.txt
    echo "$(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/missl2_MM.txt
    echo "$(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==3')" >> ./results/missl2_MV_UJ.txt
    echo "$(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==4')" >> ./results/missl2_MM_UJ.txt
    echo "$(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==5')" >> ./results/missl2_MV_UJB.txt
    echo "$(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==6')" >> ./results/missl2_MM_UJB.txt
    likwid-perfctr -C ${CPU} -g ENERGY -m ./matmult $size > output.txt
    echo "$(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==1')" >> ./results/energy_MV.txt
    echo "$(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==2')" >> ./results/energy_MM.txt
    echo "$(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==3')" >> ./results/energy_MV_UJ.txt
    echo "$(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==4')" >> ./results/energy_MM_UJ.txt
    echo "$(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==5')" >> ./results/energy_MV_UJB.txt
    echo "$(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==6')" >> ./results/energy_MM_UJB.txt
    likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./matmult $size > output.txt
    echo "$(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==1')" >> ./results/flops_MV.txt
    echo "$(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==2')" >> ./results/flops_MM.txt
    echo "$(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==3')" >> ./results/flops_MV_UJ.txt
    echo "$(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==4')" >> ./results/flops_MM_UJ.txt
    echo "$(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==5')" >> ./results/flops_MV_UJB.txt
    echo "$(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==6')" >> ./results/flops_MM_UJB.txt
    echo "$(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==1')" >> ./results/flops_avx_MV.txt
    echo "$(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==2')" >> ./results/flops_avx_MM.txt
    echo "$(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==3')" >> ./results/flops_avx_MV_UJ.txt
    echo "$(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==4')" >> ./results/flops_avx_MM_UJ.txt
    echo "$(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==5')" >> ./results/flops_avx_MV_UJB.txt
    echo "$(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==6')" >> ./results/flops_avx_MM_UJB.txt
done

rm output.txt

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor