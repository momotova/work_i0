#!/usr/bin/wish
wm title . "Window"
wm resizable . 1 1 
wm geometry . =700x500+10+10
menu .menubar
. configure -menu .menubar 
.menubar add cascade -label File -menu .menubar.file -underline 0
 menu .menubar.file -tearoff 0
.menubar.file add command -label Open -command "doIt .l1 .var"  
.menubar.file add command -label Exit -command exit
global file_read
global file_data
global var
global types
global file
set types {   {"SPICE netlists" {.cir .sp}  }  
 {"verilog" {.v}  }  
 {"All files"            *     } } 

proc doIt { entry} { 
global types
global file_data
global file
set file [tk_getOpenFile -filetypes $types -parent .] 
set fp [open $file r]
set file_data [read $fp]
close $fp

.t1 delete 1.0 end
.t1 insert end $file_data
#$label configure -text $file
$entry configure -textvariable $file

set data [split $file_data "\n"]
foreach line $data {
	global name_module
	if { [regexp {^module\s(\w+)} $line word_all name_module ] } {
	.t2 insert end "$name_module "  ""}
 } 
}
 

#label .l1 -text "File name"
entry .e -width 20  -textvariable "File name"

button .b1 -text "..." -command "doIt .e"   
focus .e
#button .b2 -text "Open file" -command   { entry_en $var }
grid .e -row 0 -column 1 -sticky w  
grid .b1 -row 0 -column 3 
#grid .b2 -row 1 -column 3 
#grid .l1 -row 0 -column 0 
text .t1 -width 60 -height 50 
text .t2 -width 40 -height 50 
grid .t1 -row 1 -column 0 
grid .t2 -row 1 -column 1

