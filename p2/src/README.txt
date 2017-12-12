# Usage:
./parser  [filename]

# Platform: 
Linux, Unix like system

# Abilities:

  This parser can only do the syntactically check of the input p program. It checks the following syntax. The following elements will all eventually reduce into Program, which is defined as the following form.
  ```
	identifier;
	<zero or more variable and constant declaration>
	<zero or more function declaration>
	<one compound statement>
	end identifier

  ```

1. Function:
  ```
	identifier (<zero or more formal arguments>): type;
	<one compound statement>
	end identifier

  ```
  The formal arguments are declared in a formal argument section, each declaration has the form
  ```
	  identifier_list: type
	  identifier_list-> identifier, identifier, ..., identifier
  ```

2. Data Types and Declarations
  There are four scalar data types: integer, real, string, and boolean, and a structure type array.
  ```
	var identifier list: scalar type;
	var identifier list: array integer constant to integer constant of type;

  ```
  constant
  ```
	var identifier list: literal constant;
  ```

3. Statements 
  There are seven distinct types of statements: compound, simple, conditional, while, for, return, and procedure call.

  3-1 Compound
  ```
	begin
	<zero or more variable and constant declaration>
	<zero or more statements>
	end

  ```

  3-2 Simple
  ```
	variable reference := expression;
	print variable reference; or print expression;
	read variable reference;

	* variable reference-> integer or
	  identifier [integer expression] [integer expression] [...]

	* expression
	  +, /, mod, .....

  ```

  3-3 Conditional
  ```
	if boolean expr then
	<zero or more statements>
	else
	<zero or more statements>
	end if

	if boolean expr then <zero or more statements> end if

  ```

  3-4 While
  ```
	while boolean expr do
	<zero or more statements>
	end do

  ```

  3-5 For
  ```
	for identifier := integer constant to integer constant do
	<zero or more statements>
	end do

  ```

  3-6 Return
  ```
	return expression ;

  ```

  3-7 Function invocation
  ```
	identifier (<expressions separated by zero or more comma>) ;
  ```


