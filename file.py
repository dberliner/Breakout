import tkFileDialog
import os
# Delete the holder if it exists (it should not)
try:
    os.remove("~LoadFilePath")
except OSError:
	pass

# Get another file and write it in the holder
f = open("~LoadFilePath",'w')
f.write(tkFileDialog.askopenfilename(filetypes=[('Breakout Game File','*.bgm')])) # python will convert \n to os.linesep
f.close() # you can omit in most cases as the destructor will call if
