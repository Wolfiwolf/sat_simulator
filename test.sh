#!/bin/bash

passed_tests=() 
failed_tests=() 

tests_dir=./tests
for filename in "$tests_dir"/*
do
    arrIN=(${filename//// })
    test_name=${arrIN[2]}     
    arrIN=(${test_name//./ })
    test_name=${arrIN[0]}     
    echo "### ${test_name^^} ###"
    python $filename
    
    if [ $? -eq 0 ]; then
        echo -e -n '\033[32m'
        echo "Test has passed!"
        echo -e -n '\033[m'
        passed_tests+=($test_name)
    else
        echo -e -n '\033[31m'
        echo "Test has failed!"
        echo -e -n '\033[m'
        failed_tests+=($test_name)
    fi
    echo ""
done

echo ""
echo "### Testing Results ###"
echo -e -n '\033[32m'
echo "Passed tests (${#passed_tests[*]})"
echo -e -n '\033[m'

echo -e -n '\033[31m'
echo "Failed tests (${#failed_tests[*]}):"
echo -e -n '\033[m'
for i in ${failed_tests[*]}
do
    echo $i
done
