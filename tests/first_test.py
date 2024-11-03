# first test

from lark import Token, Tree

from operations import something, somethingelse, toast


def test_something():
    assert something(1) == 2
    assert something(6) == 7

def test_somethingelse():
    assert somethingelse(1) == 0
    assert somethingelse(6) == 5

def test_toast():
       assert toast('''
         for creature.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'for'),
                                [Token('CNAME', 'creature')])])])

       assert toast('''
         brainₕ is cortex, hippocampus, thalamus.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'identity'),
                                [Tree(Token('RULE', 'qname'),
                                      [Token('CNAME', 'brain'),
                                       Tree(Token('RULE', 'qual'),
                                            [Tree('subh', [])])]),
                                 Tree(Token('RULE', 'identx'),
                                      [Tree(Token('RULE', 'namelist'),
                                            [Tree(Token('RULE', 'qname'),
                                                  [Token('CNAME', 'cortex'), None]),
                                             Tree(Token('RULE', 'qname'),
                                                  [Token('CNAME', 'hippocampus'), None]),
                                             Tree(Token('RULE', 'qname'),
                                                  [Token('CNAME', 'thalamus'), None])])])])])])


       assert toast('''
         L2 has pyramidal p, grid g, disp d.
       ''') == Tree(Token('RULE', 'value'), 
                    [Tree(Token('RULE', 'assertion'), 
                          [Tree(Token('RULE', 'composition'), 
                                [Token('CNAME', 'L2'),
                                 Tree(Token('RULE', 'pairlist'),
                                      [Tree(Token('RULE', 'qname'),
                                            [Token('CNAME', 'pyramidal'), None]),
                                       Tree(Token('RULE', 'qname'),
                                            [Token('CNAME', 'p'), None]),
                                       Tree(Token('RULE', 'qname'),
                                            [Token('CNAME', 'grid'), None]),
                                       Tree(Token('RULE', 'qname'),
                                            [Token('CNAME', 'g'), None]),
                                       Tree(Token('RULE', 'qname'),
                                            [Token('CNAME', 'disp'), None]),
                                       Tree(Token('RULE', 'qname'),
                                            [Token('CNAME', 'd'), None])])])])])

       assert toast('''
         l | r in h mirrored.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'qualification'),
                                [Tree(Token('RULE', 'orlist'),
                                      [Token('CNAME', 'l'),
                                       Token('CNAME', 'r')]),
                                 Token('CNAME', 'h'),
                                 Tree(Token('RULE', 'mirrored'), [])])])])

       assert toast('''
         l | b | r in v.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'qualification'),
                                [Tree(Token('RULE', 'orlist'),
                                      [Token('CNAME', 'l'),
                                       Token('CNAME', 'b'),
                                       Token('CNAME', 'r')]),
                                 Token('CNAME', 'v'), None])])])


