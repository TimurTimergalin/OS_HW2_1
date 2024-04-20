from os.path import join
from os import listdir
from sys import argv


if __name__ == "__main__":
    entries = []
    dir_path = argv[1]
    
    for filename in listdir(dir_path):
        with open(join(dir_path, filename)) as f:
            for line in f:
                t, mes = line.split(None, 1)
                t = int(t)
                entries.append((t, mes))
    
    entries.sort()
    
    with open(join(dir_path, "merged.txt"), "w") as f:
        for _, mes in entries:
            f.write(mes)
