# main 
import argparse

from operations import something, somethingelse, toast

parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbose", help="increase verbosity",
                    action="store_true")

args = parser.parse_args()



if args.verbose:
    print("verbose output")

print( toast("l | r in h mirrored."))
print( something(1))
print( somethingelse(1))

