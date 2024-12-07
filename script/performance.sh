#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <binary> [output_prefix]"
    echo "Example: $0 ./myprogram perf_myprogram"
    exit 1
fi

BINARY="$1"

OUTPUT_PREFIX="${2:-perf}"

OUTPUT_DIR="csv"
mkdir -p "$OUTPUT_DIR"

OUTPUT_FILE="$OUTPUT_DIR/${OUTPUT_PREFIX}.csv"

if [ ! -x "$BINARY" ]; then
    echo "Error: Binary $BINARY is not executable"
    exit 1
fi

configure_binary() {
    local binary="$1"
    local threads="$2"
    
    ARGS=("$threads")
    
    if [[ "$binary" == *"prodcons"* ]] || [[ "$binary" == *"writeread"* ]]; then
        HALF_THREADS=$((threads / 2))
        ARGS=("$HALF_THREADS" "$HALF_THREADS")
    fi
    
    echo "${ARGS[@]}"
}

run_performance_test() {
    local binary="$1"
    local output_file="$2"
    local thread_counts=(2 4 8 16 32)
    
    echo "Threads,Run1,Run2,Run3,Run4,Run5" > "$output_file"

    for NTHREADS in "${thread_counts[@]}"
    do  
        BINARY_ARGS=$(configure_binary "$binary" "$NTHREADS")
        
        echo -n "$NTHREADS" >> "$output_file"

        for i in $(seq 1 5)
        do
            TIME=$( { /usr/bin/time -f "%e" "$binary" $BINARY_ARGS; } 2>&1 > /dev/null )
            echo -n ",$TIME" >> "$output_file"
        done
        
        echo "" >> "$output_file"
    done
}

echo "Performance testing started for $BINARY..."

run_performance_test "$BINARY" "$OUTPUT_FILE"

echo "✅ Performance testing completed for $BINARY"