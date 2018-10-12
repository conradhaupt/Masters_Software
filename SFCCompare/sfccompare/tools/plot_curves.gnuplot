# Load file
# print filename;
filename = ARG1
if (!exists("filename")) print "Couldn\'t find the file, defaulting to morton.coords"; filename = 'morton.coords'
# Get the max values for each dimension
stats filename using 1;
max_1 = STATS_max + 1
stats filename using 2;
max_2 = STATS_max + 1

# Set the ranges based off of the maximum values
set xrange[-1:max_1]
set yrange[max_2:-1] reverse

# Set labels
set title "Morton order"
set xlabel "x"
set ylabel "y"

# Define a grid with separation 1
set key off
set size square
set grid ytics xtics
set ytics 1
set xtics 1
set grid

# Plot
set samples 1024
plot filename using 1:2 with lines
# , filename using 1:2:($0+1) with labels