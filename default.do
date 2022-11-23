# vim: ft=sh

redo-ifchange ./config.rc
. ./config.rc

# Custom config
[ -f "${1%/*}/config.rc" ] && \
    . "${1%/*}/config.rc"

# Object files
OBJS="${OBJS:-}"

OBJS_BUILD=""
for obj in ${OBJS}; do
    OBJS_BUILD+="./build/${obj} "
done

redo-ifchange "./build/${2}.o"
redo-ifchange ${OBJS_BUILD}

"${CC:-clang}" ${CFLAGS} ${LIBS} "./build/${2}.o" ${OBJS_BUILD} -o "${3}"
