BEGIN {
	FS="|";
  print "msc {";
  print "";
  print "hscale=\"1.5\";";
  print "";
  print "# Entities";
  print "PHY,MAC,SCHED,RLC,PDCP,RRC,SEC,UECM,CRM,S1AP,OAM;";
  print "";
}
/\|[ \t]*MESG[ \t]*\|/ {
  From=$5; 
  To=$6;
  Label=$4;
  Details=$NF
  if(id != "") {
    if($3 ~ id) {
      print(From " =>" To " [ label = \"" Label "\\n" Details "\" ];");
    }
  } else {
    print(From " =>" To " [ label = \"" Label "\\n" Details "\" ];");
  }
}
/\|[ \t]*FUNC[ \t]*\|/ {
  From=$5; 
  To=$6;
  Label=$4;
  Details=$NF;
  if(id != "") {
    if($3 ~ id) {
      print(From " ->" To " [ label = \"" Label "\\n" Details "\", linecolour=\"violet\", textcolour=\"violet\" ];");
    }
  } else {
    print(From " ->" To " [ label = \"" Label "\\n" Details "\", linecolour=\"violet\", textcolour=\"violet\" ];");
  }
}
/\|[ \t]*EVHDLR[ \t]*\|/ {
  Label=$4;
  Details=$NF;
  if(id != "") {
    if($3 ~ id) {
      print("UECM box UECM" " [ label = \"" Label "\\n" Details "\", linecolour=\"blue\", textcolour=\"blue\" ];");
    }
  } else {
    print("UECM box UECM" " [ label = \"" Label "\\n" Details "\", linecolour=\"blue\", textcolour=\"blue\" ];");
  }
}
/\|[ \t]*EVENT[ \t]*\|/ {
  From="UECM";
  To="UECM";
  Label=$4;
  Details=$NF;
  if(id != "") {
    if($3 ~ id) {
      print(From " box " To " [ label = \"Event " Label "\\n" Details "\", linecolour=\"blue\", textcolour=\"blue\" ];");
    }
  } else {
    print(From " box " To " [ label = \"Event " Label "\\n" Details "\", linecolour=\"blue\", textcolour=\"blue\" ];");
  }
}
/\|[ \t]*STATE[ \t]*\|/ {
  From="UECM";
  To="UECM";
  Details=$NF;
  if(id != "") {
    if($3 ~ id) {
     print(From " rbox " To " [ label = \"" Details "\", linecolour=\"teal\", textcolour=\"teal\" ];");
    }
  } else {
    print(From " rbox " To " [ label = \"" Details "\", linecolour=\"teal\", textcolour=\"teal\" ];");
  }
}
/\|[ \t]*TIMER[ \t]*\|/ {
  Label=$4;
  Details=$NF;
  if(index(Details,"EXPIRED") > 0) {
    if(id != "") {
      if($3 ~ id) {
        print("UECM box UECM" " [ label = \"" Label "\\n" Details "\", linecolour=\"red\", textcolour=\"red\" ];");
      }
    } else {
      print("UECM box UECM" " [ label = \"" Label "\\n" Details "\", linecolour=\"red\", textcolour=\"red\" ];");
    }
  }
}
END {
  print "}";
} 
