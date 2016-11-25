set terminal png 
set size 1,1
set output 'matrix.png'
set xlabel "matrix row length"
set ylabel "time"
set title "Time of matrix multiplication"
set grid
set style line 1
plot "block.txt" using 1:2 title "Blocked", \
     "transp.txt" using 1:2 title "Transp", \
     "simple.txt"  using 1:2 title "Ordinary"