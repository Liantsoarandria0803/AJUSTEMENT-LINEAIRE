set title 'Least Squares Regression'
set xlabel 'x'
set ylabel 'y'
plot 'data.dat' using 1:2 with points title 'Data Points', 10.6694 * x + -8.65895 title 'Fitted Line' with lines
pause -1
