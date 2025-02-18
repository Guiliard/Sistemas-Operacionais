<h1 align="center">Simulador de Arquitetura Von Neumann</h1>

<div style="display: inline-block;">
<img align="center" height="20px" width="90px" src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white"/> 
<img align="center" height="20px" width="90px" src="https://img.shields.io/badge/Made%20for-VSCode-1f425f.svg"/> 
<img align="center" height="20px" width="90px" src="https://img.shields.io/badge/Contributions-welcome-brightgreen.svg?style=flat"/>
</div>
<br>

## Simulador de Arquitetura Von Neumann

O Simulador de Arquitetura Von Neumann é uma aplicação desenvolvida em C que simula o funcionamento de um computador baseado no modelo de Von Neumann. Nesse modelo, tanto os dados quanto as instruções são armazenados na mesma memória e acessados pelo mesmo barramento, o que caracteriza sua estrutura fundamental.

O projeto tem como objetivo ilustrar como o processador interage com a memória para executar processos, implementando diferentes algoritmos de escalonamento de processos. Além disso, ele realiza o gerenciamento de memória, incluindo o controle de memória cache e a simulação de sistemas multitarefas.

## Requisitos

- GCC >= 9.0
- Linux ou WSL (Windows Subsystem for Linux)
- Make >= 4.0

## Instalação

1. **Clone o repositório:**
    ```bash
    git clone https://github.com/Guiliard/Sistemas-Operacionais.git
    ```

2. **Acesse o repositório:**
    ```bash
    cd Sistemas-Operacionais
    ```

3. **Execute o seguinte comando:**
    ```bash
    clear && make clean && make && make run
    ```

4. **Vá até a pasta `output` para observar os resultados da simulação:**
    ```bash
    cd output
    ```

## Especificações do Dispositivo Utilizado

| Componentes            | Detalhes                                                                                         |
| -----------------------| -----------------------------------------------------------------------------------------------  |
|  `Processador`         | Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz   1.50 GHz                                             |
|  `RAM Instalada`       | 12.0 GB (Utilizável: 11.8 GB)                                                                    |
|  `Tipo de Sistema`     | Sistema Operacional de 64 bits, processador baseado em x64                                       |
|  `Sistema Operacional` | Linux Pop!_OS 22.04 LTS                                                                          |

## Referências

[1] TANENBAUM, A. S., & BOS, H. **Modern Operating Systems** (4th ed.). Pearson, 2014. Acessado em: 17 de Fevereiro de 2025.
