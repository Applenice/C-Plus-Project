#! /bin/bash
input_file="$1"
[[ ! -f "${input_file}" ]] && echo "input file ${input_file} don't exist!" && exit 255

for ip in `cat $input_file`
do
    echo $ip
    ping_result=`ping -c 1 $ip | grep loss | awk '{print $6}'| awk -F "%" '{print $1}'`
    if [ $ping_result -eq 100 ];then 
        echo $ip is down >> result.txt
    else
        echo $ip is up >> result.txt
    fi
done
