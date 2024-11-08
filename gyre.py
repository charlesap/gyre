#!/usr/bin/env python

import argparse

from parsing import toast, blast, collate
from production import produce


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
    ok = True
    thecst = {}
    scopes = {}
    for element in args.ids:
        if ok:
            print("reading ",element)
            with open(element, 'r', encoding='utf-8') as f:
                text = f.read()
                theast = toast(text)
                print(theast.pretty())
                ok = blast(theast,thecst,scopes)
    if ok:
        ok,current = collate(thecst)

    if ok:
        ok = produce(current,thecst)

    if ok:
        print("generation complete.")
    else:
        print("generation failed.")
