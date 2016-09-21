# -*- coding: utf8 -*-


from curses import (
    noecho, cbreak,
    start_color, init_pair, color_pair, COLOR_GREEN, COLOR_BLACK,
    A_NORMAL
)
from random import randint
from threading import Thread
from sys import exc_info
from time import sleep


class daemon_curses(Thread):
    '''thread gérant l'affichage graphique avec curses
    NOTES :
        - scr.refresh() ne semble pas nécessaire, ptet que la méthode wrapper
            lancée dans le run.py l'inclue automatiquement ?

    '''

    def __init__(self, scr, core_ref):
        Thread.__init__(self)
        self.scr = scr
        self.core = core_ref
        self.core.logger.p_log('(CURSES) init')

    def scr_init(self):
        '''initialistion de ncurses

        '''
        noecho()
        cbreak()
        self.scr.keypad(1)

        Y, X = self.scr.getmaxyx()
        self.X, self.Y = X-2, Y-2-1
        self.scr.clear()

        # Draw a border around the board
        border_line = '+'+(self.X*'-')+'+'
        self.scr.addstr(0, 0, border_line)
        self.scr.addstr(self.Y+1, 0, border_line)
        for y in range(0, self.Y):
            self.scr.addstr(1+y, 0, '|')
            self.scr.addstr(1+y, self.X+1, '|')
        self.scr.refresh()

        # defines ncurses colors
        start_color()
        init_pair(1, COLOR_GREEN, COLOR_BLACK)
        self.scr.addstr(2, 1, "touche <echap> pour quitter !")

    def run(self):
        while 1:
            sleep(0.1)
            try:
                self.scr.addstr(4, 1, "derniere touche entree au clavier : ")
                self.scr.addstr(
                    4,
                    37,
                    str(self.core.last_entry) + (5 - len(str(self.core.last_entry))) * " ",
                    A_NORMAL
                )

                self.scr.addstr(6, 1, "donnees arduino : ")
                capteur1 = int(self.core.d_arduino.data['capteur1var'])
                self.scr.addstr(6, 19,
                    str(capteur1) + " "*(3-len(str(capteur1)))
                )

                self.scr.addstr(7, 10, "volume : ")
                self.scr.addstr(7, 19, str(
                        self.core.d_audio.volume
                    )
                )

                self.scr.addstr(9, 1, "ambiance actuelle (python) : ")
                self.scr.addstr(9, 30, str(self.core.d_audio.current))
                self.scr.addstr(10, 1, "(changez avec les touches \"a\" pour la scene0, \"z\" pour la scene1, \"e\" pour la scene2, etc)")

                self.scr.addstr(12, 1, "ambiance actuelle (arduino) : ")
                self.scr.addstr(12, 31, str(self.core.d_arduino.data['ambiancevar']))

                # self.scr.addstr(9, 29,str(self.core.d_arduino.dicoarduino['capteur1']))


            except:
                self.core.logger.p_log('(GRAPHIC) SCR_ERROR', error=exc_info())

