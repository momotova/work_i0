#!/usr/bin/wish
wm title . "Window"
wm resizable . 1 1 

wm geometry . =700x500+10+10
menu .menubar
. configure -menu .menubar 
.menubar add cascade -label File -menu .menubar.file -underline 0
 menu .menubar.file -tearoff 0
.menubar.file add command -label Open -command open_file 
.menubar.file add command -label Exit -command exit

#set fp [open $var1 r]
#set file_data [read $fp]
#close $fp


#set data [split $file_data "\n"]
global file
proc open_file {file} {
set types {   {"SPICE netlists" {.cir .sp}  }  
 {"All files"            *     } } 

proc doIt {label} {  

global types  

set file [tk_getOpenFile -filetypes $types -parent .]  
$label configure -text $file 
} 
 



}   



label .l1 -text "File name"                 	 
entry .e -width 40 -textvariable var1
button .b1 -text "..." -command { open_file $file }
grid .e -row 0 -column 1 -sticky w
grid .b1 -row 0 -column 2
grid .l1 -row 0 -column 0

text .t1 -width 60 -height 50 
text .t2 -width 40 -height 50 
#.t1 insert  $data
grid .t1 -row 1 -column 0 
