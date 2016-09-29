use Cwd;
my $dir = getcwd;

while (<>)
{
 if(/^(.*\.o:)(.*)/)
 {
    print "$dir/build/arm/$1$2\n";
 }
 else
 {
   print;
 }
}
