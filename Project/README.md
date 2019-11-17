# CSE 570 Fall 2019

Lex and yacc calculator

Added exit feature. Type ```EXIT``` to end the program.  
Added feature to save most recent calculation. Enter argument with no left hand side to use that value as the left hand side.  
For example:
```
5*2
10
/2
5
```

# Compile 
1. ```lex calc.lex```
2. ```yacc -d calc.yacc````
3. ```cc lex.yy.c y.tab.c -o [output name]```

# Run 
```./[output_name]```
