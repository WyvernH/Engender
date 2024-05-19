! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: kernel sequences multiline math math.functions math.parser io random ranges ;
USING: prettyprint ;
USING: engender.generate.defns engender.corpus engender.analyse engender.settings ;
IN: engender.generate


! analyse-layout, analyse-key-array
ALIAS: default-analyse analyse-layout
! new-alphabet, random-layout
ALIAS: default-layout new-alphabet
! alpha-key-array, random-key-array
ALIAS: default-key-array alpha-key-array
! default-layout, default-key-array
ALIAS: default-layout-type default-layout
! evolve, evolve-working-set, force-evolve, force-evolve-working-set
ALIAS: default-evolve force-evolve-working-set


: copy-all ( src dest -- ) 0 swap copy ; inline

: copy-into ( matrix elt n -- )
    [ rot nth copy-all ] 2curry each ; inline

! other definition (be careful with stack-effect):
/*
: copy-into ( matrix elt n -- new-matrix )
    '[ _ clone _ pick set-nth ] map ; inline
*/

! note: destructive (3d)sequence operation
: matrix-spread ( matrix -- )
    dup [ rot last rot copy-into ] curry each-index ; inline

! other definition using second copy-into definition
/*
: matrix-spread ( matrix -- )
    dup [ [ last ] dip copy-into ] each-index drop ; inline
*/

: evolve-row ( row degree -- )
    [ default-evolve ] curry each ; inline

: best-in-row ( row ng-corpus weights -- default-layout-type )
    [ rot default-analyse ] 2curry minimum-by ; inline

: optimise-row ( row ng-corpus weights -- )
    [ best-in-row clone ] 2curry [ set-last ] bi ; inline

: evolve-matrix ( matrix -- )
    [ 1 + evolve-row ] each-index ; inline

: optimise-matrix ( ng-corpus weights matrix -- ng-corpus weights )
    pick pick [ optimise-row ] 2curry each ; inline

: matrix-next-chain ( ng-corpus weights matrix -- ng-corpus weights )
    [ matrix-spread ] [ evolve-matrix ] [ optimise-matrix ] tri ; inline


/* potentially faster operation
: evolve&optimise ( ng-corpus weights matrix -- ng-corpus weights )
    pick pick '[ [ 1 + evolve-row ] curry [ _ _ optimise-row ] bi ] each-index ;
: matrix-next-chain ( ng-corpus weights matrix -- ng-corpus weights )
    [ matrix-spread ] [ evolve&optimise ] bi ;
*/


: matrix-next ( ng-corpus weights matrix -- ng-corpus weights matrix ) [ matrix-next-chain ] keep ;

: matrix-size ( -- fixnum ) alpha-len fixed-keys - ; inline

: init-row ( -- row )
    {  } matrix-size [ default-layout-type suffix ] times ; inline

: init-matrix ( -- matrix )
    {  } matrix-size [ init-row suffix ] times ; inline
