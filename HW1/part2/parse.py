import numpy as np
import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('no argument')
        sys.exit()

    fname=sys.argv[1]

    f = open(fname, 'r')
    lines = f.readlines()

    data_list = []
    info = ''
    for line in lines:
        if 'sec' in line:
            data = float(line.split('sec')[0])
            data_list.append(data)
            info = line.split('sec')[1].strip()

    data_list = np.asarray(data_list)
    print(info)
    print('total : {} times'.format(len(data_list)))
    print('mean = {} sec'.format(data_list.mean()))
    print('std = {} sec'.format(data_list.std()))