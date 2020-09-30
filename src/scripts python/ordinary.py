#!/usr/bin/python3

import sys
import re

class Card:
    seq_order = list(range(1, 13)) + [0]
    values = '3456789TJQKA2'
    suits = 'DCHS'
    def __init__(self, txt):
        v = txt[0]
        s = txt[1]
        self.value = Card.values.index(v)
        self.suit = Card.suits.index(s)
        self.index = self.suit * 13 + self.value
    def get_suit(self):
        return self.suit
    def get_value(self):
        return self.value
    def get_order(self):
        return Card.seq_order[self.get_value()]
    def __repr__(self):
        return '{}{}'.format(Card.values[self.get_value()], Card.suits[self.get_suit()])
    def __hash__(self):
        return self.index
    def __lt__(self, other):
        return self.value < other.value or (self.value == other.value and self.suit < other.suit)
    def __eq__(self, other):
        if other == None:
            return False
        return self.value == other.value and self.suit == other.suit
    def is_previous_in_seq(self, other):
        a = self.get_order()
        b = other.get_order()
        return a + 1 == b or (a == 12 and b == 0)
        
class Cards:
    def __init__(self, txt):
        if type(txt) == str:
            self.cards = {Card(c) for c in txt.split(' ')}
        if type(txt) == list:
            self.cards = set(txt)
    def __repr__(self):
        return str(self.cards)
    def __iter__(self):
        return (c for c in self.cards)
    def __len__(self):
        return len(self.cards)
    def __contains__(self, item):
        return item in self.cards
    def remove(self, other):
        self.cards = self.cards - other.cards

ultima = None
jogada = None
passaram = 0
mao = None
for l in sys.stdin:
    l = l.strip()
    if re.match(r'PASSOU', l):
        passaram += 1
    m = re.match(r'MAO\s+(.*)', l)
    if m:
        mao = sorted(Cards(m.group(1)))[::-1]
    m = re.match(r'JOGOU\s+(.*)', l)
    if m:
        ultima = sorted(Cards(m.group(1)))[0]
    if re.match(r'JOGADA', l):
        validas = mao
        if ultima != None and passaram < 3:
            validas = [c for c in mao if c > ultima]
        if len(validas) > 0:
            jogada = validas[-1]
            print(jogada)
        else:
            print('PASSO')
        passaram = 0
    if re.match(r'OK', l):
        mao.remove(jogada)
        jogada = None
    if re.match(r'NAO', l):
        jogada = None
