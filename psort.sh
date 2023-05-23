#!/bin/bash

# define the input sizes
input_sizes=(10 100 1000 10000 100000 1000000)

# define the number of processes
num_processes=4

# programs to run
programs=(mpiVersion openmpVersion hybridVT sortingHoare sortingLomuto)

for program in "${programs[@]}"
do
  # create a new csv file and write the headers
  echo "size,iteration,result" > "${program}_results.csv"
  
  for size in "${input_sizes[@]}"
  do
    for iteration in {1..10}
    do
      # check if the program is mpiVersion or hybridVT
      if [ "$program" == "mpiVersion" ] || [ "$program" == "hybridVT" ]
      then
        # run the mpi program and get the result
        result=$(mpirun -np "$num_processes" "./$program" "$size")
      elif [ "$program" == "openmpVersion" ]
      then
        # run the openmp program and get the result
        result=$("./$program" "$size" -fopenmp)
      else
        # run the sorting program and get the result
        result=$("./$program" "$size")
      fi

      # append the result to the csv file
      echo "$size,$iteration,$result" >> "${program}_results.csv"
    done
  done
done
