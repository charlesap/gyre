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
                 | projection
                 | "IS" | "IN" | "AS" | "FLIPS"

        COMMENT: "--" /[^\n]*/ NEWLINE
        %ignore COMMENT
        NEWLINE: "\n"

        for : "for" CNAME "."
        identity : qname "is" identx
        identx : namelist "." | qname identity
        composition : CNAME "has" pairlist "."
        qualification : orlist "in" CNAME [mirrored] "."
        projection : qname projector modifier mname ("," mname)* "."

        modifier : "to" | "across" 
        projector : "maps" | "flips" | "flops" | "spreads" 
        mirrored : "mirrored"
        qname : CNAME [qual]
        mname : CNAME [qual] (subbullet CNAME [qual])*
        qual : subrange | subletter ( subletter | notsubletter )* | notsubletter ( subletter * notsubletter )*
        orlist : CNAME "|" CNAME ("|" CNAME)*
        namelist : [qname ("," qname)*]
        pairlist : [qname qname ("," qname qname)*]

        subletter : "ₕ" -> subh | "ₙ" -> subn | "ₛ" -> subs | "ₜ" -> subt | "ᵥ" -> subv 
        notsubletter : "ₕ̃" -> nsubh
        subrange : subnumber [ subdotdot subnumber ]
        subnumber : subdigit (subdigit)*
        subdigit : "₀" -> sub0 | "₁" -> sub1 | "₂" -> sub2 | "₄" -> sub4 | "₅" -> sub5 | "₆" -> sub6 | "₇" -> sub7
        subdotdot : "‥"
        subbullet : "•" 

        %import common.ESCAPED_STRING
        %import common.SIGNED_NUMBER
        %import common.CNAME
        %import common.WS
        %ignore WS

        """, start='value', parser='lalr')

    return( gyre_parser.parse(theinput) )



