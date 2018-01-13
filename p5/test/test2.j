; test2.j
.class public test2
.super java/lang/Object
.field public static a I
.field public static b Z
.field public static c F
.field public static d F
.field public static e F

.method public static main([Ljava/lang/String;)V
	.limit stack 100
	limit local 100
	ldc 3
	i2f
	i2f
	ldc 0.000000
	i2f
	i2f
	ldc 0.000000
	ldc 3
	i2f
	fmul
	i2f
	i2f
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc 3
	ldc 0.000000
	fdiv
	i2f
	i2f
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc 1
	ldc 100
	iadd
	i2f
	i2f
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc 100
	ldc 17
	irem
	i2f
	i2f
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc 0.000000
	ldc 3
	i2f
	fsub
	i2f
	i2f
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc 100
	ldc 20
	iadd
	ldc 100
	ldc 0.000000
	fmul
	ldc 2
	i2f
	fdiv
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
	return
.end method
