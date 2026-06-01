#!/bin/bash

BASE_MACRO="vrml.mac"
N=10           # number of runs (number of macros to generate and execute)
NEVENTS=10000  # number of events
ONOFF="OFF"    # ON/OFF parameter
NTHREADS=""     # number of threads

# Function: generate a random floating-point number within a range [min, max]
rand_in_range() {
    local min=$1
    local max=$2
    # awk with srand seeded by system time + process ID, to avoid repetition across runs
    awk -v min="$min" -v max="$max" 'BEGIN {srand(systime() + PROCINFO["pid"]); print min + rand() * (max - min)}'
}

# Parameter ranges
FOCUS_MIN=-399.824698
FOCUS_MAX=1798.325132

NORM_MIN=1.100516
NORM_MAX=1.849792

FRAC_MIN=0.002064
FRAC_MAX=0.119983

PRESS_MIN=10.094508
PRESS_MAX=99.957409

# CSV file to keep track of the generated parameters
CSV_FILE="run_parameters.csv"
echo "Run,Focus,NormVec,FracDop,Pressure" > "$CSV_FILE"

# Loop over the number of runs
for i in $(seq 1 $N); do
    # Generate random values for each parameter
    FOCUS=$(rand_in_range $FOCUS_MIN $FOCUS_MAX)
    NORM=$(rand_in_range $NORM_MIN $NORM_MAX)
    FRAC=$(rand_in_range $FRAC_MIN $FRAC_MAX)
    PRESS=$(rand_in_range $PRESS_MIN $PRESS_MAX)

    # Create a new macro file based on the base one
    OUT_MACRO="macro_${i}.mac"
    cp "$BASE_MACRO" "$OUT_MACRO"

    # Replace parameters inside the macro file
    sed -i "s|^/PlasmaMLPALLAS/laser/setOffsetLaserFocus .*|/PlasmaMLPALLAS/laser/setOffsetLaserFocus $FOCUS|" "$OUT_MACRO"
    sed -i "s|^/PlasmaMLPALLAS/laser/setNormVecPotential .*|/PlasmaMLPALLAS/laser/setNormVecPotential $NORM|" "$OUT_MACRO"
    sed -i "s|^/PlasmaMLPALLAS/laser/setFracDopTargetChamber .*|/PlasmaMLPALLAS/laser/setFracDopTargetChamber $FRAC|" "$OUT_MACRO"
    sed -i "s|^/PlasmaMLPALLAS/laser/setPressure .*|/PlasmaMLPALLAS/laser/setPressure $PRESS|" "$OUT_MACRO"

    # Define the output file name for this run
    OUTPUT="output_${i}"

    echo "[$i] Running simulation with:"
    echo "   Focus = $FOCUS"
    echo "   NormVec = $NORM"
    echo "   FracDop = $FRAC"
    echo "   Pressure = $PRESS"
    echo "   --> Results in $OUTPUT"

    # Run the simulation with the generated macro and parameters
    ./PlasmaMLPALLAS "$OUTPUT" "$NEVENTS" "$OUT_MACRO" "$ONOFF" "$NTHREADS"

    # Save the parameters used into the CSV file
    echo "$i,$FOCUS,$NORM,$FRAC,$PRESS" >> "$CSV_FILE"
done
