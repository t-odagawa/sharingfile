import subprocess

filepath = './draw_and_save.sh'

f = open('../input/list.txt', 'r')

for line in f.readlines() :
    year, month, day, entry = line.split()
    print(year, month, day, entry)
#    subprocess.call([filepath + " " + str(year) + " " + str(month) + " " + str(day) + " " + str(entry)], shell=True)

f.close()
