#!/usr/bin/python3

from enum import Enum
import itertools
import unittest
import random
import subprocess
import pty
import os
import sys
import re

from asyncproc import with_timeout

class InvalidCombination(Exception):
    def __init__(self, value):
        self.value = value
    def __repr__(self):
        return repr(self.value)
    
class Combinations(Enum):
    one = 1
    two = 2
    three = 3
    straight = 4
    flush = 5
    full_house = 6
    four_of_a_kind = 7
    straight_flush = 8
    def __lt__(self, other):
        return self.value < other.value
    
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
        else:
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

class Game:
    def __init__(self):
        deck = random.sample([Card('{}{}'.format(v, s)) for v in Card.values for s in Card.suits], 52)
        self.hands = [Cards(deck[13 * i : 13 * (i + 1)]) for i in range(4)]
        self.last_player = None
        self.last_combo = None
    def __repr__(self):
        return 'Last player: {}\nCombo: {}\n{}'.format(self.last_player, self.last_combo, '\n'.join([repr(sorted(h)) for h in self.hands]))
    def has_cards(self, player, hand):
        for h in Cards(hand):
            if h not in self.hands[player]:
                return False
        return True
    def can_play(self, player, hand):
        try:
            if not self.has_cards(player, hand):
                return False
            combination(Cards(hand))
            if self.last_player == None:
                return Card('3D') in Cards(hand)
            if self.last_player == player:
                return True
            return beats(Cards(hand), self.last_combo)
        except InvalidCombination:
            return False
    def play(self, player, hand):
        if self.can_play(player, hand):
            self.last_player = player
            self.last_combo = Cards(hand)
            self.hands[player].remove(Cards(hand))
            return True
        return False
    def getScore(self):
        def score(tam):
            if tam < 10:
                return tam
            else:
                if tam < 13:
                    return 2 * tam
                else:
                    return 39
        sizes = [len(h) for h in self.hands]
        if min(sizes) == 0:
            return [score(s) for s in sizes]
        else:
            return None
        

class GameHost:
    def __init__(self, programs, log_level = 1):
        self.game = Game()
        self.order = 0
        self.turn = 0
        self.processes = []
        self.inputs = []
        self.outputs = []
        self.log_level = log_level
        for p in programs:
            master, slave = pty.openpty()
            proc = subprocess.Popen(p, stdout = slave, stdin = subprocess.PIPE)
            self.processes.append(proc)
            self.inputs.append(proc.stdin)
            self.outputs.append(os.fdopen(master))
        for p in range(4):
            self.tell(p, 'MAO {}'.format(' '.join([str(x) for x in self.game.hands[p]])))
    def __repr__(self):
        return 'Next Player: {}\n{}'.format(self.order, repr(self.game))
    def next(self):
        self.order = (self.order + 1) % 4
        self.turn += 1
    def Play(self, comb):
        if self.game.play(self.order, comb):
            self.tellPlayers('play', Cards(comb))
            ret = True
        else:
            self.tellPlayers('pass', 'no')
            ret = False
        self.next()
        return ret
    def Pass(self):
        self.tellPlayers('pass', 'ok')
        self.next()
    def tellPlayers(self, action, comb):
        for p in range(4):
            if p == self.order:
                if action == 'play':
                    self.tell(p, 'OK')
                else:
                    if comb == 'no':
                        self.tell(p, 'NAO')
            else:
                if action == 'play':
                    msg = 'JOGOU {}'.format(' '.join([str(x) for x in comb]))
                else:
                    msg = 'PASSOU'
                self.tell(p, msg)
    def tell(self, player, msg):
        try:
            self.inputs[player].write(bytes(msg + "\n", 'ascii'))
            self.inputs[player].flush()
        except Exception:
            pass
    def log(self, lvl, msg):
        if lvl < self.log_level:
            print(msg)
    def cycle(self, timeout = 1):
        going = True
        while going:
            passaram = 0
            for n, p in enumerate(self.outputs):
                score = self.game.getScore() 
                if score == None:
                    self.log(1, self)
                    self.tell(n, 'JOGADA')
                    try:
                        cmd = with_timeout(timeout, p.readline)
                    except Exception:
                        self.outputs[n] = None
                        cmd = 'PASSO'
                    self.log(3, '{} disse "{}"'.format(n, cmd.rstrip()))
                    if re.match(r'\s*PASSO\s*', cmd):
                        self.log(2, '{} vai passar'.format(n))
                        self.Pass()
                        passaram += 1
                    else:
                        m = re.match(r'\s*([2-9TJQKA][DCHS](\s+[2-9TJQKA][DCHS])*)\s*', cmd)
                        if m:
                            play = m.group(1)
                            if self.Play(play):
                                self.log(2, '{} vai jogar "{}"'.format(n, play))
                            else:
                                self.log(2, '{} nao pode jogar {} e por isso vai passar'.format(n, cmd.rstrip()))
                                passaram += 1
                        else:
                            self.log(2, '{} vai passar'.format(n))
                            self.Pass()
                            passaram += 1
                else:
                    going = False
            if passaram == 4:
                self.log(0, 'Passaram todos!')
                going = False
        if score != None:
            for n, s in enumerate(score):
                g.tell(n , 'ACABOU {}'.format(s))
                self.log(0, 'Jogador {}: {}'.format(n + 1, s))
        for p in self.processes:
            p.kill()
        
