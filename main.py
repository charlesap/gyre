# main 
from lark import Lark

from operations import something, somethingelse

example = Lark('''start: WORD "," WORD "!"

            %import common.WORD   // imports from terminal library
            %ignore " "           // Disregard spaces in text
         ''')

print( example.parse("Hello, World!") )
print( something(1))
print( somethingelse(1))

