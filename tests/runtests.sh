echo "Running unit tests:"

for i in src/*_tests
do
    if test -f $i
    then
        if $VALGRIND ./$i 2>> src/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's src/tests.log"
            echo "------"
            tail src/tests.log
            exit 1
        fi
    fi
done

echo ""