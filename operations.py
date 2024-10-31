# operations 
from lark import Lark


def something(param):
    """Does something."""
    return(param+1)

def somethingelse(param):
    """Does something."""
    return(param-1)

def toast(theinput):
    gyre_parser = Lark(r"""
        value: (assertion)*

        assertion: for
                 | identity
                 | composition
                 | qualification
                 | "IS" | "IN" | "AS" | "FLIPS"

        for : "for" CNAME "."
        identity : CNAME "is" namelist "."
        composition : CNAME "has" namelist "."
        qualification : orlist "in" CNAME [mirrored] "."

        mirrored : "mirrored"
        orlist : CNAME "|" CNAME ("|" CNAME)*
        namelist : [CNAME ("," CNAME)*]

        %import common.ESCAPED_STRING
        %import common.SIGNED_NUMBER
        %import common.CNAME
        %import common.WS
        %ignore WS

        """, start='value')

    return( gyre_parser.parse(theinput) )



