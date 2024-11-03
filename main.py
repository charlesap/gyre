#!/usr/bin/env python

import argparse

from operations import toast

parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbose", help="increase verbosity",
                    action="store_true")
parser.add_argument('ids', nargs = '*', help = 'some ids')

args = parser.parse_args()



if args.verbose:
    print("verbose output")

if args.ids == []:
    print("no input files")
else:
    for element in args.ids:
        print(element)
        with open(element, 'r', encoding='utf-8') as f:
            text = f.read()
            print(toast(text))

#print( toast("l | r in h mirrored."))

