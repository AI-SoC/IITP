set tics font "Titillium Web,16"

# margins
set rmargin 4
set lmargin 12
set bmargin 4

set key font "Titillium Web,16"
set label font "Titillium Web,16"

set title "Given data" font "Titillium Web,16"
set xlabel "Samples (n)" font "Titillium Web,16" offset 0,-0.5
set ylabel "Amplitude" font "Titillium Web,16" offset -3,0
plot "01_data20.txt" using 1:2 w l title ""

pause mouse any "Any key or button will terminate"
