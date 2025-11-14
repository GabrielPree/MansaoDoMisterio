# 🕵️‍♂️ Mansão do Mistério – Jogo Detetive em C  
**Autor:** Gabriel Preé  

## 📌 Sobre o Projeto  
Este projeto implementa um jogo de investigação em C, onde o jogador explora uma mansão representada por uma **árvore binária**. Cada sala pode conter uma pista, que é coletada e armazenada em uma **árvore BST**. As pistas também são associadas a suspeitos por meio de uma **tabela hash**.

O objetivo final é explorar os cômodos, coletar pistas e acusar o culpado com base nas evidências acumuladas.

---

## 🧩 Estruturas Utilizadas  
- **Árvore Binária:** representa o mapa da mansão.  
- **BST (Árvore de Busca Binária):** armazena todas as pistas coletadas.  
- **Tabela Hash:** associa cada pista ao seu suspeito correspondente.  

---

## 🎮 Como Funciona  
1. O jogador inicia no **Hall de Entrada**.  
2. Pode navegar usando:
   - **(e)** – Ir para a esquerda  
   - **(d)** – Ir para a direita  
   - **(p)** – Ver pistas coletadas  
   - **(s)** – Sair e iniciar julgamento  
3. Pistas são coletadas automaticamente ao entrar em salas que possuem uma.  
4. Na fase final, o jogador acusa um suspeito.  
5. O suspeito só é culpado se houver **2 ou mais pistas** relacionadas a ele.  

---

## 🏁 Como Executar  
Compile e execute o projeto com:

```bash
gcc main.c -o mansao
./mansao
```

---

## 📜 Licença

#### Projeto livre — sem licença definida.
