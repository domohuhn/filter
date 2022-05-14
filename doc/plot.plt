if (!exists("filename")) filename='butterworth_lowpass.txt'
outfile=sprintf("%s.png",filename)
set output outfile
set term 'png' size 1280,960
set multiplot layout 2,2 rowsfirst
set grid
set xlabel 'Frequency f/fsample'
set ylabel 'Gain'
set title 'Frequency response'
plot filename u 6:7 w lp ti "Gain" lt rgb 'red'


set xlabel 'Frequency f/fsample'
set ylabel 'Phase shift (degree)'
set title 'Phase shift'
plot filename u 6:8 w lp ti "Phase shift" lt rgb 'red'

set title 'Impulse response'
set xlabel 'Filter cycle'
set ylabel 'Output'
plot filename u 1:2 w lp ti "Input" lt rgb 'green', filename u 1:3 w lp ti "Output" lt rgb 'red'

set title 'Step response'
set xlabel 'Filter cycle'
set ylabel 'Output'
plot filename u 1:4 w lp ti "Input" lt rgb 'green', filename u 1:5 w lp ti "Output" lt rgb 'red'
unset multiplot
unset output
