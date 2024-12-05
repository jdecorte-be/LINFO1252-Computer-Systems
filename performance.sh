#!/bin/bash

SRCS_PHILO=tache1.1/src/main.c
SRCS_PRODCONS=tache1.2/src/main.c
SRCS_WRITEREAD=tache1.3/src/main.c



CFLAGS="-Wall -Wextra -Werror"

OUTPUT_PHILO="perf_finale_1/perf_philo.csv"
OUTPUT_PRODCONS="perf_finale_1/perf_prodcons.csv"
OUTPUT_WRITEREAD="perf_finale_1/perf_writeread.csv"



gcc $CFLAGS $SRCS_PHILO -o philo
gcc $CFLAGS $SRCS_PRODCONS -o prodcons
gcc $CFLAGS $SRCS_WRITEREAD -o writeread



echo "Threads,Sample,Time" > $OUTPUT_PHILO
echo "Threads,Sample,Time" > $OUTPUT_PRODCONS
echo "Threads,Sample,Time" > $OUTPUT_WRITEREAD


for NTHREADS in 2 4 8 16 32
do  
    HALF_NT=$(( NTHREADS / 2))


    for i in $(seq 1 5)
    do
        TIME_PHILO=$( { /usr/bin/time -f "%e" ./philo $NTHREADS; } 2>&1 > /dev/null )
        TIME_PRODCONS=$( { /usr/bin/time -f "%e" ./prodcons $HALF_NT $HALF_NT; } 2>&1 > /dev/null )
        TIME_WRITEREAD=$( { /usr/bin/time -f "%e" ./writeread $HALF_NT $HALF_NT; } 2>&1 > /dev/null )
        
        echo "$NTHREADS,$i,$TIME_PHILO" >> $OUTPUT_PHILO
        echo "$NTHREADS,$i,$TIME_PRODCONS" >> $OUTPUT_PRODCONS
        echo "$NTHREADS,$i,$TIME_WRITEREAD" >> $OUTPUT_WRITEREAD
    done
    
done

rm ./philo
rm ./prodcons
rm ./writeread

