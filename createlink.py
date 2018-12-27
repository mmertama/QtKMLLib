import os
import sys

link = sys.argv[1]
dir = sys.argv[2]

if not os.path.exists(link):
    path = os.path.dirname(link)
    if not os.path.exists(path):
        os.makedirs(path)
    print ("creating " + dir + "-->" + link)
    os.symlink(dir, link)

    


