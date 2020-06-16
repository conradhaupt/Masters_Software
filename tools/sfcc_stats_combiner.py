from os.path import join, basename
import sys

if len(sys.argv) != 4:
    print('sfcc_stats_combiner.py <original_stats.csv> <new_stats.csv> <output_stats.csv>')
    exit(1)

arg_original = sys.argv[1]
arg_new = sys.argv[2]
arg_output = sys.argv[3]

f_original = open(arg_original, 'r')
f_new = open(arg_new, 'r')
f_output = open(arg_output, 'w')


# Original lines
lines_original = dict()
header = f_original.readline()
for line in f_original.readlines():
    args = line.split(',')
    key = ','.join(args[:4])
    value = ','.join(args[4:])
    lines_original[key] = value

# New lines
lines_new = dict()
_ = f_new.readline()
for line in f_new.readlines():
    args = line.split(',')
    key = ','.join(args[:4])
    value = ','.join(args[4:])
    lines_new[key] = value


# Combine
f_output.write(header)
replaced = dict()
for k, v in lines_original.items():
    if k in lines_new:
        new_val = lines_new.pop(k)
        f_output.write(k + ',' + new_val)
        replaced[k] = new_val
    else:
      if 'BWT' in k:
        print('Failed to replace BWT line [%s]' % (k))
      f_output.write(k + ',' + v)

for k, v in lines_new.items():
    print('Failed to replace [%s]' % (k))
    f_output.write(k + ',' + v)
f_output.close()
