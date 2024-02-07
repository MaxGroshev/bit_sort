import subprocess
import argparse
import random
import numpy as np
import os
import sys

# -----------------------------------------------------------------------------------------

def print_test_data(args, test_data):
    dat_file = open(args.file, "w")

    dat_file.write("%d "%  args.count)
    dat_file.write("\n")

    for i in range (args.count):
        dat_file.write("%d "% test_data[i])
        if ((i % 10) == 0):
            dat_file.write("\n")

    test_data.sort()
    dat_file.write("(")
    for i in range (args.count):
        dat_file.write("%d "% test_data[i])
    dat_file.write(")")
    dat_file.close()


def generate_test_data(args):
    test_data = []

    for i in range(args.count):
        test_data.append(random.randint(args.diaposon[0], args.diaposon[1]))

    return test_data

# -----------------------------------------------------------------------------------------

def add_parse_arguments(parser):
    parser.add_argument("-c",  "--count",         type = int, default = 2048)
    parser.add_argument('-d',  "--diaposon",      nargs = 2, type = int, default = [-10000, 10000])
    parser.add_argument('-f',  '--file',          type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_test_dat/gen_test.dat')

# -----------------------------------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    add_parse_arguments(parser)
    args = parser.parse_args()
    # check_args(args)

    test_data = generate_test_data(args)
    print_test_data(args, test_data)
