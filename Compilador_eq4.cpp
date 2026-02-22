#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cctype>
#include <string>
#include <iomanip>

using namespace std;

enum class TokenType {
    PALABRA_RESERVADA, IDENTIFICADOR, ENTERO,
    DECIMAL, CADENA, OPERADOR, SIMBOLO, ERROR
};

struct Token {
    int numero;                 // <-- Token #
    string lexema, valor;
    TokenType tipo;
    int linea, columna;
    Token* siguiente;
};

Token* cabeza = nullptr;
Token* cola = nullptr;
int contadorTokens = 0;

void insertarToken(const string& lex, TokenType tipo, const string& val, int lin, int col) {
    Token* nuevo = new Token{++contadorTokens, lex, val, tipo, lin, col, nullptr};
    if (!cabeza) cabeza = cola = nuevo;
    else {
        cola->siguiente = nuevo;
        cola = nuevo;
    }
}

bool esDecimal(const string& s) {
    bool punto = false;
    if (s.empty()) return false;
    for (char c : s) {
        if (c == '.') {
            if (punto) return false;
            punto = true;
        } else if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return punto; // true si tiene un punto
}

string nombreTipo(TokenType t) {
    switch (t) {
        case TokenType::PALABRA_RESERVADA: return "RESERVADA";
        case TokenType::IDENTIFICADOR:     return "ID";
        case TokenType::ENTERO:            return "ENTERO";
        case TokenType::DECIMAL:           return "DECIMAL";
        case TokenType::CADENA:            return "CADENA";
        case TokenType::OPERADOR:          return "OPERADOR";
        case TokenType::SIMBOLO:           return "SIMBOLO";
        case TokenType::ERROR:             return "ERROR";
        default:                           return "??";
    }
}

bool esSimboloPermitido(char c) {
    switch (c) {
        case ';': case ',': case '(': case ')': case '{': case '}':
            return true;
        default:
            return false;
    }
}

bool esOperadorPermitido(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': case '=': case '<': case '>':
            return true;
        default:
            return false;
    }
}

int main() {
    ifstream archivo("entrada.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo\n";
        return 1;
    }

    unordered_map<string,string> reservadas = {
    {"Inicio","Inicio"}, {"Fin","Fin"}, {"si","Condicional"},
    {"sino","Alternativa"}, {"mientras","Bucle"},
    {"Para","Bucle"}, {"Funcion","Función"},
    {"Dec","Tipo"}, {"Imprimir","Salida"}
};

    char c;
    string buffer;
    int linea = 1, columna = 0;
    bool hayErrores = false;

    while (archivo.get(c)) {
        columna++;

        if (c == '\n') { linea++; columna = 0; continue; }
        if (isspace(static_cast<unsigned char>(c))) continue;

        buffer.clear();
        int colInicio = columna;

        if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
            buffer += c;
            while (archivo.peek() != EOF &&
                   (isalnum(static_cast<unsigned char>(archivo.peek())) || archivo.peek() == '_')) {
                archivo.get(c);
                buffer += c;
                columna++;
            }

            if (reservadas.count(buffer))
                insertarToken(buffer, TokenType::PALABRA_RESERVADA, reservadas[buffer], linea, colInicio);
            else
                insertarToken(buffer, TokenType::IDENTIFICADOR, "-", linea, colInicio);
        }
        else if (isdigit(static_cast<unsigned char>(c))) {
            buffer += c;
            bool vioPunto = false;

            while (archivo.peek() != EOF &&
                  (isdigit(static_cast<unsigned char>(archivo.peek())) || archivo.peek() == '.')) {

                if (archivo.peek() == '.') {
                    if (vioPunto) break;
                    vioPunto = true;
                }

                archivo.get(c);
                buffer += c;
                columna++;
            }

            if (vioPunto) {
                if (esDecimal(buffer))
                    insertarToken(buffer, TokenType::DECIMAL, buffer, linea, colInicio);
                else {
                    insertarToken(buffer, TokenType::ERROR, "Decimal mal formado", linea, colInicio);
                    hayErrores = true;
                }
            } else {
                insertarToken(buffer, TokenType::ENTERO, buffer, linea, colInicio);
            }
        }
        else if (c == '"') {
            buffer += c;
            bool cerrada = false;

            while (archivo.get(c)) {
                columna++;
                buffer += c;
                if (c == '"') { cerrada = true; break; }
                if (c == '\n') {
                    linea++; columna = 0;
                }
            }

            if (cerrada) {
                insertarToken(buffer, TokenType::CADENA, buffer, linea, colInicio);
            } else {
                insertarToken(buffer, TokenType::ERROR, "Cadena sin cerrar", linea, colInicio);
                hayErrores = true;
            }
        }
        else if (esOperadorPermitido(c)) {
            string s(1, c);
            insertarToken(s, TokenType::OPERADOR, "-", linea, colInicio);
        }
        else if (esSimboloPermitido(c)) {
            string s(1, c);
            insertarToken(s, TokenType::SIMBOLO, "-", linea, colInicio);
        }
        else {
            string s(1, c);
            insertarToken(s, TokenType::ERROR, "Símbolo no válido", linea, colInicio);
            hayErrores = true;
        }
    }

    // ======= AQUÍ ESTÁ LA ÚNICA CORRECCIÓN (IMPRESIÓN) =======

    if (hayErrores) {
        // Solo mostrar errores (sin tabla)
        cout << "\n===== ERRORES LÉXICOS DETECTADOS =====\n";
        Token* aux = cabeza;
        while (aux) {
            if (aux->tipo == TokenType::ERROR) {
                cout << "ERROR LÉXICO (Token #" << aux->numero << "): "
                     << aux->valor << " -> '" << aux->lexema
                     << "' en fila " << aux->linea
                     << ", columna " << aux->columna << "\n";
            }
            aux = aux->siguiente;
        }
    } else {
        // Mostrar tabla completa solo si no hay errores
        cout << left
             << setw(8)  << "Token#"
             << setw(15) << "Lexico"
             << setw(18) << "Valor"
             << setw(12) << "Tipo"
             << setw(10) << "Fila"
             << setw(10) << "Col"
             << "\n";
        cout << string(73, '-') << "\n";

        Token* aux = cabeza;
        while (aux) {
            cout << left
                 << setw(8)  << aux->numero
                 << setw(15) << aux->lexema
                 << setw(18) << aux->valor
                 << setw(12) << nombreTipo(aux->tipo)
                 << setw(10) << aux->linea
                 << setw(10) << aux->columna
                 << "\n";
            aux = aux->siguiente;
        }

        cout << "\nNo se detectaron errores léxicos.\n";
    }

    // liberar memoria
    Token* aux = cabeza;
    while (aux) {
        Token* tmp = aux->siguiente;
        delete aux;
        aux = tmp;
    }

    return 0;
}