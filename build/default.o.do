# vim: ft=sh

redo-ifchange ../config.rc
. ../config.rc

# Custom config
if [ -f "../${1%/*}/config.rc" ]; then
    redo-ifchange "../${1%/*}/config.rc"
    . "../${1%/*}/config.rc"
fi

redo-ifchange "../${2}.c"

"${CC:-clang}" -c "../${2}.c" ${CFLAGS} -MD -MF "${2}.d" -o "${3}"

read DEPS <"${2}.d"
redo-ifchange ${DEPS#*:}
