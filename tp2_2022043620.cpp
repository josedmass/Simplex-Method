#include <iostream>
#include <climits>
#include <vector>

#define INVIAVEL -1
#define ILIMITADO 0
#define OTIMO 1

using namespace std;

// Função auxiliar utilizada para depuração
void print_tableau(const vector<vector<double>>& tableau) {
    for(int i = 0; i < tableau.size(); i++) {
        for(int j = 0; j < tableau[i].size(); j++) {
            cout << tableau[i][j] << " ";
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

// Cria a matriz U, que soma lin1 multiplicada por val a lin2
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

// Cria a matriz D, que multiplica a linha lin por um real val
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
    for(int j = n; j < n+m; j++) aux_tab[n+1][j] = tableau[0][j]; //linha final para facilitar
}

// Retorna um vetor com os índices da base canônica
vector<int> idx_canonico(const vector<vector<double>>& tableau, int n, int m, bool flag) {
    vector<int> ans, aux;
    for(int i = 1; i <= n; i++) {
        for(int j = n; j < n+m; j++) {
            bool flag = true;
            if(tableau[i][j] == 1) {
                for(int k = 1; k <= n; k++) {
                    if(k == i) continue;
                    if(tableau[k][j] != 0) { // Não pertence à base
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
        //print_tableau(tableau);
        //cout << "|" << idx << "|" << endl;
        double val = -tableau[0][idx];
        //cout << val << endl;
        if(val == 0) {
            it++;
            continue;
        }
        vector<vector<double>> U;
        matriz_pivoteamento(U, val, it, 0, n);
        //print_tableau(tableau); cout << endl;
        tableau = mult(U, tableau);
        it++;
    }
    //print_tableau(tableau); cout << endl;
}

// Executa a primeira fase do simplex, verificando se a PL é viável
void fase1_simplex(vector<vector<double>>& tableau, int n, int m) {
    // Passo 1: fazer com que as variáveis artificiais adicionadas sejam iguais a 0 no objetivo artificial
    // Basta somar cada uma das linhas x(-1) a primeira linha
    vector<vector<double>> U(n+2, vector<double>(n+2, 0));
    for(int j = 1; j < n+1; j++) U[0][j] = -1;
    for(int i = 0; i < n+2; i++) {
        for(int j = 0; j < n+2; j++) {
            if(i == j) U[i][j] = 1;
        }
    }
    tableau = mult(U, tableau);

    //print_tableau(tableau);

    //cout << "oii" << endl;

    // Passo 2: realizar o algoritmo do simplex com o auxiliar
    for(int i = n; i < n+m+n; i++) {
        if(tableau[0][i] < 0) {
            //print_tableau(tableau);
            int line = 0;
            double razao = 100000000;
            for(int j = 1; j <= n; j++) {
                //cout << razao << endl;
                if(tableau[j][i] > 0) {
                    if((tableau[j][n+m+n]*1.0 / tableau[j][i]) < razao) {
                        razao = tableau[j][n+m+n]*1.0 / tableau[j][i];
                        line = j;
                    }
                }
            }
            //cout << "hello || " << line << "|" << i << endl;
            if(tableau[line][i] != 1) {
                //cout << "line: " << line << " | i: " << i << endl;
                //cout << 1.0/tableau[line][i] << endl;
                vector<vector<double>> D;
                matriz_multiplicacao(D, line, 1.0/tableau[line][i], n+1);
                tableau = mult(D, tableau);
            }
            //cout << "hello" << endl;
            for(int j = 0; j <= n+1; j++) {
                if(j == line) continue;
                vector<vector<double>> U;
                matriz_pivoteamento(U, -tableau[j][i], line, j, n+1);
                tableau = mult(U, tableau);
            }
            i = n-1;
        }
    }

    //print_tableau(tableau);

}

// Executa o simplex e retorna o tableau final
pair<int, vector<vector<double>>> simplex(vector<vector<double>>& A, vector<vector<double>>& b, vector<vector<double>>& c) {
    int n = A.size(), m = A[0].size();
    vector<vector<double>> tableau; 
    tableau_inicial(tableau, A, b, c);
    //print_tableau(tableau);
    vector<int> can = idx_canonico(tableau, n, m, false);
    // for(int idx : can) cout << idx << " ";
    // cout << endl;
    
    if(can.size() == 0) { // Não há uma base canônica explícita, então buscamos resolver a PL auxiliar
        //cout << "||" << "INICIO Fase 1 Simplex" << "||" << endl;
        vector<vector<double>> aux_tab;
        aux_tableau_inicial(aux_tab, tableau, n, m);
        //print_tableau(aux_tab);
        fase1_simplex(aux_tab, n, m);
        //print_tableau(aux_tab);
        if(aux_tab[0][n+m+n] < 0) return {INVIAVEL, aux_tab};
        else {
            for(int i = 0; i <= n; i++) {
                for(int j = 0; j < n+m; j++) {
                    tableau[i][j] = aux_tab[i][j];
                }
            }
            for(int i = 0; i <= n; i++) tableau[i][n+m] = aux_tab[i][n+m+n];
            for(int j = n; j < n+m; j++) tableau[0][j] = aux_tab[n+1][j];
            tableau[0][n+m] = aux_tab[n+1][n+m+n];
        }
    }
    //cout << "||" << "FIM Fase 1 Simplex" << "||" << endl;
    //print_tableau(tableau);
    can = idx_canonico(tableau, n, m, false);
    // 1. Garantir que esteja na forma canônica
    //print_tableau(tableau);
    canonico(tableau, can, n);
    //print_tableau(tableau);
    // 2. Encontrar uma coluna na primeira linha de valor negativo
    //cout << "|| INICIO PIVOTEAMENTO ||" << endl;
    for(int i = n; i < n+m; i++) {
        if(tableau[0][i] < 0) {
            //print_tableau(tableau);
            // 2.1. Buscamos a linha que tem a menor razão
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
            if(line == 0) return {ILIMITADO, tableau};
            else {
                //cout << "|" << line << "|" << endl;
                if(tableau[line][i] != 1) {
                    vector<vector<double>> D;
                    matriz_multiplicacao(D, line, 1.0/tableau[line][i], n);
                    // for(int it1 = 0; it1 <= n; it1++) {
                    //     for(int it2 = 0; it2 <= n; it2++)
                    //         cout << D[it1][it2] << " ";
                    //     cout << endl;
                    // }
                    tableau = mult(D, tableau);
                    // cout << "D" << endl;
                    // print_tableau(tableau);
                }
                // Pivoteamos, fazendo com que todos os elementos além de line sejam 0
                for(int j = 0; j <= n; j++) {
                    if(j == line) continue;
                    vector<vector<double>> U;
                    matriz_pivoteamento(U, -tableau[j][i], line, j, n);
                    tableau = mult(U, tableau);
                }
                i = n-1;
            }

        }
    }
    //print_tableau(tableau);
    return {OTIMO, tableau}; // Não tem mais nenhum valor negativo

}

int main() {

    int n, m;
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

    //Passo 2: execução do algoritmo simplex
    pair<int, vector<vector<double>>> simp = simplex(A, b, c);
    if(simp.first == INVIAVEL) {
        cout << "inviavel" << endl;
        for(int j = 0; j < n; j++) cout << simp.second[0][j] << " ";
        cout << endl;
    }
    else if(simp.first == OTIMO) {
        cout << "otima" << endl;
        cout << simp.second[0][n+m] << endl; // valor ótimo
        //print_tableau(simp.second);
        vector<int> can = idx_canonico(simp.second, n, m, true);
        //cout << can.size() << endl;
        vector<double> x(m, 0);
        int it = 1;
        for(int idx : can) {
            //cout << idx << " ";
            x[idx-n] = simp.second[it][n+m];
            it++;
        }
        for(double val : x) cout << val << " "; // valores das variáveis ótimas
        cout << endl;
        for(int i = 0; i < n; i++) cout << simp.second[0][i] << " "; // certificado
        cout << endl;
    }
    else if(simp.first == ILIMITADO) {
        cout << "ilimitada" << endl;
        vector<int> can = idx_canonico(simp.second, n, m, true);
        vector<int> x(m, 0);
        int it = 1;
        for(int idx : can) {
            x[idx-n] = simp.second[it][n+m];
            it++;
        }
        for(int val : x) cout << val << " ";
        cout << endl;
        vector<int> d(m, 0);
        int i = n;
        for(; i < n+m; i++) {
            if(simp.second[0][i] < 0) { // linha onde deu problema
                d[i-n] = 1;
                break;
            }
        }
        it = 1;
        for(int idx : can) {
            d[idx - n] = -simp.second[it][i];
            it++;
        }
        for(int val : d) cout << val << " ";
        cout << endl;
    }
    // vector<vector<double>> tableau;
    // tableau_inicial(tableau, A, b, c);
    // vector<vector<double>> aux;
    // aux_tableau_inicial(aux, tableau, n, m);
    // vector<int> can = idx_canonico(tableau, n, m);
    // canonico(tableau, can, n);

    // vector<int> canonic = canonico(tableau, n, m);
    // for(int i : canonic) cout << i << " ";
    // cout << endl;


    return 0;
}