set title 'Comparison of Two Lines'
set xlabel 'X'
set ylabel 'Y'
set grid
plot 'data.dat' using 1:2 with points title 'Data Points', \
     1 * x + 1 title 'First Line' with lines, \
8.79438* x +-3.26146 title '500 Line' with lines, \
9.50588* x +-5.6135 title '1000 Line' with lines, \
9.81525* x +-6.6362 title '1500 Line' with lines, \
9.94977* x +-7.08087 title '2000 Line' with lines, \
10.0083* x +-7.27422 title '2500 Line' with lines, \
10.0337* x +-7.35829 title '3000 Line' with lines, \
     10.0447 * x - 7.3948 title 'Fitted Line' with lines
pause -1
