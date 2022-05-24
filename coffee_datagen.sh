echo "$(cat coffee.csv | grep --invert-match $(cat coffee.csv|head -n1)|awk 'BEGIN { FS = "," } ; {printf $((2+3))/2","}';echo)" > test.csv
