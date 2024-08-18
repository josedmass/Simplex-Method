# Simplex-Method
Implementação do algoritmo Simplex para resolução de problemas de otimização linear para o Trabalho Prático 2 da disciplina de Pesquisa Operacional da UFMG, em 2024/1.

## Autor
- José Eduardo Duarte Massucato

## Entrada
$$
    A = \begin{pmatrix}
    a_{11} & a_{12} & \cdots & a_{1m} \\
    a_{21} & a_{22} & \cdots & a_{2m} \\
    \vdots & \vdots & \ddots & \vdots \\
    a_{m1} & a_{m2} & \cdots & a_{nm}
    \end{pmatrix}
    \quad
    b = \begin{pmatrix}
    b_1 \\
    b_2 \\
    \vdots \\
    b_n
    \end{pmatrix}
    \quad
    c = \begin{pmatrix}
    c_1 \\
    c_2 \\
    \vdots \\
    c_m
    \end{pmatrix}
$$


... descrição da entrada ...

## Funcionalidades
Resolução de modelos de otimização linear, utilizando a linguagem C++.
- Se o problema possui solução ótima, é impresso "otima", seguido do seu valor ótimo, dos valores ótimos do vetor X e, na última linha, o certificado de otimalidade.
- Se o problema é ilimitado, é impresso "ilimitada", seguido de uma solução viável para X e, na última linha, um certificado de ilimitabilidade.
- Se o problema é inviável, é impresso "inviavel", seguido do certificado de inviabilidade.

## Como usar
...