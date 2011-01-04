#!/bin/bash
INPUT_FILE=$1
TEMP_FILE1=${INPUT_FILE}1.tmp
TEMP_FILE2=${INPUT_FILE}2.tmp
TEMP_FILE3=${INPUT_FILE}3.tmp
ERROR_FILE=$2
OUTPUT_FILE=$3
HTML_OUTPUT_FILE=$3.html
TEST_RESULTS=$4

flattenGcovOutput() {
while read line1
do
  read line2
  echo $line2 " " $line1 
  read junk
  read junk
done < ${INPUT_FILE}
}

getRidOfCruft() {
sed '-e s/^Lines.*://g' \
    '-e s/^[0-9]\./  &/g' \
    '-e s/^[0-9][0-9]\./ &/g' \
    '-e s/of.*File/ /g' \
    "-e s/'//g" \
    '-e s/^.*\/usr\/.*$//g' \
    '-e s/^.*\.$//g' 
}

getFileNameRootFromErrorFile() {
sed '-e s/gc..:cannot open .* file//g' ${ERROR_FILE}
}

writeEachNoTestCoverageFile() {
while read line
do
  echo "  0.00%  " ${line}
done 
}

createHtmlOutput() {
    echo "<table border="2" cellspacing="5" cellpadding="5">"
    echo "<tr><th>Coverage</th><th>File</th></tr>"
    sed "-e s/.*%   /<tr><td>&<\/td><td>/" \
        "-e s/[a-zA-Z0-9_]*\.[ch][a-z]*/<a href='file:\.\/&.gcov'>&<\/a><\/td><\/tr>/" 
    echo "</table>"
    sed "-e s/.*/&<br>/g" < ${TEST_RESULTS}
}


flattenGcovOutput | getRidOfCruft  > ${TEMP_FILE1}
getFileNameRootFromErrorFile | writeEachNoTestCoverageFile > ${TEMP_FILE2}
cat ${TEMP_FILE1}  ${TEMP_FILE2} | sort | uniq > ${OUTPUT_FILE}
createHtmlOutput < ${OUTPUT_FILE} > ${HTML_OUTPUT_FILE}
rm -f ${TEMP_FILE1} ${TEMP_FILE2} 
erage</th><th>File</th></tr>"
    sed "-e s/.*%   /<tr><td>&<\/td><td>/" \
        "-e s/[a-zA-Z0-9_]*\.[ch][a-z]*/<a href='file:\.\/&.gcov'>&<\/a><\/td><\/tr>/" 
    echo "</table>"
    sed "-e s/.*/&<br>/g" < ${TEST_RESULTS