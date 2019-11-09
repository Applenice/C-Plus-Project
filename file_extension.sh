#!/bin/bash
input_dir="$1"
[[ ! -d "${input_dir}" ]] && echo "input dir ${input_dir} don't exist!" && exit 255

file_ext=("html" "exe" "xml" "zip" "gif" "jpg" "doc" "jar" "apk" "dll" "pdf" "dex" "rar" "msi" "cab" "7z" "bz2")

for filename in `ls $input_dir`
do
	all_name=$input_dir"/"$filename
	if [ -f $all_name ];then
		echo $all_name

		if [[ "${file_ext[@]}" =~ "${all_name##*.}" ]];then
			echo "$all_name already have file extension"
		else
			result=$(file -b --uncompress $all_name)
			if echo $result | grep 'HTML document'
			then
				suffix=.html
				new_all_name=$all_name$suffix
				mv $all_name $new_all_name
			fi
		fi
	fi
done
