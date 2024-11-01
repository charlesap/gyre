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
        identity : qname "is" namelist "."
        composition : CNAME "has" namelist "."
        qualification : orlist "in" CNAME [mirrored] "."
        projection : CNAME projector modifier CNAME "."

        modifier : "to" | "accross" 
        projector : "maps" | "flips" | "flops" | "spreads" 
        mirrored : "mirrored"
        qname : CNAME [qual]
        qual : subrange | subletter ( subletter | notsubletter )* | notsubletter ( subletter * notsubletter )*
        orlist : CNAME "|" CNAME ("|" CNAME)*
        namelist : [CNAME ("," CNAME)*]

        subletter : "ₕ" -> subh | "ₙ" -> subn | "ₜ" -> subt | "ᵥ" -> subv 
        notsubletter : "ₕ̃" -> nsubh
        subrange : subnumber [ subdotdot subnumber ]
        subnumber : subdigit (subdigit)*
        subdigit : "₀" -> sub0 | "₁" -> sub1 | "₂" -> sub2 | "₄" -> sub4 | "₆" -> sub6 | "₇" -> sub7
        subdotdot : "‥"
        subbullet : "•" 

        %import common.ESCAPED_STRING
        %import common.SIGNED_NUMBER
        %import common.CNAME
        %import common.WS
        %ignore WS

        """, start='value', parser='lalr')

    return( gyre_parser.parse(theinput) )