def text_to_tex(text):
    return '\\ '.join([carta_to_tex(text_to_carta(c)) for c in text.split(' ')])
    
def carta_to_tex(card):
    suits = ['\spades', '\hearts', '\clubs', '\diamonds']
    values = list(range(3, 11)) + list('JQKA2')
    return '{} {}'.format(values[get_value(card)], suits[get_suit(card)])

def split_suits(cards):
    suits = {}
    for c in cards:
        suits[c.get_suit()] = suits.get(c.get_suit(), []) + [c]
    return suits

def split_values(cards):
    values = {}
    for c in cards:
        values[c.get_value()] = values.get(c.get_value(), []) + [c]
    return values

def is_sequence(cards):
    cards = list(cards)
    for a, b in zip(cards[:-1], cards[1:]):
        if not b.is_previous_in_seq(a):
            return False
    return True

def is_flush(cards):    
    return len({c.get_suit() for c in cards}) == 1

def is_straight(cards):
    if straight_order(cards):
        return True
    return False

def straight_order(cards):
    cards = sorted(cards, key = lambda c: -c.get_order())
    if is_sequence(cards):
        return cards
    other = cards[1:] + [cards[0]]
    if is_sequence(other):
        return other
    
def five_card_combo(cards):
    if len(cards) != 5:
        return None
    is_f = is_flush(cards)
    is_s = is_straight(cards)
    if is_s and is_f:
        return Combinations.straight_flush
    if is_s:
        return Combinations.straight
    if is_f:
        return Combinations.flush
    combos = [len(x) for x in split_values(cards).values()]
    if len(combos) == 2:
        combos = sorted(combos)
        if combos == [2, 3]:
            return Combinations.full_house
        if combos == [1, 4]:
            return Combinations.four_of_a_kind
    raise InvalidCombination(cards)

        
def combination(cards):
    L = len(cards)
    if L == 1:
        return Combinations.one
    if L == 2 and len(split_values(cards)) == 1:
        return Combinations.two
    if L == 3 and len(split_values(cards)) == 1:
        return Combinations.three
    if L == 5:
        return five_card_combo(cards)
    raise InvalidCombination(cards)

def beats(c1, c2):
    if len(c1) != len(c2):
        return False
    C1 = combination(c1)
    C2 = combination(c2)
    if C1 != C2:
        return C1 > C2
    if C1 < Combinations.straight:
        MC1 = max(c1, key = lambda c: c.get_suit())
        MC2 = max(c2, key = lambda c: c.get_suit())
        return MC1 > MC2
    if C1 == Combinations.straight or C1 == Combinations.straight_flush:
        MC1 = straight_order(c1)
        MC2 = straight_order(c2)
        return MC1[0] > MC2[0]
    if C1 == Combinations.flush:
        suit1 = list(c1)[0].get_suit()
        suit2 = list(c2)[0].get_suit()
        if suit1 != suit2:
            return suit1 > suit2
        MC1 = max(c1, key = lambda c: c.get_value())
        MC2 = max(c2, key = lambda c: c.get_value())
        return MC1 > MC2
    if C1 == Combinations.full_house or C1 == Combinations.four_of_a_kind:
        v1 = split_values(c1)
        v2 = split_values(c2)
        lv1 = max(v1, key = lambda c: len(v1[c]))
        lv2 = max(v2, key = lambda c: len(v2[c]))
        return lv1 > lv2
          
def combinations(cards):
    cards = sorted(cards, key = lambda c: c.get_order())
    comb = []
    straights = []
    flushes = []
    straight_flushes = []
    suits = split_suits(cards)
    values = split_values(cards)
    print(suits)
    print(values)
    for s in suits.values():
        if len(s) >= 5:
            possible_straights = values[11] + s # Add the aces to the beginning
            for f in itertools.combinations(possible_straights, 5):
                if is_sequence(f):
                    straight_flushes.append(f)
                else:
                    flushes.append(f)
    return (flushes, straight_flushes)
            

