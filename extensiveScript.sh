#!/bin/bash

CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

sizes=(64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 1000000 10000000 100000000)
arraySize=${#sizes[@]}

mkdir results > /dev/null 2>&1
echo "#" > ./results/time_O_least_squares.dat
echo "#" > ./results/flops_O_least_squares.dat
echo "#" > ./results/flops_avx_O_least_squares.dat
echo "#" > ./results/bandwidth_O_least_squares.dat
echo "#" > ./results/missl2_O_least_squares.dat
echo "#" > ./results/time_N_least_squares.dat
echo "#" > ./results/flops_N_least_squares.dat
echo "#" > ./results/flops_avx_N_least_squares.dat
echo "#" > ./results/bandwidth_N_least_squares.dat
echo "#" > ./results/missl2_N_least_squares.dat
echo "#" > ./results/time_O_system_sol.dat
echo "#" > ./results/flops_O_system_sol.dat
echo "#" > ./results/flops_avx_O_system_sol.dat
echo "#" > ./results/bandwidth_O_system_sol.dat
echo "#" > ./results/missl2_O_system_sol.dat
echo "#" > ./results/time_N_system_sol.dat
echo "#" > ./results/flops_N_system_sol.dat
echo "#" > ./results/flops_avx_N_system_sol.dat
echo "#" > ./results/bandwidth_N_system_sol.dat
echo "#" > ./results/missl2_N_system_sol.dat
echo "#" > ./results/time_O_residue_sol.dat
echo "#" > ./results/flops_O_residue_sol.dat
echo "#" > ./results/flops_avx_O_residue_sol.dat
echo "#" > ./results/bandwidth_O_residue_sol.dat
echo "#" > ./results/missl2_O_residue_sol.dat
echo "#" > ./results/time_N_residue_sol.dat
echo "#" > ./results/flops_N_residue_sol.dat
echo "#" > ./results/flops_avx_N_residue_sol.dat
echo "#" > ./results/bandwidth_N_residue_sol.dat
echo "#" > ./results/missl2_N_residue_sol.dat

for size in "${sizes[@]}"; do
    ./geraEntrada $size > entry.dat
    echo "Testing size $size"
    cat entry.dat | likwid-perfctr -C ${CPU} -g L3 -m ./ajustePolOpt > output.txt
    echo "$size $(grep "LeastSquares: " output.txt | awk '{print $2}')" >> ./results/time_O_least_squares.dat
    echo "$size $(grep "SystemSol: " output.txt | awk '{print $2}')" >> ./results/time_O_system_sol.dat
    echo "$size $(grep "ResidualSol: " output.txt | awk '{print $2}')" >> ./results/time_O_residue_sol.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/bandwidth_O_least_squares.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/bandwidth_O_system_sol.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==3')" >> ./results/bandwidth_O_residue_sol.dat

    cat entry.dat | likwid-perfctr -C ${CPU} -g L3 -m ./ajustePol > output.txt
    echo "$size $(grep "LeastSquares: " output.txt | awk '{print $2}')" >> ./results/time_N_least_squares.dat
    echo "$size $(grep "SystemSol: " output.txt | awk '{print $2}')" >> ./results/time_N_system_sol.dat
    echo "$size $(grep "ResidualSol: " output.txt | awk '{print $2}')" >> ./results/time_N_residue_sol.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/bandwidth_N_least_squares.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/bandwidth_N_system_sol.dat
    echo "$size $(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==3')" >> ./results/bandwidth_N_residue_sol.dat

    cat entry.dat | likwid-perfctr -C ${CPU} -g L2CACHE -m ./ajustePolOpt > output.txt
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/missl2_O_least_squares.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/missl2_O_system_sol.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==3')" >> ./results/missl2_O_residue_sol.dat

    cat entry.dat | likwid-perfctr -C ${CPU} -g L2CACHE -m ./ajustePol > output.txt
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==1')" >> ./results/missl2_N_least_squares.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==2')" >> ./results/missl2_N_system_sol.dat
    echo "$size $(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==3')" >> ./results/missl2_N_residue_sol.dat

    cat entry.dat | likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./ajustePolOpt > output.txt
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==1')" >> ./results/flops_O_least_squares.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==2')" >> ./results/flops_O_system_sol.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==3')" >> ./results/flops_O_residue_sol.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==1')" >> ./results/flops_avx_O_least_squares.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==2')" >> ./results/flops_avx_O_system_sol.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==3')" >> ./results/flops_avx_O_residue_sol.dat

    cat entry.dat | likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./ajustePol > output.txt
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==1')" >> ./results/flops_N_least_squares.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==2')" >> ./results/flops_N_system_sol.dat
    echo "$size $(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==3')" >> ./results/flops_N_residue_sol.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==1')" >> ./results/flops_avx_N_least_squares.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==2')" >> ./results/flops_avx_N_system_sol.dat
    echo "$size $(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==3')" >> ./results/flops_avx_N_residue_sol.dat
