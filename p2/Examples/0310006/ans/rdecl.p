1: /**
2:  * decl.p: declarations
3:  */
4: //&T-
5: decl;
6: 
7: // global variables declaration
8: var ga: integer;
9: var gb, gc: boolean;
10: var gd: real;
11: var ge: string;
12: var garray: array 1 to 10 of integer;
13: 
14: // global constants declaration
15: var PI: 3.1415926;
16: var course: "Introduction to Compiler Design";
17: var isPass: true;
18: 
19: begin
20:         // local constants declaration
21:         var arraySize: 1024;
22: 
23:         // local variables declaration
24:         var la: integer;
25:         var lb, lc, ld: boolean;
26:         var le, lf: real;
27:         var lg: string;
28:         var L1darray: array 1 to 5 of integer;                  // one dimensional
29:         var L2darray: array 1 to 5 of array 1 to 3 of integer;  // two dimensional
30: 
31: 
==============================================================================================================
Name                             Kind       Level      Type             Attribute  
--------------------------------------------------------------------------------------------------------------
arraySize                        constant   1(local)   integer          1024
la                               variable   1(local)   integer          
lb                               variable   1(local)   boolean          
lc                               variable   1(local)   boolean          
ld                               variable   1(local)   boolean          
le                               variable   1(local)   real             
lf                               variable   1(local)   real             
lg                               variable   1(local)   string           
L1darray                         variable   1(local)   integer [5]      
L2darray                         variable   1(local)   integer [5][3]   
--------------------------------------------------------------------------------------------------------------
32: end
==============================================================================================================
Name                             Kind       Level      Type             Attribute  
--------------------------------------------------------------------------------------------------------------
decl                             program    0(global)  void             
ga                               variable   0(global)  integer          
gb                               variable   0(global)  boolean          
gc                               variable   0(global)  boolean          
gd                               variable   0(global)  real             
ge                               variable   0(global)  string           
garray                           variable   0(global)  integer [10]     
PI                               constant   0(global)  real             3.141593
course                           constant   0(global)  string           "Introduction to Compiler Design"
isPass                           constant   0(global)  boolean          true       
--------------------------------------------------------------------------------------------------------------
33: end decl
