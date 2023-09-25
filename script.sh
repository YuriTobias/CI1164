#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

xe="$1"
input="$2"
resultado="output.txt"

#filtra a saída para exibir somente os valores esperados
likwid-perfctr -C ${CPU} -g $METRICA -m ./interpola $xe > output.txt
sed -n '6,9p;/DP \[MFLOP\/s\] /p' output.txt | grep -v "AVX DP \[MFLOP/s\]"

rm -f output.txt

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor