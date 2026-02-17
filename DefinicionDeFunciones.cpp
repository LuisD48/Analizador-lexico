#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

//  Función 1: preprocesarArchivo
void preprocesarArchivo(string nombreEntrada, string nombreSalida) {
    ifstream archivoLectura(nombreEntrada);
    ofstream archivoEscritura(nombreSalida);

    //  Validación de existencia
    if (!archivoLectura.is_open()) {
        cerr << "Error: El archivo de entrada '" << nombreEntrada << "' no existe o no se pudo abrir." << endl;
        return;
    }

    string linea;
    bool enComentarioBloque = false;

    while (getline(archivoLectura, linea)) {
        string lineaProcesada = "";
        
        for (size_t i = 0; i < linea.length(); ++i) {
            //  Manejo de comentarios de una sola línea (//)
            if (linea[i] == '/' && i + 1 < linea.length() && linea[i + 1] == '/') {
                break; 
            }

            // Filtrar espacios, tabuladores y saltos de línea
            if (!isspace(linea[i])) {
                lineaProcesada += linea[i];
            }
        }

        //  Solo escribir si la línea resultante no está vacía
        if (!lineaProcesada.empty()) {
            archivoEscritura << lineaProcesada << endl;
        }
    }

    archivoLectura.close();
    archivoEscritura.close();
    cout << "Archivo preprocesado exitosamente en: " << nombreSalida << endl;
}

//  Función 2: recorrerArchivo
void recorrerArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    char caracter;

    if (!archivo.is_open()) {
        cerr << "Error: El archivo '" << nombreArchivo << "' no existe." << endl;
        return;
    }

    cout << "\nContenido limpio (sin saltos de linea):" << endl;
    while (archivo.get(caracter)) {
        // Esta condicion evita que los saltos de linea (\n o \r) se impriman
        if (caracter != '\n' && caracter != '\r') {
            cout << "[" << caracter << "]";
        }
    }
    cout << endl;

    archivo.close();
}

int main() {
    string entrada = "codigo_fuente.txt";
    string salida = "archivo_limpio.txt";

    //  Llamada a las funciones
    preprocesarArchivo(entrada, salida);
    recorrerArchivo(salida);


    return 0;
}