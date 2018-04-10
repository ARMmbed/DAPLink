for (( i = 0; i < 200 ; i ++))
do
    echo "loop $i"
    if python test/run_test.py --force --firmwaredir ../../../Shared/uvision_release/ --targetdir ../../../Shared; then
       continue
    else
       break
    fi
done
