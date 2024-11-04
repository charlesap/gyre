# operations 
from lark import Lark, Transformer, Tree


def something(param):
    """Does something."""
    return(param+1)

def somethingelse(param):
    """Does something."""
    return(param-1)

class GyreTransformer(Transformer):

    def scope(self, args):
        return "FOR",""+args[0],None,[]

    def qname(self, args):
        return ""+args[0],args[1]

    def mname(self, args):
        return ""+args[0],args[1]

    def qual(self, args):
        rv = []
        for subtree in args[0].children:
            rv.append(subtree)
        return list(args)

    def identx(self, args):
        return args

    def namelist(self, args):
        return args

    def pairlist(self, args):
        return args[0],args[1]

    def orlist(self, args):
        return args

    def projector(self, args):
        return ""+args[0]

    def layout(self, args):
        nm0, nm1 = args[0]
        return "BORDERS",nm0,nm1,args[1]

    def modifier(self, args):
        return ""+args[0]

    def identity(self, args):
        nm0, nm1 = args[0]
        return "IS",nm0,nm1,args[1]

    def composition(self, args):
        return "HAS",""+args[0],None,args[1]

    def qualification(self, args):
        return "IN",""+args[1],args[0]

    def projection(self, args):
        if len(args) == 4:
            how = args[2].upper()
            rest = args[3]
        else:
            how = args[2].upper()
            mod = args[3]
            rest = args[4]
        name=args[0][0]
        more=args[0][1]
        return args[1].upper(),name,more,how,rest

    def subletter(self, args):
        return Tree("s",""+args[0])

    def notsubletter(self, args):
        return Tree("n",""+args[0])

    def subdigit(self, args):
        return Tree("d",""+args[0])

    def subnumber(self, args):
        return Tree("n",args[0])

    def subrange(self, args):
        return Tree("r",args)

def toast(theinput):
    gyre_parser = Lark(r"""
        value: (assertion)*

        assertion: scope
                 | identity
                 | composition
                 | qualification
                 | projection
                 | layout

        COMMENT : "--" /[^\n]*/ NEWLINE
        %ignore COMMENT
        NEWLINE : "\n"
        TO : "to"
        ACROSS : "across"
        MAPS : "maps"
        FLIPS : "flips"
        FLOPS : "flops"
        SPREADS : "spreads"
        MIRRORED : "mirrored"

        scope : "for" CNAME "."
        identity : qname "is" identx
        identx : namelist "." | qname identity
        composition : CNAME "has" pairlist "."
        qualification : orlist "in" CNAME [mirrored] "."
        projection : qname projector modifier mname ("," mname)* "."
        layout : qname "borders" namelist "."

        modifier : TO | ACROSS 
        projector : MAPS | FLIPS | FLOPS | SPREADS
        mirrored : MIRRORED
        qname : CNAME [qual]
        mname : CNAME [qual] (subbullet CNAME [qual])*
        qual : subrange | subletter ( subletter | notsubletter )* | notsubletter ( subletter * notsubletter )*
        orlist : CNAME "|" CNAME ("|" CNAME)*
        namelist : [qname ("," qname)*]
        pairlist : [qname qname ("," qname qname)*]

        !subletter : /ₕ/ | /ₙ/ | /ₛ/ | /ₜ/ | /ᵥ/
        !notsubletter : /ₕ̃/
        subrange : subnumber [ subdotdot subnumber ]
        subnumber : subdigit (subdigit)*
        !subdigit : /₀/ | /₁/ | /₂/ | /₄/ | /₅/ | /₆/ | /₇/
        !subdotdot : /‥/
        !subbullet : /•/ 

        %import common.ESCAPED_STRING
        %import common.SIGNED_NUMBER
        %import common.CNAME
        %import common.WS
        %ignore WS

        """, start='value', parser='lalr', transformer=GyreTransformer())

    return( gyre_parser.parse(theinput) )


