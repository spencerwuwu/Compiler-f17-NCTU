; test4.j
.class public test4
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.method public static main([Ljava/lang/String;)V
	.limit stack 100
	.limit locals 100
	new java/util/Scanner
	dup
	getstatic java/lang/System/in Ljava/io/InputStream;
	invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
	putstatic test4/_sc Ljava/util/Scanner;
	ldc 1
	istore 1
Lfor_8:
	iload 1
	ldc 10
	isub
	ifle Ltrue_8
	iconst_0
	goto Lfalse_8
Ltrue_8:
	iconst_1
Lfalse_8:
	ifeq Lexit_8
	ldc 1
	istore 2
Lfor_9:
	iload 2
	ldc 10
	isub
	ifle Ltrue_9
	iconst_0
	goto Lfalse_9
Ltrue_9:
	iconst_1
Lfalse_9:
	ifeq Lexit_9
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iload 1
	iload 2
	imul
	invokevirtual java/io/PrintStream/print(I)V
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc "\n"
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	iload 2
	sipush 1
	iadd
	istore 2
	goto Lfor_9
Lexit_9:
Lexit_9:
	iload 1
	sipush 1
	iadd
	istore 1
	goto Lfor_8
Lexit_8:
Lexit_8:
	return
.end method
