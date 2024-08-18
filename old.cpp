#include <iostream>
#include <climits>
#include <vector>
#include <iomanip>

#define INVIAVEL -1
#define ILIMITADO 0
#define OTIMO 1

using namespace std;

// Função auxiliar utilizada para depuração
void print_tableau(const vector<vector<double>>& tableau) {
    for(int i = 0; i < tableau.size(); i++) {
        for(int j = 0; j < tableau[i].size(); j++) {
            cout << tableau[i][j] << '\t';
        }
        cout << endl;
    }
    cout << endl;
}

// Multiplica a matriz A pela matriz B
vector<vector<double>> mult(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int n1 = A.size(), m1 = A[0].size();
    int n2 = B.size(), m2 = B[0].size();
    vector<vector<double>> C(n1, vector<double>(m2));
    if(m1 == n2) {
        for(int i = 0; i < n1; i++) {
            for(int j = 0; j < m2; j++) {
                double res = 0;
                for(int k = 0; k < m1; k++)
                    res += A[i][k] * B[k][j];
                C[i][j] = res;
            }
        }
    }
    return C;
}

// Cria a matriz U, que faz lin2 = lin2 + lin1 * val
void matriz_pivoteamento(vector<vector<double>>& U, double val, int lin1, int lin2, int n) {
    U = vector<vector<double>>(n+1, vector<double>(n+1));
    for(int i = 0; i < n+1; i++) {
        for(int j = 0; j < n+1; j++) {
            if(i == j) U[i][j] = 1;
            else if(i == lin2 && j == lin1) U[i][j] = val;
            else U[i][j] = 0;
        }
    }
}

// Cria a matriz D, que faz lin = lin * val
void matriz_multiplicacao(vector<vector<double>>& D, int lin, double val, int n) {
    D = vector<vector<double>>(n+1, vector<double>(n+1));
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= n; j++) {
            if(i == j) {
                if(i == lin) D[i][j] = val;
                else D[i][j] = 1;
            }
            else D[i][j] = 0;
        }
    }
}

// Constrói o tableau inicial, seguindo a formatação do slide
void tableau_inicial(vector<vector<double>>& tableau, const vector<vector<double>>& A, const vector<vector<double>>& b, const vector<vector<double>>& c) {
    int n = A.size(), m = A[0].size();
    tableau = vector<vector<double>>(n+1, vector<double>(n+m+1));
    for(int i = 0; i < n; i++) tableau[0][i] = 0;
    for(int i = n; i < n+m; i++) tableau[0][i] = -c[i-n][0];
    tableau[0][n+m] = 0;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) tableau[i+1][j] = 1;
            else tableau[i+1][j] = 0;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            tableau[i+1][n+j] = A[i][j];
        }
    }

    for(int i = 0; i < n; i++) tableau[i+1][n+m] = b[i][0];

    // Invertendo linhas onde o b[i] era negativo
    for(int i = 0; i < n; i++) {
        if(tableau[i+1][n+m] < 0) {
            for(int j = 0; j < n+m+1; j++) tableau[i+1][j] *= -1;
        }
    }
}

// Constrói o tableau auxiliar, quando não há uma base canônica trivial, seguindo a formatação do slide
void aux_tableau_inicial(vector<vector<double>>& aux_tab, const vector<vector<double>>& tableau, int n, int m) {
    aux_tab = vector<vector<double>>(n+2, vector<double>(n+m+n+1, 0));
    for(int i = 0; i < n; i++) aux_tab[0][i] = tableau[0][i];
    for(int i = n; i < n+m; i++) aux_tab[0][i] = 0;
    for(int i = n+m; i < n+m+n; i++) aux_tab[0][i] = 1;
    aux_tab[0][n+m+n] = 0;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            aux_tab[i+1][j] = tableau[i+1][j];
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            aux_tab[i+1][n+j] = tableau[i+1][n+j];
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) aux_tab[i+1][j+n+m] = 1;
            else aux_tab[i+1][j+n+m] = 0;
        }
    }

    for(int i = 0; i < n; i++) aux_tab[i+1][n+m+n] = tableau[i+1][n+m];

    // Linha final com os mesmos valores da primeira linha do tableau original, para facilitar na fase 2 do simplex
    for(int j = 0; j < n; j++) aux_tab[n+1][j] = 0;
    for(int j = n; j < n+m; j++) aux_tab[n+1][j] = tableau[0][j];
}

