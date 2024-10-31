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
    assert toast("Hello, World!") == Tree(Token('RULE', 'start'), [Token('WORD', 'Hello'), Token('WORD', 'World')])


