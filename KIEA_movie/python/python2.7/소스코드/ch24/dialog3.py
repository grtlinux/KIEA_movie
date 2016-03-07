#!/usr/loca/bin/python
#File : dialog3.pyw

from Tkinter import *
import string
import tkSimpleDialog

class MyDialog(tkSimpleDialog.Dialog):

    def body(self, master):

        Label(master, text="First:").grid(row=0)
        Label(master, text="Second:").grid(row=1)

        self.e1 = Entry(master)
        self.e2 = Entry(master)

        self.e1.grid(row=0, column=1)
        self.e2.grid(row=1, column=1)
        return self.e1 # initial focus

    def apply(self):
        first = string.atoi(self.e1.get())
        second = string.atoi(self.e2.get())
        self.result = first, second

root = Tk()
d = MyDialog(root, 'test window')
print d.result
