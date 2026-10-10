# PONG (Jogo Local PvP & PvC)

Jogo Pong clássico recriado em linguagem C utilizando a biblioteca SDL2.

## Modos de Jogo
* **1 PLAYER (VS CPU):** Jogador vs Computador.
* **2 PLAYERS (P1 VS P2):** Jogador 1 vs Jogador 2 local no mesmo teclado.

## Controles
### Menu Inicial:
* **Setas Cima / Baixo** ou Teclas **1** e **2**: Selecionar o Modo de Jogo
* **Espaço** ou **Enter**: Iniciar a partida

### Durante a Partida:
* **Jogador 1 (Paleta Esquerda):** Teclas `W` (Subir) e `S` (Descer) *(no modo 1P, as setas também funcionam para P1)*
* **Jogador 2 (Paleta Direita - Modo 2P):** Teclas `Seta para Cima` e `Seta para Baixo`
* **ESC:** Sair do jogo

## Como Compilar
Para compilar no Windows com GCC e SDL2:
```bash
gcc -DSDL_MAIN_HANDLED pong.c -IE:/msys64/ucrt64/include/SDL2 -LE:/msys64/ucrt64/lib -lSDL2 -o pong.exe
```
Ou com `sdl2-config` em Linux:
```bash
gcc pong.c `sdl2-config --cflags --libs` -o pong
```
