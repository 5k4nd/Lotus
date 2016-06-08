#!env/bin/python2
# -*- coding: utf8 -*-

"""
Le Lotus: alpha version.
Licence GNU GPL v3.
    Bapt Abl, 2016.

"""
from core import core
from config_lotus import *


# fonction principale du programme
# n'oubliez pas de définir vos ports arduino !
def launch(scr):
    core(
        arduino_port=ARDUINO_PORT,
        arduino_speed=ARDUINO_SPEED,
        arduino_fake=simulate_arduino,
        SCR=scr,
        ncurses=run_with_curses
    ).start()


if run_with_curses:
    from curses import wrapper
    wrapper(launch)  # encapsulation pour curses
else:
    launch(False)
