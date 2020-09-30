CFLAGS=-Wall -Wextra -pedantic -Wno-long-long -O2

EXEC = cartas

compile:
	sudo cp -r img /var/www/html
	sudo cp -r 00.jpg /var/www/html/cards
	gcc $(FLAGS) -o $(EXEC) cartas.c estruturas.h bots.c bots.h
	sudo mv $(EXEC) /usr/lib/cgi-bin

open:
	xdg-open http://127.0.0.1/cgi-bin/cartas

clear:
	sudo rm -rf /var/www/html/img
	sudo rm -rf /var/www/html/cards/00.jpg

install:
	make clear
	make compile
	make open
