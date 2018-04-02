#!/usr/bin/tclsh
set fp [open "2bit_shift_REG_hier.v" r]
set file_data [read $fp]
close $fp



set data [split $file_data "\n"]
#set modulenum 0
foreach line $data {
	if { [regexp {^module\s+(\w+)\(\s*(\w+),\s*(\w+),} $line word_all name_module pin1 pin2 ] } {
	puts stdout $word_all
	puts stdout $name_module 
	puts stdout $pin1 
	puts stdout $pin2 }
	
	#set words [split $line " ("]
	
	#if { [lindex $words 0] == "module"} { 
	
	#	incr modulenum
	#	puts stdout [lindex $words 1]  }
	
	#set var2 [ string $line 2]
#}
	#set str [string trim $str]
  #set str [string range $line 0 1]

 # if { $str != "//" } { puts stdout $line}



	

