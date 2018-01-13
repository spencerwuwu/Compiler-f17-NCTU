; semtest1.j
.class public semtest1
.super java/lang/Object
; Line #9: semtest1;
; Line #10: 
.field public static b I
; Line #12: var b : integer;
; Line #13: 
.method public static fun(II)I
	.limit stack 100
	.limit locals 100
; Line #14: fun( a,b : integer ) : integer;
; Line #15: begin
gg a 0
gg a 0
gg b 1
gg b 1
	iadd
	ireturn
; Line #16:     return a+b;
Lif_17:
	ifeq Lelse_17
gg a 0
gg a 0
gg b 1
gg b 1
	isub
	ifge Ltrue_17
	iconst_0
	goto Lfalse_17
Ltrue_17:
	iconst_1
Lfalse_17:
; Line #17:     if a >= b then
gg a 0
gg a 0
	ldc 3
	imul
	ireturn
; Line #18:         return a*3;
	goto Lexit_17
Lelse_17:
; Line #19:     else
gg b 1
gg b 1
	ldc 3
	imul
	ireturn
; Line #20:         return b*3;
Lexit_17:
; Line #21:     end if
; Line #22: end 
.end method

; Line #23: end fun
; Line #24: 
.method public static fun2()V
	.limit stack 100
	.limit locals 100
; Line #25: fun2();
; Line #26: begin
	ldc "message\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #27:     print "message\n";
; Line #28: end
.end method

; Line #29: end fun2
; Line #30: 
.method public static main([Ljava/lang/String;)V
	.limit stack 100
	.limit local 100
; Line #32: begin
; Line #33:     
; Line #34:     var aa : integer;
; Line #35:     var bb,cc : boolean;
; Line #36: 
; Line #37:     //&S+
Lfor_38:
	sipush 1
	istore 1
Ltrue_38:
	iload 1
	sipush 9
	isub
	iflt Ltrue_38
	iconst_0
	goto Lfalse_38
Ltrue_38:
	iconst_1
Lfalse_38:
	ifeq Lexit_38
; Line #38:     for i := 1 to 9 do 
	ldc "loop\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #39:         print "loop\n";
	iload 1
	sipush 1
	iadd
	istore 1
	goto Ltrue_38
Lexit_38:
Lexit_38:
; Line #40:     end do
gg a 0
gg a 0
	i2f
	i2f
; Line #41:     aa := a;
	ldc 1
	ldc 2
	i2f
	i2f
	ldc 1
	ldc 2
gg a 0
gg a 0
gg a 0
gg a 0
	i2f
	i2f
	return
.end method
; Line #46: end
