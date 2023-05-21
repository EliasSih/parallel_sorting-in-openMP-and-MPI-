import argparse
import random

# Set up command-line argument parsing
parser = argparse.ArgumentParser(description='Populate a text file with random numbers.')
parser.add_argument('--num', type=int, help='The number of random numbers to generate')
parser.add_argument('--max', type=int, help='The maximum value for the random numbers')
parser.add_argument('--filename', type=str, default='input.txt', help='The name of the output file')

args = parser.parse_args()

# Generate random numbers
random_numbers = [random.randint(0, args.max) for _ in range(args.num)]

# Write the random numbers to the specified file, 30 per line
with open(args.filename, 'w') as f:
    for i in range(0, len(random_numbers), 10):
        f.write(' '.join(map(str, random_numbers[i:i+10])) + '\n')

