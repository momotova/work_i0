#!/usr/bin/wish
menu .menubar -type menubar
.menubar add cascade -label File -menu .menubar.file -underline 0

#file menu
menu .menubar.file -tearoff 0
.menubar.file add command -label "New" -underline 0 \
    -command { new } 
.menubar.file add command -label "Open..." -underline 0 \
    -command {
        set fileid [open $filename r]
         # filename is set in file_save_as
        set data [read $fileid $filesize]
        close $fileid
        .text.t insert end $data
        wm title . $filename
    }

proc save { } {
    set data [.text.t get 1.0 {end -1c}]
    set fileid [open $filename w]
    puts -nonewline $fileid $data
    close $fileid
}

proc file_save_as { } {
    global filename
    set data [.text.t get 1.0 {end -1c}]
    set file_types {
     {"Tcl Files" { .tcl .TCL .tk .TK} }
     {"Text Files" { .txt .TXT} }
     {"All Files" * }
    }

    set filename [tk_getSaveFile -filetypes $file_types\
        -initialdir pwd -initialfile $filename\
        -defaultextension .tcl]
    wm title . $filename
    set fileid [open $filename w]
    puts -nonewline $fileid $data
    close $fileid
}

tk appname "Edit"
