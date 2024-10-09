<h1 align="center"> Sistemas Operacionais</h1>
<div style="display: inline-block;">
<img align="center" height="20px" width="90px" src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white"/> 
<img align="center" height="20px" width="90px" src="https://img.shields.io/badge/Made%20for-VSCode-1f425f.svg"/> 
<img align="center" height="20px" width="90px" src="https://img.shields.io/badge/Contributions-welcome-brightgreen.svg?style=flat"/>
</div>
<br>

## Introdução

O objetivo deste projeto é desenvolver um sistema operacional simplificado utilizando a linguagem C, que se caracteriza por seu baixo nível de abstração e alto controle sobre os recursos de hardware. Este trabalho acadêmico, proposto pelo professor <a href="https://github.com/mpiress" target="_blank">Michel Pires</a> (CEFET/MG), foi realizado para oferecer uma compreensão dos conceitos fundamentais de sistemas operacionais, como gerenciamento de processos, controle de memória e interação com dispositivos de entrada e saída. A escolha da linguagem C permite uma experiência prática de como os sistemas operacionais gerenciam diretamente os recursos de hardware, proporcionando uma visão detalhada da arquitetura de computadores.

## Compilação e Execução

Um arquivo Makefile que realiza todo o procedimento de compilação e execução está disponível no código. Para utilizá-lo, siga as diretrizes de execução no terminal:

| Comando                |  Função                                                                                           |                     
| -----------------------| ------------------------------------------------------------------------------------------------- |
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                        |
|  `make`                | Executa a compilação do programa utilizando o gcc, e o executável vai para a pasta build          |
|  `make run`            | Executa o programa da pasta build após a realização da compilação                                 |