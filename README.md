# Simplex-Method
Implementação do algoritmo Simplex para resolução de problemas de otimização linear para o Trabalho Prático 2 da disciplina de Pesquisa Operacional da UFMG, em 2024/1.

## Autor
- José Eduardo Duarte Massucato

## Funcionalidades
O algoritmo busca resolver a programação linear definida por

$$
\text{max} \quad c^T x
$$

sujeito a:

$$
A x = b
$$

e

$$
x \geq 0
$$

e encontrar o certificado que comprove seu resultado, onde

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

Diante disso, temos:
- Se o problema possui solução ótima, é impresso **"otima"**, seguido do seu valor ótimo, dos valores ótimos do vetor de solução X e, na última linha, o certificado de otimalidade.
- Se o problema é ilimitado, é impresso **"ilimitada"**, seguido de uma solução viável para X e, na última linha, um certificado de ilimitabilidade.
- Se o problema é inviável, é impresso **"inviavel"**, seguido do certificado de inviabilidade.

## Entrada
A primeira linha da entrada contém dois inteiros $n$ e $m$, o número de restrições e variáveis, respectivamente. A segunda linha contém $m$ inteiros, $c_i$, que formam o vetor $c$ de custo. 

Cada uma das $n$ linhas seguintes contêm $m+1$ inteiros que representam as restrições. Para cada $i$-ésima linha, os $m$ primeiros números são $a_{i1}, a_{i2}, ..., a_{im}$, enquanto o último é $b_i$. Todos esses valores **podem ser negativos**.

Uma entrada genérica é da forma

$$
    \begin{aligned}
        &\text{n}   &\quad &\text{m}   &\quad & & & & \\
        &c_1        &\quad &c_2        &\quad &\cdots &\quad &c_m \\
        &a_{11}     &\quad &a_{12}     &\quad &\cdots &\quad &a_{1m} &\quad &b_1 \\
        &a_{21}     &\quad &a_{22}     &\quad &\cdots &\quad &a_{2m} &\quad &b_2 \\
        &\vdots     &\quad &\vdots     &\quad &\ddots &\quad &\vdots &\quad &\vdots \\
        &a_{n1}     &\quad &a_{n2}     &\quad &\cdots &\quad &a_{nm} &\quad &b_n \\
    \end{aligned}
$$

onde

$1 \leq n \leq 100$  
$1 \leq m \leq 100$  

$\forall i \in \{1,...,n\} \text{ e } \forall j \in \{1,...,m\}: |a_{ij}| \leq 100, |b_i| \leq 100, |c_j| \leq 100$

## Como usar
1. Compilar o programa
```
g++ main.cpp -o exe
```

2. Executar com algum arquivo de entrada
```
./exe < <caminho_para_arquivo_de_entrada>
```