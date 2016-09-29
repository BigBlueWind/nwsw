BEGIN {
	FS="|";
	OFS="|";
}
{
  search="/";
  n=split($7,array,search);
  if(id != "") {
    if($3 ~ id) {
      printf("%14s | %6s | %4s | %30s | %5s | %5s | %-25s  |  %s\n",$1,$2,$3,$4,$5,$6,array[n],$8);
    }
  } else {
      printf("%14s | %6s | %4s | %30s | %5s | %5s | %-25s  |  %s\n",$1,$2,$3,$4,$5,$6,array[n],$8);
  }
}
