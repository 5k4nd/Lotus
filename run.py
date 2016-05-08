#!/usr/bin/python2
# -*- coding: utf8 -*-

"""
Le Lotus: alpha version.
Licence GNU GPL v3.
    Bapt Abl, 2016.
"""

from core import core
from curses import wrapper

def launch(scr):
    core(
        SCR=scr,
    ).start()

wrapper(launch)  # encapsulation pour curses

