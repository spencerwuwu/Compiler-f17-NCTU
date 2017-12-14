# Usage:
./parser  [filename]

# Platform: 
Linux, Unix like system

# Abilities:
This parser inhierented the syntax parser from the last time, and generate a
symbol table declared in every scopes. This semantic parser only checks whether
the variable has been declared before or not.

A scope is similar to C. Name declaraion is unique in a scope. A compound 
statement and a function declaration forms a local scope.

A for loop also forms a scope, each loop must maintain a distinct loop iterator,
which should not be the same as other declarations within the loop, or inner 
nested for loop.

The format of the parser is as the following example:
============================================================
Name       Kind       Level      Type          Attribute
------------------------------------------------------------
a          variable   2(local)   string		  "Hello"
------------------------------------------------------------

and
=====================================================================
Name       Kind       Level      Type          Attribute
---------------------------------------------------------------------
error      program    0(global)  void
func1      function   0(global)  real [10]     string, integer [2]
---------------------------------------------------------------------

The maximum length of name is 32 characters. Name longer than this lenghth would be
cut off.

An variable redeclaration will print the follwing message:
<Error> found in Line 24: symbol 'name is redeclared

