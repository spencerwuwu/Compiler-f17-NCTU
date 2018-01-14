//&T-
/**
 *  semtest1.p :
 *
 *  There is no syntax error, but exists some semantic errors.
 *  You must indicate all of the semantic errors.
 *
 */
semtest1;

var a : 10;       // a is integer constant 
var b : integer;

fun( a,b : integer ) : integer;
begin
    return a+b;
    if a >= b then
        return a*3;
    else
        return b*3;
    end if
end 
end fun

fun2();
begin
    print "message\n";
end
end fun2

// main program
begin
    
    var aa : integer;
    var bb,cc : boolean;
    var i : integer;

    //&S+
    i := 1;
    while i < 10 do
        print "loop\n";
        i := i + 1;
    end do
    aa := a;
    aa := fun(1,2);    // ok
    fun(1,2);          // ok
    aa := fun(a,a);    // ok
    fun2();            // ok
end
end semtest1