// Retorna um vetor com os índices da base canônica. Se flag = false, retorna uma base de tamanho n. Senão, retorna com tamanho <= n.
vector<int> idx_canonico(const vector<vector<double>>& tableau, int n, int m, bool flag) {
    vector<int> ans, aux;
    for(int i = 1; i <= n; i++) {
        for(int j = n; j < n+m; j++) {
            bool flag = true;
            if(tableau[i][j] == 1) {
                for(int k = 1; k <= n; k++) {
                    if(k == i) continue;
                    if(tableau[k][j] != 0) { //Não pertence à base
                        flag = false;
                        break;
                    }
                }
                if(flag) {
                    ans.push_back(j);
                    break;
                }
            }
        }
    }

    if(ans.size() < n && !flag) ans = aux;
    return ans;
}

// Faz com que a primeira linha das colunas canônicas seja zero
void canonico(vector<vector<double>>& tableau, const vector<int>& can, int n) {
    int it = 1;
    for(int idx : can) {
        double val = -tableau[0][idx];
        if(val == 0) {
            it++;
            continue;
        }
        vector<vector<double>> U;
        matriz_pivoteamento(U, val, it, 0, n);
        tableau = mult(U, tableau);
        it++;
    }
}

// Executa a primeira fase do simplex, verificando se a PL é viável
void fase1_simplex(vector<vector<double>>& tableau, int n, int m) {
    // 1. Zerar os valores das variáveis artificiais na primeira linha do tableau. Fazemos com que L1 = -L2-L3-...-Ln
    vector<vector<double>> U(n+2, vector<double>(n+2, 0));
    for(int j = 1; j < n+1; j++) U[0][j] = -1;
    for(int i = 0; i < n+2; i++) {
        for(int j = 0; j < n+2; j++) {
            if(i == j) U[i][j] = 1;
        }
    }
    tableau = mult(U, tableau);
    // 2. Rodamos o simplex com o tableau obtido
    for(int i = n; i < n+m+n; i++) {
        // 2.1. Buscamos uma coluna i com valor negativo
        if(tableau[0][i] < 0) {
            int line = 0;
            double razao = 100000000;
            for(int j = 1; j <= n; j++) {
                // 2.2. Buscamos a linha j com menor razão
                if(tableau[j][i] > 0) {
                    if((tableau[j][n+m+n]*1.0 / tableau[j][i]) < razao) {
                        razao = tableau[j][n+m+n]*1.0 / tableau[j][i];
                        line = j;
                    }
                }
            }
            // 2.3. Pivoteamos essa coluna, para incluí-la na base
            if(tableau[line][i] != 1) {
                // 2.3.1. O elemento da linha deve ser igual a 1
                vector<vector<double>> D;
                matriz_multiplicacao(D, line, 1.0/tableau[line][i], n+1);
                tableau = mult(D, tableau);
            }
            for(int j = 0; j <= n+1; j++) {
                // 2.3.2. Os demais elementos da coluna devem ser iguais a 0
                if(j == line) continue;
                vector<vector<double>> U;
                matriz_pivoteamento(U, -tableau[j][i], line, j, n+1);
                tableau = mult(U, tableau);
            }
            i = n-1;
        }
    }
}

