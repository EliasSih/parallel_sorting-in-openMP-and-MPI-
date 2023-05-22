#!/bin/sh
program_name="./hoare"
output_file="outputHr.csv"
array_file="arraysize.txt"

# Check if the array size file exists
if [ ! -f "$array_file" ]; then
    echo "Array size file '$array_file' not found."
    exit 1
fi

#Create the output file and write the header
echo "Input,Run,Output" > $output_file

#Read the array sizes from the file into an array
readarray -t array_sizes < "$array_file"

#Define an array of inputs
#inputs=(10 100 1000 10000 100000 1000000)

#Iterate over each array size
for array_size in "${array_sizes[@]}"
do
    array_size="${array_size[$index]}"

    # Check if the array size is a valid number
    #if ! [[ $array_size =~ ^[0-9]+$ ]]; then
    #    echo "Invalid array size: $array_size"
    #    continue
    #fi

    #Run the program 10 times and redirect output
    for ((i=1; i<=10; i++))
    do
        echo "Running program for array size $array_size - Run $i"
        output=$($program_name $array_size)
        echo "$array_size,$i,\"$output\"" >> $output_file
        echo "Program completed for array size $array_size - Run $i"
    done
done