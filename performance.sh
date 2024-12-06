#!/bin/bash

SRCS_PHILO=philosopher/src/philosopher.c
SRCS_PRODCONS=prodcons/src/prodcons.c
SRCS_WRITEREAD=writeread/src/writeread.c



CFLAGS="-Wall -Wextra -Werror -pthread"

OUTPUT_PHILO="perf_finale_1/perf_philo.csv"
OUTPUT_PRODCONS="perf_finale_1/perf_prodcons.csv"
OUTPUT_WRITEREAD="perf_finale_1/perf_writeread.csv"

echo "performance tourne.."

gcc $CFLAGS $SRCS_PHILO -o philo_perf
gcc $CFLAGS $SRCS_PRODCONS -o prodcons_perf
gcc $CFLAGS $SRCS_WRITEREAD -o writeread_perf


echo "Threads,Run1,Run2,Run3,Run4,Run5" > $OUTPUT_PHILO
echo "Threads,Run1,Run2,Run3,Run4,Run5" > $OUTPUT_PRODCONS
echo "Threads,Run1,Run2,Run3,Run4,Run5" > $OUTPUT_WRITEREAD


for NTHREADS in 2 4 8 16 32
do  
    HALF_NT=$(( NTHREADS / 2))
    echo -n "$NTHREADS" >> $OUTPUT_PHILO
    echo -n "$NTHREADS" >> $OUTPUT_PRODCONS
    echo -n "$NTHREADS" >> $OUTPUT_WRITEREAD


    for i in $(seq 1 5)
    do
        TIME_PHILO=$( { /usr/bin/time -f "%e" ./philo_perf $NTHREADS; } 2>&1 > /dev/null )
        TIME_PRODCONS=$( { /usr/bin/time -f "%e" ./prodcons_perf $HALF_NT $HALF_NT; } 2>&1 > /dev/null )
        TIME_WRITEREAD=$( { /usr/bin/time -f "%e" ./writeread_perf $HALF_NT $HALF_NT; } 2>&1 > /dev/null )
        
        echo -n ",$TIME_PHILO" >> $OUTPUT_PHILO
        echo -n ",$TIME_PRODCONS" >> $OUTPUT_PRODCONS
        echo -n ",$TIME_WRITEREAD" >> $OUTPUT_WRITEREAD
    done
    echo "" >> $OUTPUT_PHILO
    echo "" >> $OUTPUT_PRODCONS
    echo "" >> $OUTPUT_WRITEREAD

    
done

rm ./philo_perf
rm ./prodcons_perf
rm ./writeread_perf

echo "performance est fini !"

