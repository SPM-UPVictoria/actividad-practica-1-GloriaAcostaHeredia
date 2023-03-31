#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Estructura para representar una arista
struct Edge {
    int source;
    int destination;
    int weight;
};

// Función para leer la lista de aristas desde un archivo
vector<Edge> leerAristas(string nombreArchivo) {
    vector<Edge> aristas;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            Edge e;
            stringstream ss(linea);
            ss >> e.source >> e.destination >> e.weight;
            aristas.push_back(e);
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }
    return aristas;
}

// Función para leer la lista de vértices desde un archivo
vector<int> leerVertices(string nombreArchivo) {
    vector<int> vertices;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            int v;
            stringstream ss(linea);
            ss >> v;
            vertices.push_back(v);
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }
    return vertices;
}

// Función para determinar el tipo de grafo
string determinarTipoGrafo(vector<Edge> aristas) {
    int numAristas = aristas.size();
    int numVertices = 0;
    for (Edge e : aristas) {
        numVertices = max(numVertices, max(e.source, e.destination));
    }

    bool esDirigido = true;
    bool esNoDirigido = true;
    bool esMultigrafo = false;
    for (int i = 0; i < numAristas; i++) {
        for (int j = i + 1; j < numAristas; j++) {
            if (aristas[i].source == aristas[j].source && aristas[i].destination == aristas[j].destination) {
                esMultigrafo = true;
            }
            if (aristas[i].source == aristas[j].destination && aristas[i].destination == aristas[j].source) {
                esDirigido = false;
            }
        }
    }

    string tipoGrafo = "";

    if (numAristas == 0 || numVertices == 0) {
        tipoGrafo = "Grafo vacio";
    } else if (numAristas == 1) {
        tipoGrafo = "Grafo trivial";
    } else if (esMultigrafo) {
        tipoGrafo = "Multi-grafo";
    } else if (esDirigido && esNoDirigido) {
        tipoGrafo = "Grafo mixto";
    } else if (esDirigido) {
        tipoGrafo = "Grafo dirigido";
    } else {
        tipoGrafo = "Grafo no dirigido";
    }

    return tipoGrafo;
}

// Función para determinar el grado de cada vértice
// Función para determinar el grado de cada vértice en un grafo
vector<int> determinarGrado(vector<Edge> aristas, int numVertices) {
    vector<int> grados(numVertices, 0);
    for (Edge e : aristas) {
        grados[e.source]++;
        grados[e.destination]++;
    }
    return grados;
}

// Función para determinar el grado de entrada de cada vértice en un grafo dirigido o mixto
vector<int> determinarGradoEntrada(vector<Edge> aristas, int numVertices) {
    vector<int> grados(numVertices, 0);
    for (Edge e : aristas) {
        grados[e.destination]++;
    }
    return grados;
}

// Función para determinar el grado de salida de cada vértice en un grafo dirigido o mixto
vector<int> determinarGradoSalida(vector<Edge> aristas, int numVertices) {
    vector<int> grados(numVertices, 0);
    for (Edge e : aristas) {
        grados[e.source]++;
    }
    return grados;
}

// Función para escribir la información del grafo en un archivo CSV
void escribirCSV(string nombreArchivo, string tipoGrafo, vector<int> grados, vector<int> gradosEntrada, vector<int> gradosSalida) {
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo << "Tipo de grafo," << tipoGrafo << endl;
        archivo << "Vertice,Grado,Grado de entrada,Grado de salida" << endl;
        int numVertices = grados.size();
        for (int i = 0; i < numVertices; i++) {
            archivo << i << "," << grados[i] << "," << gradosEntrada[i] << "," << gradosSalida[i] << endl;
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo " << nombreArchivo << " para escritura" << endl;
    }
}

int main() {
    // Leer la lista de aristas y de vertices desde los archivos correspondientes
    vector<Edge> aristas = leerAristas("aristas.txt");
    vector<int> vertices = leerVertices("vertices.txt");

    // Determinar el tipo de grafo
    string tipoGrafo = determinarTipoGrafo(aristas);

    // Determinar el grado de cada vértice
    vector<int> grados = determinarGrado(aristas, vertices.size());

    // Determinar el grado de entrada y de salida de cada vértice en caso de grafo dirigido o mixto
    vector<int> gradosEntrada;
    vector<int> gradosSalida;
    if (tipoGrafo == "Grafo dirigido" || tipoGrafo == "Grafo mixto") {
        gradosEntrada = determinarGradoEntrada(aristas, vertices.size());
        gradosSalida = determinarGradoSalida(aristas, vertices.size());
    } else {
        gradosEntrada = vector<int>(vertices.size(), 0);
        gradosSalida = vector<int>(vertices.size(), 0);
    }

    // Escribir la información del grafo en un archivo CSV
    escribirCSV("grafo.csv", tipoGrafo, grados, gradosEntrada, gradosSalida);

    return 0;
}