class Test(unittest.TestCase):
    def setUp(self):
        self.tst_combs = [
            ('3S', Combinations.one),
            ('3S 3C', Combinations.two),
            ('2S 2C', Combinations.two),
            ('2S 2C 2H', Combinations.three),
            ('AS 5S 4S 2C 3S', Combinations.straight),
            ('AH QS KS TS JS', Combinations.straight),
            ('AS 5S 4S 2C 3S', Combinations.straight),
            ('6S 5S 4S 2C 3S', Combinations.straight),
            ('QS 5S 4S 2S 3S', Combinations.flush),
            ('QH AH JH 2H KH', Combinations.flush),
            ('QC KC AC 2C JC', Combinations.flush),
            ('7D 5D 4D 2D 3D', Combinations.flush),
            ('7S 5S 4S 8S 3S', Combinations.flush),
            ('3S 2C 3H 2D 3D', Combinations.full_house),
            ('2S 2C 3H 2D 3D', Combinations.full_house),
            ('3S 2C 3H 3C 3D', Combinations.four_of_a_kind),
            ('2S 2C 2H 2D 3D', Combinations.four_of_a_kind),
            ('AH QH KH TH JH', Combinations.straight_flush),
            ('AS 5S 4S 2S 3S', Combinations.straight_flush),
            ('6S 5S 4S 2S 3S', Combinations.straight_flush),
            ('QH AH JH TH KH', Combinations.straight_flush),
            ('QC KC AC TC JC', Combinations.straight_flush),
            ('6D 5D 4D 2D 3D', Combinations.straight_flush),
            ('AD 5D 4D 2D 3D', Combinations.straight_flush),
            ]

    def test_invalid(self):
        inv = [
            '3S 4C',
            '3S 4C 4H',
            '3S 4C 5H 6C 8C',
            '2S AC QH KC JC',
            '2S 3S 2C 3H 4S',
            ]
        for comb in inv:
            with self.assertRaises(InvalidCombination):
                combination(Cards(comb))
                
    def test_combinations(self):
        for cards, comb in self.tst_combs:
            with self.subTest(comb):
                self.assertEqual(combination(Cards(cards)), comb)
    def test_beats_diff_combo(self):
        tst = filter(lambda c: c[1] > Combinations.three, self.tst_combs)
        comb = {}
        for c, t in tst:
            comb[t] = comb.get(t, []) + [c]
        for comb1, comb2 in itertools.combinations(sorted(comb.keys(), key = lambda c: -c.value), 2):
            for c1 in comb[comb1]:
                for c2 in comb[comb2]:
                    with self.subTest('{} > {}'.format(c1, c2)):
                        beats(Cards(c1), Cards(c2))
        
    def test_beats_same_combo(self):
        tst = [
            ('3S', '3D', 'single, mesmo valor'),
            ('4D', '3S', 'single, valores diferentes'),
            ('3S 3D', '3C 3H', 'pares, pelo naipe'),
            ('4C 4D', '3S 3H', 'pares'),
            ('2C 2D', '3S 3H', 'pares'),
            ('4C 4D 4H', '3S 3H 3D', 'trios'),
            ('2C 2D 2H', '3S 3H 3D', 'trios'),
            ('4C 5H 3D 2D 6H', 'AS 5S 4S 2C 3S', 'straight'),
            ('AH QS KS TS JS', 'AS 5S 4S 2C 3S', 'straight'),
            ('AH QS KS TS JS', 'AC QD KD TC JC', 'straight'),
            ('AH QS KS TS JS', '9H QS KS TS JS', 'straight'),
            ('7S 5S 4S 8S 3S', 'QH AH JH 2H KH', 'flush'),
            ('AS 6S 4S 5S 3S', 'KS QS 8S TS JS', 'flush'),
            ('2S 7S 4S 5S 3S', 'KS QS 8S AS JS', 'flush'),
            ('4S 5C 4H 5D 4D', '3S 2C 3H 2D 3D', 'full house'),
            ('2S 5C 2H 5D 2D', 'AS KC AH KD AD', 'full house'),
            ('4S 4C 4H 5D 4D', '3S 3C 3H 2D 3D', 'four of a kind'),
            ('2S 2C 2H 5D 2D', 'AS AC AH KD AD', 'four of a kind'),            
            ('4H 5H 3H 2H 6H', 'AH 5H 4H 2H 3H', 'straight flush'),
            ('AH QS KS TS JS', 'AS 5S 4S 2C 3S', 'straight flush'),
            ('AS QS KS TS JS', '9H QH KH TH JH', 'straight flush'),
            ('AS QS KS TS JS', 'AD QD KD TD JD', 'straight flush'),
            ]
        for c1, c2, nome in tst:
            with self.subTest(nome):
                self.assertTrue(beats(Cards(c1), Cards(c2)), nome)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print(('Uso:' + ' {}' * 5 + ' [nivel de log]').format(sys.argv[0], *['programa{}'.format(i) for i in range(1, 5)]))
        unittest.main()
    try:
        if len(sys.argv) == 5:
            g = GameHost(sys.argv[1:])
        else:
            if len(sys.argv) == 6:
                g = GameHost(sys.argv[1:5], int(sys.argv[5]))
            else:
                print(('Uso:' + ' {}' * 5 + ' [nivel de log]').format(sys.argv[0], *['programa{}'.format(i) for i in range(1, 5)]))
                sys.exit()
        g.cycle(3)
    except FileNotFoundError as p:
        print(p)
