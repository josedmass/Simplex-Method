#include <iostream>
#include <vector>

#define INVIAVEL -1
#define ILIMITADO 0
#define OTIMO 1

using namespace std;

// Função auxiliar utilizada para depuração
void print_tableau(vector<vector<int>>& tableau) {
    for(int i = 0; i < tableau.size(); i++) {
        for(int j = 0; j < tableau[i].size(); j++) {
            cout << tableau[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Multiplica a matriz A pela matriz B
vector<vector<int>> mult(vector<vector<int>>& A, vector<vector<int>>& B) {
    int n1 = A.size(), m1 = A[0].size();
    int n2 = B.size(), m2 = B[0].size();
    vector<vector<int>> C(n1, vector<int>(m2));
    if(m1 == n2) {
        for(int i = 0; i < n1; i++) {
            for(int j = 0; j < m2; j++) {
                int res = 0;
                for(int k = 0; k < m1; k++)
                    res += A[i][k] * B[k][j];
                C[i][j] = res;
            }
        }
    }
    return C;
}

// Constrói o tableau inicial, seguindo a formatação do slide
void tableau_inicial(vector<vector<int>>& tableau, vector<vector<int>>& A, vector<vector<int>>& b, vector<vector<int>>& c) {
    int n = A.size(), m = A[0].size();
    tableau = vector<vector<int>>(n+1, vector<int>(n+m+1));
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
void aux_tableau_inicial(vector<vector<int>>& aux_tab, const vector<vector<int>>& tableau, int n, int m) {
    aux_tab = vector<vector<int>>(n+1, vector<int>(n+m+n+1));
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
}

// Retorna um vetor com os índices da base canônica
vector<int> canonico(vector<vector<int>>& tableau, int n, int m) {
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

    if(ans.size() < n) ans = aux;
    return ans;
}

// Executa a primeira fase do simplex, verificando se a PL é viável
pair<int, vector<vector<int>>> Fase1_Simplex(vector<vector<int>>& tableau, int n, int m) {

}

// Executa o simplex e retorna o tableau final
pair<int, vector<vector<int>>> simplex(vector<vector<int>>& A, vector<vector<int>>& b, vector<vector<int>>& c) {
    int n = A.size(), m = A[0].size();
    vector<vector<int>> tableau; 
    tableau_inicial(tableau, A, b, c);
    vector<int> can = canonico(tableau, n, m);
    if(can.size() == 0) { // Não há uma base canônica explícita, então buscamos resolver a PL auxiliar
        vector<vector<int>> aux_tab;
        aux_tableau_inicial(aux_tab, tableau, n, m);
        //pair<int, vector<vector<int>>> fase1 = Fase1_Simplex(aux_tab, n, m);
    }

}

int main() {

    int n, m;
    cin >> n >> m;

    vector<vector<int>> A(n, vector<int>(m));
    vector<vector<int>> b(n, vector<int>(1));
    vector<vector<int>> c(m, vector<int>(1));

    // Passo 1: leitura dos dados
    for(int i = 0; i < m; i++) cin >> c[i][0];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++)
            cin >> A[i][j];
        cin >> b[i][0];
    }

    //Passo 2: execução do algoritmo simplex
    // vector<vector<int>> tableau;
    // tableau_inicial(tableau, A, b, c);
    // vector<vector<int>> aux;
    // aux_tab_inicial(aux, tableau, n, m);
    // print_tableau(tableau);
    // print_tableau(aux);

    // vector<int> canonic = canonico(tableau, n, m);
    // for(int i : canonic) cout << i << " ";
    // cout << endl;


    return 0;
}