! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: sequences kernel prettyprint math math.functions math.parser io splitting combinators ;
USING: engender.weights engender.corpus engender.analyse engender.generate engender.settings ;
IN: engender.devel


: return-layout ( matrix -- layout ) first last ; inline

: print-layout ( matrix -- matrix ) dup return-layout "[" write write "]" print ; inline

: return-score ( ng-corpus weights matrix -- score ) return-layout default-analyse ; inline

: print-score ( ng-corpus weights matrix -- ng-corpus weights matrix ) 3dup return-score . ; inline

: print-both ( ng-corpus weights matrix -- ng-corpus weights matrix ) print-layout print-score ; inline

: return-score-matrix ( ng-corpus weights matrix -- score-matrix )
    -rot '[ [ _ _ rot default-analyse ] map ] map ;

: print-score-matrix ( ng-corpus weights matrix -- ng-corpus weights matrix )
    3dup return-score-matrix [ [ floor ] map ] map [ [ number>string but-last but-last " " append write ] each "\n" write ] each ;

: print-score-first-row ( ng-corpus weights matrix -- ng-corpus weights matrix )
    3dup return-score-matrix first . ;

: print-score-last-column ( ng-corpus weights matrix -- ng-corpus weights matrix )
    3dup return-score-matrix [ last . ] each ;

: print-first-row ( ng-corpus weights matrix -- ng-corpus weights matrix )
    dup first [ . ] each ;

: print-last-column ( ng-corpus weights matrix -- ng-corpus weights matrix )
    dup [ last . ] each ;

: evolve-1000 ( ng-corpus weights matrix -- ng-corpus weights 'matrix )
    1000 [ matrix-next print-score ] times ; inline

: print-spc ( 1str -- ) " [" write write "]" print ;
: print-spaces ( str -- ) [ " " swap suffix write ] each "\n" write ;
: print-layout-readable ( 31-char-str -- )
    { 1 11 21 } split-indices
    { [ second print-spaces ] [ third print-spaces ]
      [ fourth print-spaces ] [ first print-spc ] } cleave ;
