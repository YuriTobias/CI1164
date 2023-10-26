#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

xe="$1"
input="$2"

sizes=(64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000)
arraySize=${#sizes[@]}

echo "#" > ./results/time_MV.dat
echo "#" > ./results/bandwidth_MV.dat
echo "#" > ./results/missl2_MV.dat
echo "#" > ./results/energy_MV.dat
echo "#" > ./results/flops_MV.dat
echo "#" > ./results/flops_avx_MV.dat
echo "#" > ./results/time_MV_UJB.dat
echo "#" > ./results/bandwidth_MV_UJB.dat
echo "#" > ./results/missl2_MV_UJB.dat
echo "#" > ./results/energy_MV_UJB.dat
echo "#" > ./results/flops_MV_UJB.dat
echo "#" > ./results/flops_avx_MV_UJB.dat
echo "#" > ./results/time_MM.dat
echo "#" > ./results/bandwidth_MM.dat
echo "#" > ./results/missl2_MM.dat
echo "#" > ./results/energy_MM.dat
echo "#" > ./results/flops_MM.dat
echo "#" > ./results/flops_avx_MM.dat
echo "#" > ./results/time_MM_UJB.dat
echo "#" > ./results/bandwidth_MM_UJB.dat
echo "#" > ./results/missl2_MM_UJB.dat
echo "#" > ./results/energy_MM_UJB.dat
echo "#" > ./results/flops_MM_UJB.dat
echo "#" > ./results/flops_avx_MM_UJB.dat

for size in "${sizes[@]}"; do
    echo "Testing size $size"
    likwid-perfctr -C ${CPU} -g L3 -m ./matmult $size > output.txt
    echo "$size $(grep "multMatVetTime:" output.txt | awk '{print $2}')" >> ./results/time_MV.dat
    echo "$size $(grep "multMatMatTime:" output.txt | awk '{print $2}')" >> ./results/time_MM.dat
    echo "$size $(grep "multMatVetTimeUJB:" output.txt | awk '{print $2}')" >> ./results/time_MV_UJB.dat
    echo "$size $(grep "multMatMatTimeUJB:" output.txt | awk '{print $2}')" >> ./results/time_MM_UJB.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/bandwidth_MV.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/bandwidth_MM.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==5')" >> ./results/bandwidth_MV_UJB.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==6')" >> ./results/bandwidth_MM_UJB.dat
    likwid-perfctr -C ${CPU} -g L2CACHE -m ./matmult $size > output.txt
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/missl2_MV.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/missl2_MM.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==5')" >> ./results/missl2_MV_UJB.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==6')" >> ./results/missl2_MM_UJB.dat
    likwid-perfctr -C ${CPU} -g ENERGY -m ./matmult $size > output.txt
    echo "$size $(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==1')" >> ./results/energy_MV.dat
    echo "$size $(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==2')" >> ./results/energy_MM.dat
    echo "$size $(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==5')" >> ./results/energy_MV_UJB.dat
    echo "$size $(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==6')" >> ./results/energy_MM_UJB.dat
    likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./matmult $size > output.txt
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==1')" >> ./results/flops_MV.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==2')" >> ./results/flops_MM.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==5')" >> ./results/flops_MV_UJB.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==6')" >> ./results/flops_MM_UJB.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==1')" >> ./results/flops_avx_MV.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==2')" >> ./results/flops_avx_MM.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==5')" >> ./results/flops_avx_MV_UJB.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==6')" >> ./results/flops_avx_MM_UJB.dat
done

rm output.txt

echo "Generating plots"
gnuplot -c plot.gp "Time" "results/time_MV.png" "results/time_MV.dat" "MatVet" > /dev/null 2>&1
gnuplot -c plot.gp "Time" "results/time_MM.png" "results/time_MM.dat" "MatMat" > /dev/null 2>&1
gnuplot -c plot.gp "Time" "results/time_MV_UJB.png" "results/time_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot.gp "Time" "results/time_MM_UJB.png" "results/time_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot.gp "Bandwidth" "results/bandwidth_MV.png" "results/bandwidth_MV.dat" "MatVet" > /dev/null 2>&1
gnuplot -c plot.gp "Bandwidth" "results/bandwidth_MM.png" "results/bandwidth_MM.dat" "MatMat" > /dev/null 2>&1
gnuplot -c plot.gp "Bandwidth" "results/bandwidth_MV_UJB.png" "results/bandwidth_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot.gp "Bandwidth" "results/bandwidth_MM_UJB.png" "results/bandwidth_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_MV.png" "results/missl2_MV.dat" "MatVet" > /dev/null 2>&1
gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_MM.png" "results/missl2_MM.dat" "MatMat" > /dev/null 2>&1
gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_MV_UJB.png" "results/missl2_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_MM_UJB.png" "results/missl2_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot.gp "Energy" "results/energy_MV.png" "results/energy_MV.dat" "MatVet" > /dev/null 2>&1
gnuplot -c plot.gp "Energy" "results/energy_MM.png" "results/energy_MM.dat" "MatMat" > /dev/null 2>&1
gnuplot -c plot.gp "Energy" "results/energy_MV_UJB.png" "results/energy_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot.gp "Energy" "results/energy_MM_UJB.png" "results/energy_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS" "results/flops_MV.png" "results/flops_MV.dat" "MatVet" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS" "results/flops_MM.png" "results/flops_MM.dat" "MatMat" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS" "results/flops_MV_UJB.png" "results/flops_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS" "results/flops_MM_UJB.png" "results/flops_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS AVX" "results/flops_avx_MV.png" "results/flops_avx_MV.dat" "MatVet" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS AVX" "results/flops_avx_MM.png" "results/flops_avx_MM.dat" "MatMat" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS AVX" "results/flops_avx_MV_UJB.png" "results/flops_avx_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot.gp "FLOPS AVX" "results/flops_avx_MM_UJB.png" "results/flops_avx_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1

gnuplot -c plot_compared.gp "Time" "results/compared_time_MV.png" "results/time_MV.dat" "MatVet" "results/time_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Time" "results/compared_time_MM.png" "results/time_MM.dat" "MatMat" "results/time_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Bandwidth" "results/compared_bandwidth_MV.png" "results/bandwidth_MV.dat" "MatVet" "results/bandwidth_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Bandwidth" "results/compared_bandwidth_MM.png" "results/bandwidth_MM.dat" "MatMat" "results/bandwidth_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "L2 Miss Ratio" "results/compared_missl2_MV.png" "results/missl2_MV.dat" "MatVet" "results/missl2_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "L2 Miss Ratio" "results/compared_missl2_MM.png" "results/missl2_MM.dat" "MatMat" "results/missl2_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Energy" "results/compared_energy_MV.png" "results/energy_MV.dat" "MatVet" "results/energy_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Energy" "results/compared_energy_MM.png" "results/energy_MM.dat" "MatMat" "results/energy_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "FLOPS" "results/compared_flops_MV.png" "results/flops_MV.dat" "MatVet" "results/flops_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "FLOPS" "results/compared_flops_MM.png" "results/flops_MM.dat" "MatMat" "results/flops_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "FLOPS AVX" "results/compared_flops_avx_MV.png" "results/flops_avx_MV.dat" "MatVet" "results/flops_avx_MV_UJB.dat" "MatVetUJB" > /dev/null 2>&1
gnuplot -c plot_compared.gp "FLOPS AVX" "results/compared_flops_avx_MM.png" "results/flops_avx_MM.dat" "MatMat" "results/flops_avx_MM_UJB.dat" "MatMatUJB" > /dev/null 2>&1

echo "Removing temporary files"
rm results/*.dat

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor