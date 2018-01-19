//&P-
//&T-

fb;

fun();
begin
end
end fun

squ(a:real):real;
begin
    return a*a;
end
end squ 

funprint();
begin
    print "-------------------\n";
end
end funprint

ffg(a:integer):integer;
begin
    if a = 1 then
        return 1;
    end if
    return ffg(a-1)+a;
end
end ffg


begin
    var r:real;
    var i,j,h: integer;
    read r;
    funprint();
    fun();
    print squ(r);
    print "\n";
    print squ(r)+6;
    print "\n";
i:=0;
    for k := 1 to 10 do
        for m:= 1 to 10 do
                    funprint();
        end do
    end do

    while i < 2 do
j:=0;
        while j < 10 do
        for m:= 1 to 10 do
                    funprint();
        end do
                    print squ(r);
                    print "\n";
                    funprint();
                    j:=j+1;
            end do
            print "i\n";
        i:=i+1;
    end do

    print ffg(i)/7;
    print "\n";
    print squ(r)+6;
    print "\n";
    print (5+squ(r))/7;
    print "\n";
end
end fb
