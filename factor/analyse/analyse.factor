! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: engender.weights engender.corpus engender.settings ;
USING: kernel sequences math generalizations ranges random splitting ;
IN: engender.analyse

: 5pick ( v w x y z -- v w x y z v ) [ reach ] dip swap ; inline
: 4bi ( quot1 quot2 -- ) 4 nbi ; inline
: >slice ( seq -- slice ) 0 tail-slice ;

: ng-final-quotation ( weights seq time-sum ngraph -- weights seq+1 )
    [ pick ] dip [ first ] [ last ] bi digraph-score + suffix ; inline

DEFER: ng-recurse

: ng-recurse-quotation ( weights seq n time-sum ngraph -- weights newseq )
    5pick reach pick nth pick first swap digraph-score rot + spin tail ng-recurse ; inline

: ng-recurse ( weights seq time-sum ngraph -- weights newseq )
    [ dup length 1 - [1..b) -rot [ ng-recurse-quotation ] 2curry each ]
    [ ng-final-quotation ] 2bi ; inline recursive

: ng-time ( weights seq time-sum ngraph -- greatest-time ) ng-recurse nip maximum ; inline

: ngraph-score ( weights ngraph -- score )
    [ {  } 0 ] dip [ ng-time ] [ but-last ng-time ] 4bi - ; inline

! note: ngram is an array of alphabet indices
: ngram-score ( weights ngram key-array -- score )
    [ nth ] curry map ngraph-score ; inline

: layout>key-array ( str -- array ) [ alphaindex ] {  } map-as ; inline
: key-array>layout ( array -- str ) [ alphabet nth ] "" map-as ; inline

! default-analyse options
! gives the time it takes to type the entire corpus using the given layout
: analyse-key-array ( ng-corpus weights key-array -- score )
    0 -roll '[ [ second ] [ first ] bi _ swap _ ngram-score * + ] each ; inline
: analyse-layout ( ng-corpus weights layout -- score ) layout>key-array analyse-key-array ; inline
