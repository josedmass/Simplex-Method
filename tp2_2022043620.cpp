#include <iostream>
#include <vector>

#define INVIAVEL -1
#define ILIMITADO 0
#define OTIMO 1

using namespace std;

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

// Constrói o tableau inicial
void tableau_inicial(vector<vector<int>>& tableau, vector<vector<int>>& A, vector<vector<int>>& b, vector<vector<int>>& c) {
    int n = A.size(), m = A[0].size();
    tableau = vector<vector<int>>(n+1, vector<int>(n+m+1));
    for(int i = 0; i < n; i++) tableau[0][i] = 0;
    for(int i = n; i < n+m; i++) tableau[0][i] = -c[i][0];
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

    // Alterando linhas onde o b[i] era negativo
    for(int i = 0; i < n; i++) {
        if(tableau[i+1][n+m] < 0) {
            for(int j = 0; j < n+m+1; j++) tableau[i+1][j] *= -1;
        }
    }

}

// Executa o simplex e retorna o tableau final
pair<int, vector<vector<int>>> simplex(vector<vector<int>>& A, vector<vector<int>>& b, vector<vector<int>>& c) {
    int n = A.size(), m = A[0].size();
    vector<vector<int>> tableau; 
    tableau_inicial(tableau, A, b, c);
    
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

    // Passo 2: execução do algoritmo simplex


    return 0;
}