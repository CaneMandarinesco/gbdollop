import sys
registers = ['b', 'c', 'd', 'e', 'h', 'l', 'a']

if len(sys.argv) != 2:
    print('no argument!')
    exit()

f = open(sys.argv[1], 'w')

for ro in registers:
    for rs in registers:
        f.write(f"LD_X_Y({ro}, {rs}) ")
    f.write("\n")