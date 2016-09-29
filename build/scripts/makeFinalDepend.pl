use Cwd;
my $dir = getcwd;

while (<>)
{
 if(/^(.*\.o:)(.*)/)
 {
    print "$dir/build/x86/$1$2\n";
 }
 else
 {
   print;
 }
}
