! Copyright (C) 2023 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: engender.weights engender.corpus engender.analyse engender.generate engender.settings engender.devel engender.modules ;
USING: kernel math io sequences command-line namespaces accessors multiline ascii combinators splitting ;
IN: engender


! TODO error handling
: parse-global-options ( sequence -- global-options )
    global-options new swap [
        {
            { "-h"        [ t >>h ] }
            { "--help"    [ t >>h ] }
            { "-d"        [ t >>d ] }
            { "--dynamic" [ t >>d ] }
            [ drop ]
        } case
    ] each ;

! TODO error handling
: parse-module-option ( obj str -- )
    {
        { "-h"      [ t h<< ] }
        { "--help"  [ t h<< ] }
        { "-s"      [ t s<< ] }
        { "--score" [ t s<< ] }
    } case ; inline

! TODO error handling
! : parse-module-options ( sequence -- module-options )
!     modules-options new swap
!     [ first modules-options [ first ] {  } map-as find drop modules-options nth second ]
!     [ rest [ dup pick member? [ overd parse-module-option ] [ drop ] ] each ] bi ; inline
: parse-module-options ( sequence -- module-options ) drop module-options new ; inline

: parse-options ( 2array -- global-options module-options )
    [ first parse-global-options ] [ second parse-module-options ] bi ; inline

: find-module ( arguments -- i elt )
    [ modules but-last [ first ] {  } map-as member? ] find ; inline

: split-at ( i elt sequence -- elt array ) { 0 } 1sequence split-indices ; inline

: run-module ( global-options module-options module-name -- )
    modules case ; inline

! TODO exit on find-module failure
: parse-commands ( arguments -- global-options module-options module-name )
    [ find-module ] [ rot split-at parse-options ] bi rot ; inline

: engender ( arguments -- )
    parse-commands pick h>> [ 3drop print-help ] [ run-module ] if ;

/*
    [ print ] each print-help
    load-weight-data load-corpus-data over ng-len>> >ng-corpus
    swap weights>> init-matrix
    ! 100 [ matrix-next print-both "\n" write ] times
    36 [ matrix-next dup return-layout print-layout-readable print-score "\n" write ] times
    print-score-matrix dup return-layout print-layout-readable print-score "\n" write
    print-last-column
    3drop ;
*/

: engender-run ( -- ) command-line get engender ;

MAIN: engender-run