done

rm output.txt
rm entry.dat

echo "Generating plots"
# gnuplot -c plot.gp "Time" "results/time_O_least_squares.png" "results/time_O_least_squares.dat" "Optimized" > /dev/null 2>&1
# gnuplot -c plot.gp "Time" "results/time_N_least_squares.png" "results/time_N_least_squares.dat" "Naive" > /dev/null 2>&1
# gnuplot -c plot.gp "Flops" "results/flops_O_least_squares.png" "results/flops_O_least_squares.dat" "Optimized" > /dev/null 2>&1
# gnuplot -c plot.gp "Flops" "results/flops_N_least_squares.png" "results/flops_N_least_squares.dat" "Naive" > /dev/null 2>&1
# gnuplot -c plot.gp "Flops Avx" "results/flops_avx_O_least_squares.png" "results/flops_avx_O_least_squares.dat" "Optimized" > /dev/null 2>&1
# gnuplot -c plot.gp "Flops Avx" "results/flops_avx_N_least_squares.png" "results/flops_avx_N_least_squares.dat" "Naive" > /dev/null 2>&1
# gnuplot -c plot.gp "Bandwidth" "results/bandwidth_O_least_squares.png" "results/bandwidth_O_least_squares.dat" "Optimized" > /dev/null 2>&1
# gnuplot -c plot.gp "Bandwidth" "results/bandwidth_N_least_squares.png" "results/bandwidth_N_least_squares.dat" "Naive" > /dev/null 2>&1
# gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_O_least_squares.png" "results/missl2_O_least_squares.dat" "Optimized" > /dev/null 2>&1
# gnuplot -c plot.gp "L2 Miss Ratio" "results/missl2_N_least_squares.png" "results/missl2_N_least_squares.dat" "Naive" > /dev/null 2>&1

gnuplot -c plot_compared.gp "Time" "results/compared_time_least_squares.png" "results/time_O_least_squares.dat" "Optimized" "results/time_N_least_squares.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Flops" "results/compared_flops_least_squares.png" "results/flops_O_least_squares.dat" "Optimized" "results/flops_N_least_squares.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Flops Avx" "results/compared_flops_avx_least_squares.png" "results/flops_avx_O_least_squares.dat" "Optimized" "results/flops_avx_N_least_squares.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Bandwidth" "results/compared_bandwidth_least_squares.png" "results/bandwidth_O_least_squares.dat" "Optimized" "results/bandwidth_N_least_squares.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "L2 Miss Ratio" "results/compared_missl2_least_squares.png" "results/missl2_O_least_squares.dat" "Optimized" "results/missl2_N_least_squares.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Time" "results/compared_time_system_sol.png" "results/time_O_system_sol.dat" "Optimized" "results/time_N_system_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Flops" "results/compared_flops_system_sol.png" "results/flops_O_system_sol.dat" "Optimized" "results/flops_N_system_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Flops Avx" "results/compared_flops_avx_system_sol.png" "results/flops_avx_O_system_sol.dat" "Optimized" "results/flops_avx_N_system_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Bandwidth" "results/compared_bandwidth_system_sol.png" "results/bandwidth_O_system_sol.dat" "Optimized" "results/bandwidth_N_system_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "L2 Miss Ratio" "results/compared_missl2_system_sol.png" "results/missl2_O_system_sol.dat" "Optimized" "results/missl2_N_system_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Time" "results/compared_time_residue_sol.png" "results/time_O_residue_sol.dat" "Optimized" "results/time_N_residue_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Flops" "results/compared_flops_residue_sol.png" "results/flops_O_residue_sol.dat" "Optimized" "results/flops_N_residue_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Flops Avx" "results/compared_flops_avx_residue_sol.png" "results/flops_avx_O_residue_sol.dat" "Optimized" "results/flops_avx_N_residue_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "Bandwidth" "results/compared_bandwidth_residue_sol.png" "results/bandwidth_O_residue_sol.dat" "Optimized" "results/bandwidth_N_residue_sol.dat" "Naive" > /dev/null 2>&1
gnuplot -c plot_compared.gp "L2 Miss Ratio" "results/compared_missl2_residue_sol.png" "results/missl2_O_residue_sol.dat" "Optimized" "results/missl2_N_residue_sol.dat" "Naive" > /dev/null 2>&1

echo "Removing temporary files"
rm results/*.dat

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor