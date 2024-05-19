! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: strings kernel sequences io.directories accessors io io.files.types namespaces unicode sorting io.encodings.ascii io.files ;
USING: engender.settings engender.devel ;
IN: engender.layouts

TUPLE: meta-layout
    { name string }
    { creator string }
    { layout string } ;

C: <meta-layout> meta-layout

: fifth ( seq -- elt ) 4 swap nth ; inline
: sixth ( seq -- elt ) 5 swap nth ; inline

: list-layout-files ( dir -- nameseq )
    directory-entries [ type>> +regular-file+ = ] filter
    [ name>> ] map
    [ [ 4 tail* ".lyt" = ] [ 7 tail* ".layout" = ] bi or ] filter ;

: get-layout ( seq -- layout )
    [ [ third ] [ fourth ] [ fifth ] tri
      append append CHAR: \s swap remove ]
    [ sixth second prefix ] bi ;

: load-meta-layout ( path -- meta-layout )
    ascii file-lines
    [ first ] [ second ] [ get-layout ] tri <meta-layout> ;

: load-meta-layouts ( -- seq )
    {  } layouts-directory get
    [ list-layout-files ] [ [ swap append ] curry map! ] bi
    [ load-meta-layout suffix ] each ;

: print-meta-layout ( meta-layout -- )
    [ name>> print ] [ creator>> print ] [ layout>> print-layout-readable ] tri ;

: sort-layouts-name ( seq -- newseq ) [ name>> ] sort-by ;

: find-meta-layout ( str -- meta-layout )
    load-meta-layouts [
        name>> [ >lower CHAR: \s swap remove ] bi@ =
    ] with find nip ;
