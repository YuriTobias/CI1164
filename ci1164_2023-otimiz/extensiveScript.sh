#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

xe="$1"
input="$2"

# Results of MatVet without optimization
time_array_MV=()
bandwidth_array_MV=()
missl2_array_MV=()
energy_array_MV=()
flops_array_MV=()
flops_array_avx_MV=()

# Results of MatVet with loop unrolling
time_array_MV_UJ=()
bandwidth_array_MV_UJ=()
missl2_array_MV_UJ=()
energy_array_MV_UJ=()
flops_array_MV_UJ=()
flops_array_avx_MV_UJ=()

# Results of MatVet with loop unrolling and blocking
time_array_MV_UJB=()
bandwidth_array_MV_UJB=()
missl2_array_MV_UJB=()
energy_array_MV_UJB=()
flops_array_MV_UJB=()
flops_array_avx_MV_UJB=()

# Results of MatMat without optimization
time_array_MM=()
bandwidth_array_MM=()
missl2_array_MM=()
energy_array_MM=()
flops_array_MM=()
flops_array_avx_MM=()

# Results of MatMat with loop unrolling
time_array_MM_UJ=()
bandwidth_array_MM_UJ=()
missl2_array_MM_UJ=()
energy_array_MM_UJ=()
flops_array_MM_UJ=()
flops_array_avx_MM_UJ=()

# Results of MatMat with loop unrolling and blocking
time_array_MM_UJB=()
bandwidth_array_MM_UJB=()
missl2_array_MM_UJB=()
energy_array_MM_UJB=()
flops_array_MM_UJB=()
flops_array_avx_MM_UJB=()

