! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: csv kernel locals sequences math math.functions math.parser math.order ranges generalizations accessors namespaces io.encodings.ascii io.files ;
USING: engender.settings ;
IN: engender.weights

TUPLE: weight-data
    { weights sequence read-only }
    { ng-len integer read-only } ;

SYMBOL: weight-data

! : : <weight-data> ( weights ng-len -- weight-data ) weight-data boa ; inline
C: <weight-data> weight-data

: weights ( -- 2darray ) weight-data get weights>> ;
: ng-len ( -- fixnum ) weight-data get ng-len>> ;

: digraph-score ( weights key1 key2 -- score )
    [ swap nth ] dip swap nth ; inline

: ng-len-cmp ( current new -- greatest )
    2dup < [ ceiling nip ] [ drop ] if ; inline
! could use ng-len-cmp instead of ceiling max if faster
: ng-len-update ( current-len ds-sum ds[k][l] -- new-len )
    / 2 + ceiling max ; inline
: ng-len-ds-sum ( current-len dsm weights dsin dsij n -- new-len )
    [ - ] dip roll nth -
    [ [ swap ng-len-update ] curry each ] curry each ; inline
: ng-len-eval1 ( current-len n weights dsi dsij -- new-len )
    [ swap dupd ] 2dip pick [ nth ] 2dip
    [ ng-len-ds-sum ] 4 ncurry each ; inline
: ng-len-eval2 ( current-len dsij dsi weights -- new-len )
    layout-size [0..b) 4spin [ ng-len-eval1 ] 3curry each ; inline
! ( ds[i][n] - ds[i][j] - ds[m][n] ) / ds[k][l] + 2
:: get-ng-len ( weights -- ng-len )
    3 weights [ dup [ weights ng-len-eval2 ] curry each ] each ; inline

: >numeric-2darray ( 2darray -- new2darray )
    [ [ string>number ] {  } map-as ] map ; inline

: >literal-2darray ( 2darray -- new2darray )
    [ [ number>string ] {  } map-as ] map ; inline

: read-weights-from-file ( path -- 2darray )
    ascii [ read-csv ] with-file-reader >numeric-2darray ; inline

: write-weights-to-file ( 2darray path -- )
    ascii [ >literal-2darray write-csv ] with-file-writer ; inline

: append-ng-len ( weight-data -- 2darray )
    [ weights>> ] [ ng-len>> ] bi { 0 } 1sequence
    ! layout-size 1 - [ 0 suffix ] times
    suffix ; inline

: load-weights ( -- weight-data )
    weights-file get read-weights-from-file dup get-ng-len <weight-data> ; inline

: load-weight-data ( -- weight-data )
    weight-data-file get read-weights-from-file [ but-last ] [ last first ] bi <weight-data> ; inline

: write-weights ( weight-data -- )
    weights>> weights-file get write-weights-to-file ; inline

: write-weight-data ( weight-data -- )
    append-ng-len weight-data-file get write-weights-to-file ; inline
