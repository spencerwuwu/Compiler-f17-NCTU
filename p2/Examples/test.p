/**
 * test.p: simple P language example
 */
//&T-
test;

var a: integer; // global variable

foo( a, b: integer ): integer;
func1(x, y: integer; z: string): boolean;
func2(a: boolean): string;
func3();
begin
    return a*b;
end
end foo

// main block
begin

print foo(2*4,3);
// procedure

end
end test
