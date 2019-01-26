#!/usr/bin/env python


import os
import struct
import sys

MAGIC = 'MiSTer\0\0\0\0\0\0'


def print_usage():
    print("Utility to inject ROM into RBF file for MiSTer.")
    print("Usage: %s <core.rbf> <boot.rom> <my_new_core.rbf>" % sys.argv[0])


if len(sys.argv) < 3:
    print_usage()
    exit(1)


with open(sys.argv[3], 'wb') as rbf_out:
    with open(sys.argv[1], 'rb') as rbf:
        rbf_data = rbf.read()
        if (rbf_data.startswith(MAGIC)):
            print("Source core already injected with ROM.")
            exit(2)
        # header is 16 bytes
        # 6 bytes magic
        rbf_out.write(MAGIC)
        # 4 bytes size of core, after which the rom follows
        rbf_size = len(rbf_data)
        rbf_out.write(struct.pack("i", rbf_size))
        rbf_out.write(rbf_data)
        with open(sys.argv[2], 'rb') as rom:
            rbf_out.write(rom.read())
