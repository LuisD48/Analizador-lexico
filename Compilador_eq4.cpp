#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// TIPOS DE TOKEN (ENTIDADES LÓGICAS)
enum class TokenType {
    INICIO, FIN, SI, SINO, MIENTRAS, PARA, ROMPER, CONTINUAR,
    FUNCION, RETORNAR, ENT, DEC, CADENA_TIPO,
    VERDADERO, FALSO, IMPRIMIR, LEER, LIMPIAR_PANTALLA,

    OP_SUMA, OP_RESTA, OP_MULT, OP_DIV,
    OP_ASIGNACION, OP_MENOR, OP_MAYOR,

    SEP_PARENTESIS_A, SEP_PARENTESIS_C,
    SEP_LLAVE_A, SEP_LLAVE_C,
    SEP_PUNTO_COMA, SEP_COMA,

    ID, LIT_ENTERO, LIT_DECIMAL, LIT_CADENA,
    FIN_DE_ARCHIVO, ERROR
};

// TOKEN
struct Token {
    TokenType tipo;
    string lexema;
    int linea;

    Token(TokenType t, string l, int lin)
        : tipo(t), lexema(l), linea(lin) {}
};

// PALABRAS RESERVADAS
class TokenHelper {
public:
    static TokenType obtenerPalabraReservada(const string& lexema) {
        static const unordered_map<string, TokenType> palabras = {
            {"Inicio", TokenType::INICIO},
            {"Fin", TokenType::FIN},
            {"si", TokenType::SI},
            {"sino", TokenType::SINO},
            {"mientras", TokenType::MIENTRAS},
            {"Para", TokenType::PARA},
            {"Romper", TokenType::ROMPER},
            {"Continuar", TokenType::CONTINUAR},
            {"Funcion", TokenType::FUNCION},
            {"Retornar", TokenType::RETORNAR},
            {"ent", TokenType::ENT},
            {"Dec", TokenType::DEC},
            {"Cadena", TokenType::CADENA_TIPO},
            {"Verdadero", TokenType::VERDADERO},
            {"Falso", TokenType::FALSO},
            {"Imprimir", TokenType::IMPRIMIR},
            {"Leer", TokenType::LEER},
            {"LimpiarPantalla", TokenType::LIMPIAR_PANTALLA}
        };

        auto it = palabras.find(lexema);
        return (it != palabras.end()) ? it->second : TokenType::ID;
    }
};

// ANALIZADOR LÉXICO
class Lexer {
private:
    string fuente;
    size_t pos = 0;
    int linea = 1;

    char actual() {
        return pos < fuente.size() ? fuente[pos] : '\0';
    }

    void avanzar() {
        if (actual() == '\n') linea++;
        pos++;
    }

public:
    Lexer(const string& texto) : fuente(texto) {}

