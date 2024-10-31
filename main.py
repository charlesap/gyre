# main 
from lark import Lark

example = Lark('''start: WORD "," WORD "!"

            %import common.WORD   // imports from terminal library
            %ignore " "           // Disregard spaces in text
         ''')

print( example.parse("Hello, World!") )

