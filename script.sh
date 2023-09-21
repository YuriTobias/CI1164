#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

LIKWID_HOME=/usr/local

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./interpola
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor