#!/bin/bash

SRCS_PHILO=tache1.1/src/main.c
SRCS_PRODCONS=tache1.2/src/main.c
SRCS_WRITEREAD=tache1.3/src/main.c



CFLAGS="-Wall -Wextra -Werror"

OUTPUT_PHILO="perf_finale_1/perf_philo.csv"
OUTPUT_PRODCONS="perf_finale_1/perf_prodcons.csv"
OUTPUT_WRITEREAD="perf_finale_1/perf_writeread.csv"

echo "performance tourne.."

gcc $CFLAGS $SRCS_PHILO -o philo
#gcc $CFLAGS $SRCS_PRODCONS -o prodcons
#gcc $CFLAGS $SRCS_WRITEREAD -o writeread



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
        TIME_PHILO=$( { /usr/bin/time -f "%e" ./philo $NTHREADS; } 2>&1 > /dev/null )
        TIME_PRODCONS=$( { /usr/bin/time -f "%e" ./prodcons $HALF_NT $HALF_NT; } 2>&1 > /dev/null )
        TIME_WRITEREAD=$( { /usr/bin/time -f "%e" ./writeread $HALF_NT $HALF_NT; } 2>&1 > /dev/null )
        
        echo -n ",$TIME_PHILO" >> $OUTPUT_PHILO
        echo -n ",$TIME_PRODCONS" >> $OUTPUT_PRODCONS
        echo -n ",$TIME_WRITEREAD" >> $OUTPUT_WRITEREAD
    done
    echo "" >> $OUTPUT_PHILO
    echo "" >> $OUTPUT_PRODCONS
    echo "" >> $OUTPUT_WRITEREAD

    
done

rm ./philo
rm ./prodcons
rm ./writeread

echo "performance est fini !"

