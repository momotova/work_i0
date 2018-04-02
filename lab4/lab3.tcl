#!/usr/bin/wish
#label .l1 -text "This is a text"
#grid .l1 -row 0 -column 0
wm title . "Window"
wm resizable . 1 1 
#wm resizable . 0 1
#wm resizable . 1 0 

wm geometry . =700x500+10+10

label .l1 -text "Text 1" -foreground Yellow -background Blue
label .l2 -text "Text 2" -font { -size 24 }
label .l3 -text "Centered" -relief raised

grid .l1 -row 0 -column 0
grid .l2 -row 0 -column 1
grid .l3 -row 1 -column 0 -columnspan 2

proc b1_proc {} {
	puts stdout "Hello"
}

proc b4_proc { var1 } {
	puts stdout $var1
}

button .b1 -text "Hello" -command b1_proc
button .b2 -text "Quit" -command { destroy . }

grid .b1 -row 2 -column 0
grid .b2 -row 2 -column 1

label .l -text "Enter"

entry .e -width 40 -textvariable var1

button .b3 -text "Clear" -command { set var1 ""}
button .b4 -text "Enter var" -command { b4_proc $var1 }

focus .e

grid .l -row 3 -column 0 -sticky e
grid .e -row 3 -column 1 -sticky w
grid .b3 -row 4 -column 0 
grid .b4 -row 4 -column 1 


canvas .c -width 250 -height 100
.c create rectangle 0 0 70 60 -fill blue  
.c create oval 0 0 50 50 -fill red 
.c create line 50 50 100 100 -fill green -width 4

grid .c -row 5 -column 0


menu .menubar
. configure -menu 