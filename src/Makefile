main: clean cliente servidor

token: token.cpp token.h
	g++ -c token.cpp token.h -g -Wall

tempo: tempo.cpp tempo.h
	g++ -c tempo.cpp tempo.h -g -Wall

servidor: servidor.cpp servidor.h tempo token
	g++ -o servidor servidor.cpp servidor.h tempo.o token.o -g -Wall

tserv: servidor
	./servidor 1234

ttempo: clean token tempo
	g++ -o tst tempotst.c tempo.o token.o -g -Wall
	./tst

cliente: cliente.cpp
	g++ -o cliente cliente.cpp -g -Wall

tclient: cliente
	./cliente 127.0.0.1 1234

clean:
	rm -rf cliente servidor tst *.o *.h.gch
	clear
