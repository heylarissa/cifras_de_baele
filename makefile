CFLAGS = -Wall -g -std=c99 # gerar "warnings" detalhados e infos de depuração
 
objs = decode.o dictkey.o encode.o filehandler.o
 
# regra default (primeira regra)
all: beale
 
# regras de ligacao
beale: $(objs)
 
# regras de compilação
dictkey.o: dictkey.c dictkey.h
filehandler.o: filehandler.c filehandler.h
decode.o: decode.c decode.h
encode.o: encode.c encode.h
 
# remove arquivos temporários
clean:
	-rm -f $(objs) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f beale