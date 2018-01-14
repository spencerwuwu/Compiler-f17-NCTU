; semtest1.j
.class public semtest1
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.field public static b I
.method public static fun(II)I
	.limit stack 100
	.limit locals 100
Lif_17:
	iload 0
	iload 1
	isub
	ifge Ltrue_17
	iconst_0
	goto Lfalse_17
Ltrue_17:
	iconst_1
Lfalse_17:
	ifeq Lelse_17
	iload 0
	ldc 3
	imul
	ireturn
	goto Lexit_17
Lelse_17:
	iload 1
	ldc 3
	imul
	ireturn
Lexit_17:
.end method

.method public static fun2()V
	.limit stack 100
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc "message\n"
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	return
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 100
	.limit locals 100
	new java/util/Scanner
	dup
	getstatic java/lang/System/in Ljava/io/InputStream;
	invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
	putstatic semtest1/_sc Ljava/util/Scanner;
	ldc 1
	istore 4
Lwhile_40:
	iload 4
	ldc 10
	isub
	iflt Ltrue_40
	iconst_0
	goto Lfalse_40
Ltrue_40:
	iconst_1
Lfalse_40:
	ifeq Lelse_40
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc "loop\n"
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	iload 4
	ldc 1
	iadd
	istore 4
	goto Lwhile_40
Lelse_40:
Lexit_40:
	ldc 10
	istore 1
	ldc 1
	ldc 2
	invokestatic semtest1/fun(II)I
	istore 1
	ldc 1
	ldc 2
	ldc 10
	ldc 10
	invokestatic semtest1/fun(II)I
	istore 1
	return
.end method
