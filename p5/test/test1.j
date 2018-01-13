; test1.j
.class public test1
.super java/lang/Object
; Line #5: test1;
; Line #6: 
.field public static a I
; Line #7: var a: integer;
.field public static b Z
; Line #8: var b: boolean;
.field public static c F
.field public static d F
.field public static e F
; Line #9: var c, d, e: real;
; Line #10: 
.method public static main([Ljava/lang/String;)V
	.limit stack 100
	.limit local 100
; Line #11: begin
; Line #12:         var a, b, c, d, e: integer;
; Line #13:         var f: boolean;
; Line #14:         var g: real;
; Line #15:         var h: "hello world!";
; Line #16:         f := true;
; Line #17:         f := false;
	ldc 1234567
	i2f
	i2f
; Line #18:         a := 1234567;
	ldc 0.000000
	i2f
	i2f
; Line #19:         g := 2.86;
; Line #20: 
; Line #21: 
gg a 1
gg a 1
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #25:         print a;
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #26:         print "\n";
gg f 6
gg f 6
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #27:         print f;
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #28:         print "\n";
gg g 7
gg g 7
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #29:         print g;
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #30:         print "\n";
gg h 0
gg h 0
	getstatic java/lang/System/out Ljava/io/PrintStream;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #31:         print h;
	ldc "\n"
	getstatic java/lang/System/out Ljava/io/PrintStream;
	invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
; Line #32:         print "\n";
	return
.end method
; Line #34: end
