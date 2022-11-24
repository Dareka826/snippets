TESTS="arena"

TESTS_SUCCEDEED=1
for t in ${TESTS}; do
    redo-ifchange "${t}/${t}.test"

    printf "\033[33m[TEST]:\033[0m Running tests for %s\n" "${t}" >&2
    if "./${t}/${t}.test" >&2; then
        printf "\033[32m[TEST]:\033[0m %s passed.\n" "${t}" >&2
    else
        printf "\033[31m[TEST]: %s failed!\033[0m\n" "${t}" >&2
        TESTS_SUCCEDEED=0
    fi
done

[ "${TESTS_SUCCEDEED}" = "1" ] \
    && exit 0 \
    || exit 1
