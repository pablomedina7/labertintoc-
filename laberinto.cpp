#include <iostream> //para la entrada y salida.
#include <ctime>    //para generar numeros aleatorios 
#include <cstdlib>  //para generar numeros aleatorios 
#include <vector>   //para usar vectores (arrays dinamicos)
#include <stack>    //para usar pilas (estructura de datos)
#include <thread>   //para controlar las pausas en la ejecucion 
#include <chrono>   

using namespace std;
/*SE DEFINEN LOS MOVIMIENTOS*/
int dx[] = { 0, 0, 1, -1 };
int dy[] = { 1, -1, 0, 0 };

// Función para imprimir el laberinto y la solución en su estado actual
void imprimirLaberinto(const vector<vector<int>>& matriz, const vector<vector<int>>& solucion, int delay = 100) {
    system("cls"); // para la buena ejecucion 
    int filas = matriz.size();
    int columnas = matriz[0].size();
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (solucion[i][j] == 1) {
                cout << " _";
            } else if (matriz[i][j] == 1) {
                cout << (char)178 << (char)178;
            } else {
                cout << "  ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
    this_thread::sleep_for(chrono::milliseconds(delay)); // Pausa para visualizar el progreso
}

// Generar laberinto
void generarLaberinto(int filas, int columnas, vector<vector<int>>& matriz) {
    matriz.resize(filas, vector<int>(columnas, 1));     //se utiliza para ajustar el tamaño 
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = 1;
        }
    }
//rellena con 1 (paredes)
    srand(static_cast<unsigned int>(time(0))); //inicializa el creador de numeros aleatorios con time
                                                //convierte a un entero sin signo 
    stack<pair<int, int>> pila; 
    int x = 1, y = 1;
    matriz[x][y] = 0;
    pila.push({x, y}); //la matriz obtiene los datos de fila y columna
                        //y empuja la pila con los datos de fila y columna
/*BACKTRACKING */
    while (!pila.empty()) {         //algoritmo de backtracking con pila ejecutado hasta que no hayan elementos 
        int cx = pila.top().first;
        int cy = pila.top().second;
        pila.pop();

        int dir[] = { 0, 1, 2, 3 };         //INDICE DE DIRECCIONES 
        for (int i = 3; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(dir[i], dir[j]);
        }

        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[dir[i]] * 2;
            int ny = cy + dy[dir[i]] * 2;

            if (nx > 0 && nx < filas - 1 && ny > 0 && ny < columnas - 1 && matriz[nx][ny] == 1) {
                matriz[nx][ny] = 0;
                matriz[cx + dx[dir[i]]][cy + dy[dir[i]]] = 0;
                pila.push({nx, ny});
            }
        }
    }

    // Crear una entrada y salida claras
    matriz[1][0] = 0;  // Entrada
    matriz[filas - 2][columnas - 1] = 0;  // Salida
}

// Verificar si una celda es válida
bool esValido(const vector<vector<int>>& matriz, int x, int y, const vector<vector<int>>& solucion) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    return (x >= 0 && x < filas && y >= 0 && y < columnas && matriz[x][y] == 0 && solucion[x][y] == 0);
}

// Algoritmo de backtracking para resolver el laberinto
bool resolverLaberinto(vector<vector<int>>& matriz, int x, int y, vector<vector<int>>& solucion) {
    int filas = matriz.size();
    int columnas = matriz[0].size();

    if (x == filas - 2 && y == columnas - 1) { // Condición de salida ajustada
        solucion[x][y] = 1;
        imprimirLaberinto(matriz, solucion, 0); // Imprimir la solución final
        return true;
    }

    if (esValido(matriz, x, y, solucion)) {
        solucion[x][y] = 1;
        imprimirLaberinto(matriz, solucion); // Imprimir cada paso

        if (resolverLaberinto(matriz, x + 1, y, solucion)) {   //MOVER ABAJO 
            return true;
        }

        if (resolverLaberinto(matriz, x, y + 1, solucion)) {  //MOVER HACIA LA DERECHA 
            return true;
        }

        if (resolverLaberinto(matriz, x - 1, y, solucion)) { //MOVER HACIA ARRIBA 
            return true;
        }

        if (resolverLaberinto(matriz, x, y - 1, solucion)) { //mover hacia la izquierda.
            return true;
        }

        // Retroceso
        solucion[x][y] = 0;
        imprimirLaberinto(matriz, solucion); // Imprimir cada paso de retroceso
    }

    return false;
}

int main() { //el tamaño debe ser impar para una mejor ejecución 
    int filas = 25;
    int columnas = 25;

    vector<vector<int>> matriz;
    generarLaberinto(filas, columnas, matriz);

    vector<vector<int>> solucion(filas, vector<int>(columnas, 0));

    if (resolverLaberinto(matriz, 1, 0, solucion)) { // Comenzar desde la entrada ajustada
        cout << "Solucion encontrada:\n";
    } else {
        cout << "No se encontro solucion.\n";
    }

    system("pause");
    return 0;
}
