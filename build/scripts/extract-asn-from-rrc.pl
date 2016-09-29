# extract ASN.1 from RRC 36331 spec saved as text file
while(<>) {
  if (/--\s+ASN1START/) {
    $isAsn = 1;
  }
  if ($isAsn eq 1) {
     # replace tabs with space
     $_ =~ s/[\t+]/ /g;
     print;
  }
  if(/--\s+ASN1STOP/) {
    $isAsn = 0;
  }
}
