! Copyright (C) 2024 Matthew Hinton.
! See https://factorcode.org/license.txt for BSD license.
USING: engender.modules ;
USING: kernel strings ;
IN: engender.settings

! must initialise layouts-directory with '/' at end
SYMBOLS: weights-file weight-data-file corpus-file corpus-data-file layouts-directory globals ;

CONSTANT: default-weights-file "/home/matthew/repos/factor/work/engender/weights/weights.csv"
CONSTANT: default-weight-data-file "/home/matthew/repos/factor/work/engender/weights/weight-data.csv"

CONSTANT: default-corpus-file "/home/matthew/repos/factor/work/engender/corpus/corpus.txt"
CONSTANT: default-corpus-data-file "/home/matthew/repos/factor/work/engender/corpus/corpus-data.csv"

CONSTANT: default-layouts-directory "/home/matthew/repos/factor/work/engender/layouts/"

CONSTANT: alphabet " abcdefghijklmnopqrstuvwxyz,./'"
CONSTANT: alpha-len 31
CONSTANT: layout-size 31
CONSTANT: fixed-keys 1

UNION: strbool string boolean ;

TUPLE: global-options
    { h boolean initial: f }
    { c strbool initial: f }
    { C strbool initial: f }
    { w strbool initial: f }
    { W strbool initial: f }
    { d boolean initial: f }
    { l strbool initial: f } ;
TUPLE: module-options
    { h boolean initial: f }
    { s boolean initial: f } ;


CONSTANT: modules {
    { "a" [ analyse-module ] }
    { "analyse" [ analyse-module ] }
    { "g" [ generate-module ] }
    { "generate" [ generate-module ] }
    { "r" [ rank-module ] }
    { "rank" [ rank-module ] }
    { "c" [ corpus-module ] }
    { "corpus" [ corpus-module ] }
    { "w" [ weights-module ] }
    { "weights" [ weights-module ] }
    [ unknown-module ]
}

CONSTANT: modules-options {
    { "a" { "-h" "--help" "-s" "--score" } }
    { "g" { "-h" "--help" } }
    { "r" { "-h" "--help" } }
    { "c" { "-h" "--help" } }
    { "w" { "-h" "--help" } }
}

CONSTANT: global-help {
    "Usage: engender [global-options] module-name [module-options]\n"
    "global options"
    " -h, --help                   display this help and exit"
    "     --corpus-file=FILE       use FILE as corpus text file"
    "     --corpus-data-file=FILE  use FILE as corpus data file"
    "     --weights-file=FILE      use FILE as weights file"
    "     --weight-data-file=FILE  use FILE as weight data file"
    " -d, --dynamic                use dynamic keys for all layouts"
    "     --layout-dir=DIR         use DIR for layout directory"
}
CONSTANT: modules-help {
    "modules"
    " (a)nalyse                    analyse layout and print statistics"
    " (g)enerate                   generate an optimal layout"
    " (r)ank                       rank all layouts in layout directory"
    " (c)orpus                     update or display corpus data"
    " (w)eights                    generate, update or display weight data"
}
CONSTANT: analyse-help {
    "Usage: analyse [options] layout-name\n"
    "options"
    " -h, --help                   display this help message"
    " -s --score                   only display score (default for now)"
}
CONSTANT: generate-help {
    "Usage: generate [options]\n"
    "options"
    " -h, --help                   display this help message"
}
CONSTANT: rank-help {
    "Usage: rank [options]\n"
    "options"
    " -h, --help                   display this help message"
}
CONSTANT: corpus-help {
    "Usage: corpus [options]\n"
    "options"
    " -h, --help                   display this help message"
}
CONSTANT: weights-help {
    "Usage: weights [options]\n"
    "options"
    " -h, --help                   display this help message"
}
