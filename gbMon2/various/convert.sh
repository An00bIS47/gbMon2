#!/bin/bash
##
##  convert.sh
##  gbmon2
##
##  Created by michael on 19.10.13.
##  Copyright (c) 2013 michael. All rights reserved.
##
##	Original script:
##
##  pngtopnm $1 > tmp
##  pnmtopgm tmp | pgmtopbm | pbmtoxbm > ${1%%.*}.h
##
##  sudo apt-get install netbpm
##  sudo apt-get install ivtools-bin

today="$(date +'%d.%m.%Y')"
mkdir "bitmaps"

for file in *.png; do
	echo "Processing $file"
	pngtopnm $file > tmp
	pnmtopgm tmp | pgmtopbm | pbmtoxbm > ${file%%.*}.h

	# Add text to File
	sed -i '1s:^://\n:' ${1%%.*}.h
	sed -i "2s:^://  ${file%%.*}.h\n:" ${file%%.*}.h
	sed -i "3s:^://  gbmon2\n:" ${file%%.*}.h
	sed -i "4s:^://\n:" ${file%%.*}.h
	sed -i "5s:^://  Created by michael on $today.\n:" ${file%%.*}.h
	sed -i "6s:^://  Copyright (c) 2013 michael. All rights reserved.\n\n:" ${file%%.*}.h

	# Replace define statement
	sed -i -e 's:#define://#define:g' ${file%%.*}.h

	# Replace declaration and variable name
	sed -i -e "s:static\schar\snoname_bits:const uint8_t ${file%%.*}:" ${file%%.*}.h

	# Add to statusHeader.h
	str=$(echo "$a" | sed -n 9,10p ${file%%.*}.h)
	echo $str >> bitmaps/bitmapStatus.h

	# Delete Tempfile
	rm tmp
	mv ${file%%.*}.h bitmaps
done





