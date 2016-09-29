#!/usr/bin//perl
#variable Declaration 
@files = 0;
$fileModified = 0;
$delimit = '%8ld';
$bin = '../build/obj/enodeb';
$binTime = ( stat $bin)[9];

if ($binTime) {
   $APPDFILE = 0;
#   print"\n TeNB_Diag.tmp file getting created \n";
   open (APPDFILE, '>' , './TeNB_Diag.tmp') || die("Data Base ERROR: Cannot open the file: TeNB_Diag_db.txt");
} else {
   # Opening a new Data Base File
   $OUTFILE = 0;
#   print "\n TeNB_Diag_db.txt file getting created \n";
   open ($OUTFILE, '>' , './TeNB_Diag_db.txt') || die("Data Base ERROR: Cannot open the file: TeNB_Diag_db.txt");
}

$path = './obj';
$prd   ='0x0';
$pattern= 'ssDiagFix';
getlist($path);
openfiles ($prd, @files);

# Sub Modules Start
# check is file modified
sub checkfilemod {
   $file = $filename;
   chomp ($file);
   my $fileTime = ( stat $file)[9];
   my $diffTime = $binTime - $fileTime;
#   print "checkfilemod: BinTime: $binTime FileTime:$fileTime diff:$diffTime\n";
   if($diffTime < 0) {
      $fileModified = 2;
      # the fileModified will have 1 - Unmodified, 2 - Modified, 0 - invalid
   }
   else {
      $fileModified = 1;
   }
}

#Get the list of files from a dir
sub getlist {
   my $readdir = shift;
   chdir ("$path");
   @files = `ls *.i`;
#   print "@files";
}

#open the file and does the token insertion, also add the same one in the
# data base file.
sub hdlfile{
   $stopProess = shift;

   if ($stopProess == 2)
   {
      open $tmpfile, "> tmp.txt" || die("Data Base ERROR: Cannot Create temporary file");
      open(INFILE, "< $filename") || die("Data Base ERROR: Cannot open the file $filename for parse");
      while (<INFILE>) {
         if (/(\s*.*)(\s$pattern\()(\s*.*\,)(\s*\".*\")(\s*.*)/)
         {
#            print "start=$1  pattern=$2 Var=$3 String=$4 end=$5\n"; 
            $CStart   = $1;
            $CPattern = $2;
            $CVarible = $3;
            $CString  = $4;
            $CEnd     = $5;
            $matchcount++;

            @varibles = split(/,/, $3);
#            print"@varibles\n";
#            print"\n$CStart$CPattern$CVarible$CString$CEnd\n";

            $token = sprintf("%01s%03x%04x", $prd,$filecount,$matchcount);
            $NewVarible = sprintf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,",$token,@varibles[1], @varibles[2], @varibles[3], @varibles[4], @varibles[5], @varibles[6], @varibles[7], @varibles[8], @varibles[9], @varibles[10], @varibles[11]);

            print $tmpfile "$CStart$CPattern$NewVarible$CString$CEnd\n\n";

#            print $tmpfile "  $1$2$token,$enum$splarg$print$7\n";
#            @enum   = split(/,/, $enum);
#            $enum   = @enum[0];
#            print "$token:$filename enum=$enum,$print\n";

#            @splarg = split(/,/, $splarg);
#            $splarg = @splarg[0];

            $CString =~ s/"//;
            $CString =~ s/\"//;
#            $dbentry = sprintf("%010s; |%10s |%20s |%s", $token, $enum, $splarg, $print);

            $enum = @varibles[1];
            @enums = split(/_/, $enum);
            $idx   = @enums;
            $enum1 = $enums[$#enums];

            if( $enum =~ /NA/ ) {
               $dbentry = sprintf("%010s; |%10s      |%s", $token, $enum1, $CString);
            }
            else {
               $dbentry = sprintf("%010s; |%10s %s |%s", $token, $enum1, $delimit, $CString);
            }

            #$dbentry = sprintf("%010s; |%10s |%s", $token, $enum, $print);

            if ($binTime) {
               print APPDFILE "$dbentry\n"; 
            } else {
               print $OUTFILE "$dbentry\n"; 
            }
         }
         else
         {
            print $tmpfile $_;
         }
      }

      close(INFILE);
      close ($tmpfile);
      `cp -f tmp.txt $filename`;
      `dos2unix -q $filename`;     
   }
}

# open the files
sub openfiles {
   ($prd,@filelst) = @_;
   $filecount   = 0;

   foreach (@filelst) {
      $filename = $_;
      $filecount++;
      $matchcount = 0;

      if ($binTime) {
         checkfilemod ();
      } else {
         $fileModified = 2;
      }

      hdlfile($fileModified);

   }
   `rm -f tmp.txt`;
}
#end of sub modules


if ($binTime) {
   close (APPDFILE) || die("Data Base ERROR: Cannot Close the file TeNB_Diag_db.txt");
   $path1 = "../";
   chomp $path1;
   chdir $path1;
   open (TEMPDB,'<' , './TeNB_Diag.tmp') || die("Data Base ERROR: Cannot open the file TeNB_Diag.tmp");
   open (DB, '<' , './TeNB_Diag_db.txt') || die("Data Base ERROR: Cannot open the file TeNB_Diag_db.txt");
   open (TDBF, '>' , './TeNB_Diag_Part.tmp') || die("Data Base ERROR: Cannot open the file TeNB_Diag.tmp");

   $idx = 0;
   @tknLst = 0;

   foreach(<TEMPDB>) {
      if(/(\s*.*\;)(\s*.*)/)
      {
         $TBool = 1;
         $Ttoken = $1;
         chop ($Ttoken);
         @Tchars = split(//, $Ttoken);
         $Ttkn = sprintf("%s%s%s%s%s%s", @Tchars[0], @Tchars[1], @Tchars[2], @Tchars[3], @Tchars[4], @Tchars[5]);
         foreach(@tknLst)
         {
            if($_ eq $Ttkn)
            {
               $TBool = 0;
            }

         }
         if($TBool == 1)
         {
            @tknLst[$idx] = $Ttkn;
            $idx++;
         } 
         print TDBF $_;
      }
   }

   foreach(<DB>) {
      if(/(\s*.*\;)(\s*.*)/)
      {
         $Bool = 1;
         $token = $1;
         chop ($token);
         @chars = split(//, $token);
         $tkn = sprintf("%s%s%s%s%s%s", @chars[0], @chars[1], @chars[2], @chars[3], @chars[4], @chars[5]);
         foreach(@tknLst)
         {
            if($_ eq $tkn)
            {
               $Bool = 0;
            }

         }
         if($Bool == 1)
         {
            print TDBF $_;

         } else {
#            print "Skipping Token $token\n";
         }
      }
   }

   close (TEMPDB);
   close (DB);
   close (TDBF);  
   `cp -rfL TeNB_Diag_Part.tmp TeNB_Diag_db.txt`; 
   `rm -rf TeNB_Diag_Part.tmp TeNB_Diag.tmp`;
} else {
   close ($OUTFILE) || die("Data Base ERROR: Cannot Close the file TeNB_Diag_db.txt");
}
`rm -rf obj/*.a`;
exit;
# End of Script
