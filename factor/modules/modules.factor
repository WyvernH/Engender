! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: engender.weights engender.corpus engender.analyse engender.generate engender.settings engender.devel ;
USING: kernel math io sequences accessors ;
IN: engender.modules


: print-module-help ( module-name -- )
    {
        { "analyse"  analyse-help }
        { "generate" generate-help }
        { "rank"     rank-help }
        { "corpus"   corpus-help }
        { "weights"  weights-help }
    } case
    [ print ] each "\n" write ;

: print-help ( -- )
    global-help print-module-help
    modules-help print-module-help ;

: verify-module-options ( module-options module-name -- module-options ? )
    over h>> [ print-module-help f ]
    [ drop t ] if ; ! specific error checking line

: analyse-module ( module-options -- )
    "analyse" verify-module-options
    [ perform-analyse ] [ drop ] if ; inline

: generate-module ( module-options -- )
    "generate" verify-module-options
    [ perform-generate ] [ drop ] if ; inline

: rank-module (  module-options -- )
    "rank" verify-module-options
    [ perform-rank ] [ drop ] if ; inline

: corpus-module (  corpus-options -- )
    "corpus" verify-module-options
    [ perform-corpus ] [ drop ] if ; inline

: weights-module ( weights-options -- )
    "weights" verify-module-options
    [ perform-weights ] [ drop ] if ; inline

: unknown-module ( global-options weights-options module-name -- )
    "!!! Unknown module '" write write "'" print print-help 2drop ; inline
