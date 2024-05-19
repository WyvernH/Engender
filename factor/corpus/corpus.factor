! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: engender.weights engender.settings ;
USING: csv math math.functions sequences accessors io io.files io.encodings.utf8 io.encodings.ascii kernel strings words.constant generalizations namespaces ;
! debug:
USING: prettyprint ;
IN: engender.corpus

SYMBOLS: corpus corpus-data ;

: alphaindex ( char -- idx ) alphabet index ; inline

: ng>idx ( str -- idx )
    0 swap [ alphaindex swap alpha-len * + ] each ; inline

: ng-next-digit-quotation ( seq num -- seq num quot )
    [ alpha-len mod ] [ [ swap - alpha-len / ] curry ] bi ; inline

: append-ng-char ( str num -- newstr newnum )
    ng-next-digit-quotation [ alphabet nth prefix ] swap bi ; inline

: append-ng-idx ( seq num -- newseq newnum )
    ng-next-digit-quotation [ prefix ] swap bi ; inline

! note: if the given index is too high for the given ng-len, will output last ng-len substring of full ngram
: idx>ng ( ng-len idx -- str )
    "" spin [ append-ng-char ] times drop ; inline

! outputs array of alphabet indices instead of array of char
: idx>idx-ng ( ng-len idx -- seq )
    {  } spin [ append-ng-idx ] times drop ; inline

: ng-entry ( ng-idx -- ng-entry ) 1 {  } 2sequence ; inline

: ngram-hist ( seq ng-entry -- idx )
    [ [ first ] bi@ = ] curry find drop ; inline

: inc-ngram ( seq idx -- newseq )
    over [ 1 over [ 1 + ] change-nth ] change-nth ; inline

: add-ngram ( seq ng-idx -- newseq )
    ng-entry
    2dup ngram-hist
    dup [ nip inc-ngram ] [ drop suffix ] if ; inline

: valid-letter? ( char -- char ? ) dup alphabet member? ; inline

: register-ngram ( seq ngram -- newseq ngram ) dup [ ng>idx add-ngram ] dip ; inline

: stream-corpus ( seq ngram -- newseq )
    rest read1 valid-letter? [ [ suffix register-ngram rest read1 valid-letter? ] loop ] [  ] if
    2drop ; inline

! exits at first non-alphabetic character in corpus file
: read-corpus ( ng-len seq path -- newseq )
    ascii 4 nrot [ alphabet first <string> stream-corpus ] curry with-file-reader ; inline

: load-corpus ( ng-len -- seq ) {  } corpus-file get read-corpus ; inline

: load-corpus-data ( -- seq ) corpus-data-file get ascii [ read-csv ] with-file-reader >numeric-2darray ; inline

: write-corpus-data ( seq -- ) corpus-data-file get ascii [ >literal-2darray write-csv ] with-file-writer ; inline

: >ng-corpus ( corpus-data ng-len -- ng-corpus )
    '[ clone _ over first idx>idx-ng over set-first ] map ; inline