    vector<Token> analizar() {
        vector<Token> tokens;

        while (actual() != '\0') {

            if (isspace(actual())) {
                avanzar();
                continue;
            }

            //Identificadores y palabras reservadas
            if (isalpha(actual())) {
                string lexema;
                while (isalnum(actual())) {
                    lexema += actual();
                    avanzar();
                }
                tokens.emplace_back(
                    TokenHelper::obtenerPalabraReservada(lexema),
                    lexema,
                    linea
                );
                continue;
            }

            // Números
            if (isdigit(actual())) {
                string numero;
                bool decimal = false;

                while (isdigit(actual()) || actual() == '.') {
                    if (actual() == '.') decimal = true;
                    numero += actual();
                    avanzar();
                }

                tokens.emplace_back(
                    decimal ? TokenType::LIT_DECIMAL : TokenType::LIT_ENTERO,
                    numero,
                    linea
                );
                continue;
            }

            // Cadenas
            if (actual() == '"') {
                avanzar();
                string cadena;

                while (actual() != '"' && actual() != '\0') {
                    cadena += actual();
                    avanzar();
                }

                if (actual() == '"') {
                    avanzar();
                    tokens.emplace_back(TokenType::LIT_CADENA, cadena, linea);
                } else {
                    tokens.emplace_back(TokenType::ERROR, cadena, linea);
                }
                continue;
            }

            // Símbolos especiales
            switch (actual()) {
                case '+': tokens.emplace_back(TokenType::OP_SUMA, "+", linea); break;
                case '-': tokens.emplace_back(TokenType::OP_RESTA, "-", linea); break;
                case '*': tokens.emplace_back(TokenType::OP_MULT, "*", linea); break;
                case '/': tokens.emplace_back(TokenType::OP_DIV, "/", linea); break;
                case '=': tokens.emplace_back(TokenType::OP_ASIGNACION, "=", linea); break;
                case '<': tokens.emplace_back(TokenType::OP_MENOR, "<", linea); break;
                case '>': tokens.emplace_back(TokenType::OP_MAYOR, ">", linea); break;
                case ';': tokens.emplace_back(TokenType::SEP_PUNTO_COMA, ";", linea); break;
                case '(': tokens.emplace_back(TokenType::SEP_PARENTESIS_A, "(", linea); break;
                case ')': tokens.emplace_back(TokenType::SEP_PARENTESIS_C, ")", linea); break;
                case '{': tokens.emplace_back(TokenType::SEP_LLAVE_A, "{", linea); break;
                case '}': tokens.emplace_back(TokenType::SEP_LLAVE_C, "}", linea); break;
                case ',': tokens.emplace_back(TokenType::SEP_COMA, ",", linea); break;
                default:
                    tokens.emplace_back(TokenType::ERROR, string(1, actual()), linea);
            }
            avanzar();
        }

        tokens.emplace_back(TokenType::FIN_DE_ARCHIVO, "EOF", linea);
        return tokens;
    }
};

// CLASIFICACIÓN

string clasificarToken(TokenType tipo) {
    switch (tipo) {
        case TokenType::ID: return "Identificador";
        case TokenType::LIT_ENTERO:
        case TokenType::LIT_DECIMAL: return "Número";
        case TokenType::OP_SUMA:
        case TokenType::OP_RESTA:
        case TokenType::OP_MULT:
        case TokenType::OP_DIV:
        case TokenType::OP_ASIGNACION:
        case TokenType::OP_MENOR:
        case TokenType::OP_MAYOR:
        case TokenType::SEP_PARENTESIS_A:
        case TokenType::SEP_PARENTESIS_C:
        case TokenType::SEP_LLAVE_A:
        case TokenType::SEP_LLAVE_C:
        case TokenType::SEP_PUNTO_COMA:
        case TokenType::SEP_COMA:
            return "Simbolo especial";
        case TokenType::ERROR:
            return "ERROR LÉXICO";
        default:
            return "Palabra reservada";
    }
}

// MAIN

int main() {

    ifstream archivo("entrada.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    // Mostrar contenido del archivo
    string contenido(
        (istreambuf_iterator<char>(archivo)),
        istreambuf_iterator<char>()
    );

    cout << "===== CONTENIDO DEL ARCHIVO =====\n";
    cout << contenido << endl;
    cout << "=================================\n\n";

    Lexer lexer(contenido);
    vector<Token> tokens = lexer.analizar();

    bool hayErrores = false;

    for (const Token& t : tokens) {
        cout << "Linea " << t.linea
             << " | " << clasificarToken(t.tipo)
             << " | Lexema: \"" << t.lexema << "\""
             << endl;

        if (t.tipo == TokenType::ERROR) {
            hayErrores = true;
        }
    }

    cout << "\n===== RESULTADO DEL ANÁLISIS LÉXICO =====\n";
    if (hayErrores) {
        cout << "Se detectaron ERRORES LÉXICOS en el archivo.\n";
    } else {
        cout << "No se detectaron errores léxicos.\n";
    }

    return 0;
}
