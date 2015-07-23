import os, sys

def tail( f, window=1 ):
    BUFSIZ = 1024
    f.seek(0, 2)
    bytes = f.tell()
    #print 'bytes = %i'%bytes #debug
    size = window
    block = -1
    data = []
    while size > 0 and bytes > 0:
        if (bytes - BUFSIZ > 0):
            # Seek back one whole BUFSIZ
            f.seek(block*BUFSIZ, 2)
            # read BUFFER
            data.append(f.read(BUFSIZ))
        else:
            # file too small, start from begining
            f.seek(0,0)
            # only read what was not read
            data.append(f.read(bytes))
        linesFound = data[-1].count('\n')
        size -= linesFound
        bytes -= BUFSIZ
        block -= 1
    #print 'data = %s'%data #debug
    #return '\n'.join(''.join(data).splitlines()[-window:])
    return data

dir = sys.argv[1]

#folders = [x for x in os.walk('./'+dir).next()[1]]

#for folder in folders:
#    print folder
files = [x for x in os.listdir(sys.argv[1]) if '.stderr' in x]
#print os.listdir(sys.argv[1])
count_total = 0
count_failed = 0
for file in files:
        count_total += 1
        #print file
        current = open(sys.argv[1] + '/'+file,'r')
        #if 'All cuts' not in tail(current):
        if (tail(current)!=[]):
                count_failed +=1
                print '\t'+file
        current.close()

print "Scanning ",dir,"--> Failure Rate = ", count_failed/float(count_total)
