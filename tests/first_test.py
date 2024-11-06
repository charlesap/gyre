# first test

from lark import Token, Tree

from operations import toast, blast


def test_toast():
       assert toast('''
         for creature.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [('FOR', 'creature', None, None, None)])])

       assert toast('''
         brainₕ is cortex, hippocampus, thalamus.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [('IS', 'brain', [Tree('s', 'ₕ')],
                            [[('cortex', None), ('hippocampus', None), ('thalamus', None)]], None)])])

       assert toast('''
         L2 has pyramidal p, grid g, disp d.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [('HAS', 'L2', None, (('pyramidal', None), ('p', None)), None)])])

       assert toast('''
         l | r in h mirrored.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [('IN', 'h', [Token('CNAME', 'l'), Token('CNAME', 'r')], None, None)])])

       assert toast('''
         l | b | r in v.
       ''') == Tree(Token('RULE', 'value'),
                    [Tree(Token('RULE', 'assertion'),
                          [('IN', 'v', [Token('CNAME', 'l'), Token('CNAME', 'b'), Token('CNAME', 'r')], None, None)])])


def test_blast():
    thecst = {}
    scopes = {}
    blast(toast("for creature."),thecst,scopes)
    assert thecst == {} 
    assert scopes == {'creature': [1, True]}
