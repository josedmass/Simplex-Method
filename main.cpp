#include <iostream>
#include <climits>
#include <cstring>
#include <iomanip>

#define TOL 0.00000001
#define MAX 200
#define INVIAVEL -1
#define ILIMITADO 0
#define OTIMO 1

using namespace std;

double tableau[MAX+1][2*MAX+1], auxtableau[MAX+2][3*MAX+2];
int can[MAX];

// Procura uma base trivial no tableau
bool idx_canonico(int n, int m) {
    bool flag = true, res = true, aux = false;
    int idx = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = n; j < n+m; j++) {
            flag = true;
            aux = false;
            if(tableau[i][j] == 1) {
                aux = true;
                for(int k = 1; k <= n; k++) {
                    if(k == i) continue;
                    if(tableau[k][j] != 0) { // Não pertence à base
                        flag = false;
                        break;
                    }
                }
                if(flag) {
                    can[idx] = j;
                    break;
                }
            }
        }
        if(!flag || !aux) res = false;
        idx++;
    }
    return res;
}

// Garante que esteja na forma canônica
void canonico(int n, int m) {
    int it = 1;
    for(int i = 0; i < n; i++) {
        int col = can[i];
        double val = tableau[0][col];
        if(val == 0.0) continue;
        for(int j = 0; j <= n+m; j++) {
            tableau[0][j] = tableau[0][j] - (tableau[it][j] * val);
        }
        it++;
    }
}

// Executa a primeira fase do Simplex, caso não haja uma base inicial, retornando o valor ótimo
int simplex_aux(int n, int m) {
    // 1. Montar o tableau auxiliar, a partir do tableau original
    auxtableau[0][n+m+n] = 0; auxtableau[1][n+m+n] = 0;
    for(int i = 2; i <= n+1; i++) {
        auxtableau[i][n+m+n] = tableau[i-1][n+m];
        for(int j = 0; j < n; j++) {
            auxtableau[i][j] = tableau[i-1][j];
            if(i-2 == j) {
                auxtableau[i][j+n+m] = 1;
            }
        }
    }
    for(int j = n; j < n+m; j++) {
        auxtableau[0][j] = tableau[0][j];
        auxtableau[1][j] = 0;
    }
    for(int i = 2; i <= n+1; i++) {
        for(int j = n; j < n+m; j++) {
            auxtableau[i][j] = tableau[i-1][j];
        }
    }
    for(int j = n+m; j < n+m+n; j++) {
        auxtableau[0][j] = 0;
        auxtableau[1][j] = 1;
    }

    // 2. Deixar canônico: L1 = -L2-L3...
    for(int j = 0; j <= n+m+n; j++) {
        for(int i = 2; i <= n+1; i++) {
            auxtableau[1][j] -= auxtableau[i][j];
        }
    }

    // 3. Rodamos o Método Simplex no auxtableau
    for(int j = n; j < n+m+n; j++) {
        if(auxtableau[1][j] < 0-TOL) {// Coluna j a ser melhorada
            // Buscamos a linha i que tenha a menor razão
            int line = 0;
            double razao = 100000000;
            for(int i = 2; i <= n+1; i++) {
                if(auxtableau[i][j] > 0) {
                    if((auxtableau[i][n+m+n]*1.0 / auxtableau[i][j]) < razao) {
                        razao = auxtableau[i][n+m+n]*1.0 / auxtableau[i][j];
                        line = i;
                    }
                }
            }
            if(auxtableau[line][j] != 1) {
                double val = auxtableau[line][j];
                for(int k = 0; k <= n+m+n; k++) auxtableau[line][k] /= val;
            }
            for(int k = 0; k <= n+1; k++) {
                if(k == line) continue;
                double val = auxtableau[k][j];
                for(int l = 0; l <= n+m+n; l++)
                    auxtableau[k][l] = auxtableau[k][l] - (auxtableau[line][l] * val);
            }
            j = n-1;
        }
    }
    return auxtableau[1][n+m+n];    
}