// Executa o simplex e retorna o tableau final
pair<int, vector<vector<double>>> simplex(const vector<vector<double>>& A, const vector<vector<double>>& b, const vector<vector<double>>& c) {
    int n = A.size(), m = A[0].size();
    vector<vector<double>> tableau; 
    tableau_inicial(tableau, A, b, c);
    vector<int> can = idx_canonico(tableau, n, m, false);
    
    // SIMPLEX FASE 1: caso não haja uma base canônica explícita, buscamos resolver a PL auxiliar
    if(can.size() == 0) {
        vector<vector<double>> aux_tab;
        aux_tableau_inicial(aux_tab, tableau, n, m);
        fase1_simplex(aux_tab, n, m);

        // Se o valor ótimo foi negativo, significa que a PL original não possui solução viável
        if(aux_tab[0][n+m+n] < 0) return {INVIAVEL, aux_tab};
        else {
            // Monta o tableau para o problema original, utilizando a base encontrada na primeira fase
            for(int i = 1; i <= n; i++) {
                for(int j = 0; j < n+m; j++) {
                    tableau[i][j] = aux_tab[i][j];
                }
            }
            for(int j = 0; j < n; j++) tableau[0][j] = aux_tab[n+1][j];
            for(int i = 0; i <= n; i++) tableau[i][n+m] = aux_tab[i][n+m+n];
            for(int j = n; j < n+m; j++) tableau[0][j] = aux_tab[n+1][j];
            tableau[0][n+m] = aux_tab[n+1][n+m+n];
        }
    }

    // 1. Garantir que esteja na forma canônica
    can = idx_canonico(tableau, n, m, false);
    canonico(tableau, can, n);
    // 2. Encontrar uma coluna na primeira linha de valor negativo
    for(int i = n; i < n+m; i++) {
        if(tableau[0][i] < 0) {
            // 2.1. Buscamos a linha j que tem a menor razão
            int line = 0;
            double razao = 100000000;
            for(int j = 1; j <= n; j++) {
                if(tableau[j][i] > 0) {
                    if((tableau[j][n+m]*1.0 / tableau[j][i]) < razao) {
                        razao = tableau[j][n+m]*1.0 / tableau[j][i];
                        line = j;
                    }
                }
            }
            if(line == 0) return {ILIMITADO, tableau}; //Não achou nenhuma linha com valor > 0 para essa coluna
            else {
                // 3. Pivoteamos essa coluna, para incluí-la na base
                if(tableau[line][i] != 1) {
                    // 3.1. O elemento da linha deve ser igual a 1
                    vector<vector<double>> D;
                    matriz_multiplicacao(D, line, 1.0/tableau[line][i], n);
                    tableau = mult(D, tableau);
                }
                for(int j = 0; j <= n; j++) {
                    // 3.2. Os demais elementos da coluna devem ser iguais a 0
                    if(j == line) continue;
                    vector<vector<double>> U;
                    matriz_pivoteamento(U, -tableau[j][i], line, j, n);
                    tableau = mult(U, tableau);
                }
                i = n-1;
            }
        }
    }
    return {OTIMO, tableau}; //Não tem mais nenhum valor negativo
}

void print_inviavel(const vector<vector<double>>& tableau, int n, int m) {
    cout << "inviavel" << endl;
    for(int j = 0; j < n; j++) cout << fixed << setprecision(3) << tableau[0][j] << " "; //certificado
    cout << endl;
}

void print_otimo(const vector<vector<double>>& tableau, int n, int m) {
    cout << "otima" << endl;
    cout << fixed << setprecision(3) << tableau[0][n+m] << endl; //valor ótimo
    vector<int> can = idx_canonico(tableau, n, m, true); //true para pegar a base existentes, mesmo que não seja completa
    vector<double> x(m, 0);
    int it = 1;
    for(int idx : can) {
        x[idx-n] = tableau[it][n+m];
        it++;
    }
    for(double val : x) cout << fixed << setprecision(3) << val << " ";  //valores ótimos de x
    cout << endl;
    for(int j = 0; j < n; j++) cout << fixed << setprecision(3) << tableau[0][j] << " "; //certificado
    cout << endl;
}

void print_ilimitado(const vector<vector<double>>& tableau, int n, int m) {
    cout << "ilimitada" << endl;
    vector<int> can = idx_canonico(tableau, n, m, true); //true para pegar a base existentes, mesmo que não seja completa
    vector<double> x(m, 0);
    int it = 1;
    for(int idx : can) {
        x[idx-n] = tableau[it][n+m];
        it++;
    }
    for(double val : x) cout << fixed << setprecision(3) << val << " "; //valores da base alcançada
    cout << endl;
    vector<double> d(m, 0);
    int i = n;
    while(i < n+m) {
        if(tableau[0][i] < 0) { //linha onde deu problema
            d[i-n] = 1;
            break;
        }
        i++;
    }
    it = 1;
    for(int idx : can) {
        d[idx-n] = -tableau[it][i];
        it++;
    }
    for(double val : d) cout << fixed << setprecision(3) << val << " "; //certificado
    cout << endl;
}

int main() {

    int n, m; //n restrições e m variáveis
    cin >> n >> m;

    vector<vector<double>> A(n, vector<double>(m));
    vector<vector<double>> b(n, vector<double>(1));
    vector<vector<double>> c(m, vector<double>(1));

    // Passo 1: leitura dos dados
    for(int i = 0; i < m; i++) cin >> c[i][0];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++)
            cin >> A[i][j];
        cin >> b[i][0];
    }

    // Passo 2: execução do algoritmo simplex
    pair<int, vector<vector<double>>> simp = simplex(A, b, c);

    // Passo 3: impressão dos resultados
    if(simp.first == INVIAVEL) print_inviavel(simp.second, n, m);
    else if(simp.first == OTIMO) print_otimo(simp.second, n, m);
    else if(simp.first == ILIMITADO) print_ilimitado(simp.second, n, m);

    return 0;
}