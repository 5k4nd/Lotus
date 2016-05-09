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
        # self.d_data = d_data_ref
        self.core.logger.p_log('(CURSES) init')

    def scr_init(self):
        '''setup the curses (in fact: current terminal) screen at the firt
            launch

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

    def run(self):
        while 1:
            sleep(0.1)
            try:
                # on affiche la dernière touche entrée.
                try:
                    self.scr.addstr(2, 1, "press <echap> to quit")
                    self.scr.addstr(4, 1, "derniere touche entree au clavier : ")
                    printable_last_entry = str(self.core.last_entry)\
                        + (5 - len(str(self.core.last_entry))) * " "
                    self.scr.addstr(4, 37, printable_last_entry, A_NORMAL)
                    self.scr.addstr(6, 1, "donnees arduino : ")
                except:
                    self.core.logger.p_log('(GRAPHIC) SCR_ERROR', error=exc_info())
                try:
                    capteur1 = int(self.core.d_arduino.data['capteur1'])
                    self.scr.addstr(6, 19,
                        # petit hack pour afficher un blanc sur le 3e caractère si < 100 i.e. trois caractères
                        str(capteur1) + " "*(3-len(str(capteur1)))
                    )
                    self.scr.addstr(7, 10, "volume : ")
                    self.scr.addstr(7, 19,str(
                            1 - ( capteur1 / 120.0 )
                        )
                    )
                    self.scr.addstr(13, 1, self.core.erreurs)
                except:
                    self.core.logger.p_log('(GRAPHIC) SCR_ERROR', error=exc_info())


            except:
                self.core.erreurs = "> curses_error" + str(exc_info())

    def dataprint(self, data_type, data_content):
        if data_type == 'key':
            self.scr.addstr(2, 2, str(data_content))
