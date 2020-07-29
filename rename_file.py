import sys
import os
import re

string_to_delete = ",v"
root = "C:\\"
path = os.path.join(root, "subdir")


for path, files in os.walk(root):
    for name in files:
        if(string_to_delete in name):
            try:
                newname = name[:-2]
                os.rename(os.path.join(path, name),
                          os.path.join(path, newname))
            except FileExistsError as f_err:
                newname = name[:-2]
                newname = newname + "1"
                os.rename(os.path.join(path, name),
                          os.path.join(path, newname))
