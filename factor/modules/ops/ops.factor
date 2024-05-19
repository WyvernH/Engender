! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: engender.layouts ;
IN: engender.modules.ops


:: perform-analyse ( options -- )
    options w>> [ load-weight-data ] [ load-weights ] if
    options c>> [ load-corpus-data ] [ load-corpus ] if
    over ng-len>> >ng-corpus swap
    options layout-name>> find-meta-layout
    [ layout>> analyse-layout ] [ print-meta-layout ] bi
    "Score: " write number>string print "\n" write ;

: perform-generate ( options -- ) ;

: perform-rank ( options -- ) ;

: perform-corpus ( options -- ) ;

: perform-weights ( options -- ) ;
