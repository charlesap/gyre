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
         for brain.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'for'),
                                [Token('CNAME', 'brain')])])])

       assert toast('''
         brain is cortex, hippocampus, thalamus.
       ''') ==       Tree(Token('RULE', 'value'),
                          [Tree(Token('RULE', 'assertion'),
                                [Tree(Token('RULE', 'identity'),
                                      [Token('CNAME', 'brain'),
                                       Tree(Token('RULE', 'namelist'),
                                            [Token('CNAME', 'cortex'),
                                             Token('CNAME', 'hippocampus'),
                                             Token('CNAME', 'thalamus')])])])]) 

       assert toast('''
         L2 has pyramidal, grid, disp.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'composition'),
                                [Token('CNAME', 'L2'),
                                 Tree(Token('RULE', 'namelist'),
                                      [Token('CNAME', 'pyramidal'),
                                       Token('CNAME', 'grid'),
                                       Token('CNAME', 'disp')])])])]) 


       assert toast('''
         l | r in h mirrored.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [Tree(Token('RULE', 'qualification'),
                                [Tree(Token('RULE', 'orlist'),
                                      [Token('CNAME', 'l'), Token('CNAME', 'r')]),
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


