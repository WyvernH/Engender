! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: kernel sequences random math ranges ;
USING: multiline ;
USING: engender.analyse engender.settings ;
IN: engender.generate.defns


! not all needed
: ordered-intersect ( elt-seq seq -- newseq ) [ [ = ] curry any? ] with filter ; inline
: index-intersect ( elt-seq seq -- newseq ) [ index ] curry map ; inline
: ordered-index-intersect ( elt-seq seq -- newseq ) [ ordered-intersect ] keep index-intersect ; inline


! use 2each!

: random-element ( seq -- elt ) [ length random ] keep nth ;

! note: destructive sequence operation
: force-randomize ( seq -- 'seq )
    dup dup length 1 - [0..b) [ 2dup tail [ dup reach nth ] dip remove random-element pick index pick exchange ] each drop ;

: working-set-slice ( seq -- slice ) fixed-keys tail-slice ; inline

! default-evolve options
! all evolve words can be used on both layouts and key-arrays
: evolve ( seq degree -- )
    dupd sample dup pick ordered-index-intersect
    [ nth pick set-nth ] curry each-index drop ; inline
/* alt. def. See if faster:
: evolve (seq degree --)
    dupd sample dup pick index-intersect randomize [ nth pick set-nth ] curry each-index drop ;
*/
! like evolve, except algorithm is forced to use all letters in the repermutation
: force-evolve ( seq degree -- )
    dupd sample dup pick index-intersect force-randomize [ nth pick set-nth ] curry each-index drop ; inline
: evolve-working-set ( seq degree -- ) [ working-set-slice ] dip evolve ; inline
: force-evolve-working-set ( seq degree -- ) [ working-set-slice ] dip force-evolve ; inline

! for user analysis and manual generation of layouts
: evolve-new ( seq degree -- newseq ) [ clone dup ] dip evolve ; inline
: force-evolve-new ( seq degree -- newseq ) [ clone dup ] dip force-evolve ; inline


! default-layout options
: new-alphabet ( -- layout ) alphabet clone ; inline
: random-layout ( -- layout ) alphabet clone dup fixed-keys tail-slice randomize drop clone ; inline
! default-key-array options
: random-key-array ( -- key-array ) random-layout layout>key-array ; inline
: alpha-key-array ( -- key-array ) alphabet layout>key-array ; inline
