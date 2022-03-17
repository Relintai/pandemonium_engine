#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2019-2021 Péter Magyar
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
import os
import subprocess
import json
import shutil
import traceback

def process_folder(src):
    for item in os.listdir(src):
    
        sp = os.path.join(src, item)

        if os.path.isdir(sp):
            if item == ".git" or item == "bin" or item == "__pycache__" or item == ".import" or item == "logs" or item == "release" or item == "export" or item == "build" or item == "libs":
                continue

            process_folder(sp)
        else:
            if item.endswith(".h"):
                process_header(sp)


def process_header(path):
    original_file_data = ""

    try:
        with open(path, "r", encoding ='utf-8') as f:
            original_file_data = f.read()
    except:
        print("Error decoding file:")
        print(path)
        return
    
    file_data = original_file_data.lstrip()
    
    if file_data.startswith("#ifndef"):
        return

    file_data = file_data.replace("\r\n", "\n")

    lines = file_data.split("\n")

    guard1 = ""
    guard2 = ""
    found = False

    for l in lines:
        if guard1 == "":
            if l.lstrip().startswith("#ifndef"):
                guard1 = l
        else:
            if guard2 == "":
                if l.lstrip().startswith("#define"):
                    guard2 = l
                    found = True
                    break

    if not found:
        print("Error header guards not found in file:")
        print(path)
        return

    fdnog = original_file_data.replace(guard1, "", 1)
    fdnog = fdnog.replace(guard2, "", 1)

    with open(path, "w", encoding ='utf-8') as w:
        w.write(guard1)
        w.write("\n")
        w.write(guard2)
        w.write("\n")
        w.write(fdnog)

process_folder(os.path.abspath("."))

