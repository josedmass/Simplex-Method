#include <iostream>
#include <vector>

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