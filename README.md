# Cifras de Beale

### Funcionalidades

#### Funções de codificação
[x] Receber input em arquivo
[x] Abrir livro cifra  
[x] Load chaves na memória  
[x] Cria arquivo de chaves  
[x] Busca letra da mensagem original na lista de chaves  

#### Funções de decodificação
[x] Carregar arquivo de chaves na memória  
[x] Load chaves na memória (livro cifra)  
[x] Busca código da chave na memória  
[x] Escrever em arquivo de saída

#### Outros requisitos
[ ] Makefile
[x] Codificar letras que não existem no livro cifra
[x] Permitir que os comandos sejam inputados em diferentes ordens
[x] Compilar em C99

![Beale](https://user-images.githubusercontent.com/57672954/232651219-eaab62db-df86-4fea-aca0-d2f05a79e78a.jpg)

### Execução
    Codificar
    ./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    Decodificar
    ./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada  
    ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada
