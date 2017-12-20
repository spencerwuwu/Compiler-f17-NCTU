# Usage:
./parser  [filename]

# Platform: 
Linux, Unix like system

# Abilities:
This is the extension semantic check or the previous project.
In this project, following functions are added to complete the 
abilities required in the spec.
- checkExpr_sem
	Check the type between a variable and its assignment.

- verifyVarDeclaration
	Find whether the variable called had been declared.

- getDeclarationType
	After confirming the variable had been declared,
	get the type of the variable.

- verifyFuncInvoke
	Check whether a function call had been declared.

- verifyFuncInvokeAttr
	Iterate through and check the parameters in a function
	call.

- checkReturnType
	Check whether the return type of a function declared
	is match with its declaration.

- checkArithmetic
	Check if the type of operands are avaliable for the 
	operation.

- verifyArrayIndex
	Check if the type in the array index to be an integer.

- getRemainArray
	Get the type of array being called.
	eg.
		 var c : array 1 to 5 of array 1 to 5 of integer;
	The type of c[1] should be 'integer [5]'.

- compareArrayType
	Compare the size and dimension of two array

- checkNotArray
	Type checking for 'print' and 'read' statements.

- checkIsBoolean
	Type checking for 'if' and 'while' statements.

- getTypeString, getOperString
	Generate elements for error printing.
