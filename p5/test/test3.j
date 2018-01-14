; test3.j
.class public test3
.super java/lang/Object
.field public static _sc Ljava/util/Scanner;
.method public static main([Ljava/lang/String;)V
	.limit stack 100
	.limit locals 100
	new java/util/Scanner
	dup
	getstatic java/lang/System/in Ljava/io/InputStream;
	invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
	putstatic test3/_sc Ljava/util/Scanner;
	ldc 19
	istore 1
	; invoke java.util.Scanner.nextXXX()
	getstatic test3/_sc Ljava/util/Scanner;
	invokevirtual java/util/Scanner/nextInt()I
	istore 2
Lif_13:
	iload 2
	ldc 100
	isub
	ifgt Ltrue_13
	iconst_0
	goto Lfalse_13
Ltrue_13:
	iconst_1
Lfalse_13:
	ifeq Lelse_13
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc "b > 100\n"
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	goto Lexit_13
Lelse_13:
Lif_18:
	iload 2
	ldc 20
	isub
	iflt Ltrue_18
	iconst_0
	goto Lfalse_18
Ltrue_18:
	iconst_1
Lfalse_18:
	ifeq Lelse_18
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc "b < 20\n"
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	goto Lexit_18
Lelse_18:
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc "bbb \n"
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
Lexit_18:
Lexit_13:
	return
.end method