// Executa o Método Simplex, retornando se foi ótimo, inviável ou ilimitado
int simplex(int n, int m) {
    // 1. Toda entrada de b deve ser positiva
    for(int i = 1; i <= n; i++)
        if(tableau[i][n+m] < 0)
            for(int j = 0; j <= n+m; j++) tableau[i][j] *= -1;

    // 2. Procuramos uma base trivial para começar o algoritmo
    bool base = idx_canonico(n, m);
    // Se não tem base -> PL auxiliar 
    if(!base) {
        int fase1 = simplex_aux(n, m);
        if(fase1 < 0) return INVIAVEL;
        else { // remontar o tableau a partir do auxiliar
            tableau[0][n+m] = auxtableau[0][n+m+n];
            for(int j = 0; j < n+m; j++) tableau[0][j] = auxtableau[0][j];
            for(int i = 1; i <= n; i++) {
                tableau[i][n+m] = auxtableau[i+1][n+m+n];
                for(int j = 0; j < n+m; j++)
                    tableau[i][j] = auxtableau[i+1][j];
            }
        }
        base = idx_canonico(n, m);
    } else {
        canonico(n, m);  // Deixa a PL na forma canônica
    }

    // 3. Iniciamos o simplex
    for(int j = n; j < n+m; j++) {
        if(tableau[0][j] < 0-TOL) { // Coluna j a ser melhorada
            // Buscamos a linha i que tenha a menor razão
            int line = 0;
            double razao = 100000000;
            for(int i = 1; i <= n; i++) {
                if(tableau[i][j] > 0) {
                    if((tableau[i][n+m]*1.0 / tableau[i][j]) < razao) {
                        razao = tableau[i][n+m]*1.0 / tableau[i][j];
                        line = i;
                    }
                }
            }
            if(line == 0) return ILIMITADO;
            else {
                // Adicionamos a coluna j na base, deixando-a canônica
                if(tableau[line][j] != 1) {
                    double val = tableau[line][j];
                    for(int k = 0; k <= n+m; k++)
                        tableau[line][k] /= val;
                }
                for(int k = 0; k <= n; k++) {
                    if(k == line) continue;
                    double val = tableau[k][j];
                    for(int l = 0; l <= n+m; l++) {
                        tableau[k][l] = tableau[k][l] - (tableau[line][l] * val);
                    }
                }
                j = n-1; //voltamos
            }
        }
    }
    return OTIMO;
}

void print_otimo(int n, int m) {
    cout << "otima" << endl;
    cout << fixed << setprecision(3) << tableau[0][n+m] << endl; //valor ótimo
    idx_canonico(n, m); double x[m]; memset(x, 0.0, sizeof(x));
    for(int i = 0; i < n; i++)
        x[can[i]-n] = tableau[i+1][n+m];
    for(int i = 0; i < m; i++)
        cout << fixed << setprecision(3) << x[i] << " "; //valores de x
    cout << endl;
    for(int j = 0; j < n; j++) cout << fixed << setprecision(3) << tableau[0][j] << " "; //certificado
    cout << endl;
}

void print_ilimitado(int n, int m) {
    cout << "ilimitada" << endl;

    idx_canonico(n, m); double x[m]; memset(x, 0.0, sizeof(x));
    for(int i = 0; i < n; i++)
        x[can[i]-n] = tableau[i+1][n+m];
    for(int i = 0; i < m; i++)
        cout << fixed << setprecision(3) << x[i] << " "; //valores de x
    cout << endl;

    memset(x, 0.0, sizeof(x));
    for(int j = n; j < n+m; j++) {
        if(tableau[0][j] < 0-TOL) { // podia ser melhorada
            x[j-n] = 1;
            for(int i = 1; i <= n; i++)
                x[can[i-1]-n] = -tableau[i][j];
            break;
        }
    }
    for(int i = 0; i < m; i++)
        cout << fixed << setprecision(3) << x[i] << " "; //certificado
    cout << endl;
}

void print_inviavel(int n, int m) {
    cout << "inviavel" << endl;
    for(int j = 0; j < n; j++) cout << fixed << setprecision(3) << auxtableau[1][j] << " "; //certificado
    cout << endl;
}

int main() {

    memset(tableau, 0.0, sizeof(tableau));
    memset(auxtableau, 0.0, sizeof(auxtableau));

    int n, m;
    cin >> n >> m;
    // Lendo os dados e inserindo-os no tableau
    for(int i = 0; i < m; i++) {cin >> tableau[0][i+n]; tableau[0][i+n] *= -1;}
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= m; j++) {
            if(i-1 == j) tableau[i][j] = 1;
            cin >> tableau[i][j+n];
        }
    }
    int res = simplex(n, m);

    switch (res) {
        case OTIMO:
            print_otimo(n, m);
            break;
        case ILIMITADO:
            print_ilimitado(n, m);
            break;
        default:
            print_inviavel(n, m);
            break;
    }

    return 0;
}