sizes=(64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000)
arraySize=${#sizes[@]}

for size in "${sizes[@]}"; do
    echo "Testing size $size"
    likwid-perfctr -C ${CPU} -g L3 -m ./matmult $size > output.txt
    time_array_MV+=($(grep "multMatVetTime:" output.txt | awk '{print $2}'))
    time_array_MM+=($(grep "multMatMatTime:" output.txt | awk '{print $2}'))
    time_array_MV_UJ+=($(grep "multMatVetTimeUJ:" output.txt | awk '{print $2}'))
    time_array_MM_UJ+=($(grep "multMatMatTimeUJ:" output.txt | awk '{print $2}'))
    time_array_MV_UJB+=($(grep "multMatVetTimeUJB:" output.txt | awk '{print $2}'))
    time_array_MM_UJB+=($(grep "multMatMatTimeUJB:" output.txt | awk '{print $2}'))
    bandwidth_array_MV+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==1'))
    bandwidth_array_MM+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==2'))
    bandwidth_array_MV_UJ+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==3'))
    bandwidth_array_MM_UJ+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==4'))
    bandwidth_array_MV_UJB+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==5'))
    bandwidth_array_MM_UJB+=($(grep "L3 bandwidth \[MBytes/s\]" output.txt | awk '{print $6}' | awk 'NR==6'))
    likwid-perfctr -C ${CPU} -g L2CACHE -m ./matmult $size > output.txt
    missl2_array_MV+=($(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==1'))
    missl2_array_MM+=($(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==2'))
    missl2_array_MV_UJ+=($(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==3'))
    missl2_array_MM_UJ+=($(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==4'))
    missl2_array_MV_UJB+=($(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==5'))
    missl2_array_MM_UJB+=($(grep "L2 miss ratio" output.txt | awk '{print $6}' | awk 'NR==6'))
    likwid-perfctr -C ${CPU} -g ENERGY -m ./matmult $size > output.txt
    energy_array_MV+=($(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==1'))
    energy_array_MM+=($(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==2'))
    energy_array_MV_UJ+=($(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==3'))
    energy_array_MM_UJ+=($(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==4'))
    energy_array_MV_UJB+=($(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==5'))
    energy_array_MM_UJB+=($(grep "Energy \[J\]" output.txt | awk '{print $5}' | awk 'NR==6'))
    likwid-perfctr -C ${CPU} -g FLOPS_DP -m ./matmult $size > output.txt
    flops_array_MV+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==1'))
    flops_array_MM+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==2'))
    flops_array_MV_UJ+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==3'))
    flops_array_MM_UJ+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==4'))
    flops_array_MV_UJB+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==5'))
    flops_array_MM_UJB+=($(grep "DP \[MFLOP/s\]" output.txt | grep -v "AVX"| awk '{print $5}' | awk 'NR==6'))
    flops_array_avx_MV+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==1'))
    flops_array_avx_MM+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==2'))
    flops_array_avx_MV_UJ+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==3'))
    flops_array_avx_MM_UJ+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==4'))
    flops_array_avx_MV_UJB+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==5'))
    flops_array_avx_MM_UJB+=($(grep "AVX DP \[MFLOP/s\]" output.txt| awk '{print $6}' | awk 'NR==6'))
done

echo "multMatVet time"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${time_array_MV[$i]}
done

echo "multMatVet bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${bandwidth_array_MV[$i]}
done

echo "multMatVet L2 miss ratio"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${missl2_array_MV[$i]}
done

echo "multMatVet energy"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${energy_array_MV[$i]}
done

echo "multMatVet flops"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_MV[$i]}
done

echo "multMatVet flops avx"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_avx_MV[$i]}
done

echo "multMatVetUJ time"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${time_array_MV_UJ[$i]}
done

echo "multMatVetUJ bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${bandwidth_array_MV_UJ[$i]}
done

echo "multMatVetUJ L2 miss ratio"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${missl2_array_MV_UJ[$i]}
done

echo "multMatVetUJ energy"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${energy_array_MV_UJ[$i]}
done

echo "multMatVetUJ flops"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_MV_UJ[$i]}
done

echo "multMatVetUJ flops avx"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_avx_MV_UJ[$i]}
done

echo "multMatVetUJB time"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${time_array_MV_UJB[$i]}
done

echo "multMatVetUJB bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${bandwidth_array_MV_UJB[$i]}
done

echo "multMatVetUJB L2 miss ratio"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${missl2_array_MV_UJB[$i]}
done

echo "multMatVetUJB energy"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${energy_array_MV_UJB[$i]}
done

echo "multMatVetUJB flops"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_MV_UJB[$i]}
done

echo "multMatVetUJB flops avx"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_avx_MV_UJB[$i]}
done

echo "multMatMat time"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${time_array_MM[$i]}
done

echo "multMatMat bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${bandwidth_array_MM[$i]}
done

echo "multMatMat L2 miss ratio"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${missl2_array_MM[$i]}
done

echo "multMatMat energy"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${energy_array_MM[$i]}
done

echo "multMatMat flops"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_MM[$i]}
done

echo "multMatMat flops avx"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_avx_MM[$i]}
done

echo "multMatMatUJ time"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${time_array_MM_UJ[$i]}
done

echo "multMatMatUJ bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${bandwidth_array_MM_UJ[$i]}
done

echo "multMatMatUJ L2 miss ratio"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${missl2_array_MM_UJ[$i]}
done

echo "multMatMatUJ energy"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${energy_array_MM_UJ[$i]}
done

echo "multMatMatUJ flops"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_MM_UJ[$i]}
done

echo "multMatMatUJ flops avx"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_avx_MM_UJ[$i]}
done

echo "multMatMatUJB time"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${time_array_MM_UJB[$i]}
done

echo "multMatMatUJB bandwidth"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${bandwidth_array_MM_UJB[$i]}
done

echo "multMatMatUJB L2 miss ratio"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${missl2_array_MM_UJB[$i]}
done

echo "multMatMatUJB energy"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${energy_array_MM_UJB[$i]}
done

echo "multMatMatUJB flops"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_MM_UJB[$i]}
done

echo "multMatMatUJB flops avx"
for (( i=0; i<${arraySize}; i++ )); do
    echo ${sizes[$i]} ${flops_array_avx_MM_UJB[$i]}
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor