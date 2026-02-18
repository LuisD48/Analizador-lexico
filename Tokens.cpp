#include <string>
#include <iostream>
#include <unordered_map>

// Definición de todos los posibles tokens según la especificación
enum class TokenType {
    // Palabras reservadas (KW = KeyWord)
    KW_INICIO,          // "Inicio"
    KW_FIN,             // "Fin"
    KW_SI,              // "si"
    KW_SINO,            // "sino"
    KW_MIENTRAS,        // "mientras"
    KW_PARA,            // "Para"
    KW_ROMPER,          // "Romper"
    KW_CONTINUAR,       // "Continuar"
    KW_FUNCION,         // "Funcion"
    KW_RETORNAR,        // "Retornar"
    KW_ENT,             // "ent" (Tipo entero)
    KW_DEC,             // "Dec" (Tipo decimal)
    KW_CADENA_TYPE,     // "Cadena" (Tipo de dato string)
    KW_VERDADERO,       // "Verdadero"
    KW_FALSO,           // "Falso"
    KW_IMPRIMIR,        // "Imprimir"
    KW_LEER,            // "Leer"
    KW_LIMPIAR_PANTALLA,// "LimpiarPantalla"

    //  Operadores y simbolos (OP = OPerator)
    OP_SUMA,            // "+"
    OP_RESTA,           // "-" (También usado para números negativos)
    OP_MULT,            // "*"
    OP_DIV,             // "/"
    OP_ASIGNACION,      // "="
    OP_IGUALDAD,        // "=="
    OP_DIFERENTE,       // "!="
    OP_MENOR,           // "<"
    OP_MENOR_IGUAL,     // "<="
    OP_MAYOR,           // ">"
    OP_MAYOR_IGUAL,     // ">="
    
    // Separadores (SEP =  SEParator)
    SEP_PARENTESIS_A,   // "("
    SEP_PARENTESIS_C,   // ")"
    SEP_LLAVE_A,        // "{"
    SEP_LLAVE_C,        // "}"
    SEP_CORCHETE_A,     // "["
    SEP_CORCHETE_C,     // "]"
    SEP_PUNTO_COMA,     // ";"
    SEP_COMA,           // "," (Mencionado como separador de argumentos)
    
    // Valores totales (LIT = LITeral)
    ID,                 // Identificadores (Variables, nombres de funciones) [cite: 32]
    LIT_ENTERO,         // Números enteros (ej. 123, -5) [cite: 66]
    LIT_DECIMAL,        // Números decimales (ej. 12.34, 1.2E+5) [cite: 77]
    LIT_CADENA,         // Cadenas de texto entre comillas (ej. "Hola") [cite: 47]

    //  Control
    COMENTARIO,         // Para manejo interno (// o /* */) 
    FIN_DE_ARCHIVO,     // EOF
    ERROR               // Token no reconocido
};

struct Token {
    TokenType tipo;      // El tipo de token (del enum de arriba)
    std::string lexema;  // El texto original (ej. "contador", "15.5", "Inicio")
    int linea;           // Número de línea para reportar errores
    int columna;         // Opcional: útil para mensajes de error precisos

    // Constructor para facilitar la creación de tokens
    Token(TokenType t, std::string l, int lin, int col = 0) 
        : tipo(t), lexema(l), linea(lin), columna(col) {}
        
    // Función auxiliar para imprimir el token (útil para depurar)
    std::string toString() const {
        return "Token[Type: " + std::to_string(static_cast<int>(tipo)) + 
               ", Lexema: '" + lexema + "', Linea: " + std::to_string(linea) + "]";
    }
};


class TokenHelper {
public:
    // Mapa estático para buscar palabras reservadas rápidamente
    static TokenType obtenerPalabraReservada(const std::string& lexema) {
        static const std::unordered_map<std::string, TokenType> palabrasReservadas = {
            {"Inicio", TokenType::KW_INICIO},
            {"Fin", TokenType::KW_FIN},
            {"si", TokenType::KW_SI},
            {"sino", TokenType::KW_SINO},
            {"mientras", TokenType::KW_MIENTRAS},
            {"Para", TokenType::KW_PARA},
            {"Romper", TokenType::KW_ROMPER},
            {"Continuar", TokenType::KW_CONTINUAR},
            {"Funcion", TokenType::KW_FUNCION},
            {"Retornar", TokenType::KW_RETORNAR},
            {"ent", TokenType::KW_ENT},
            {"Dec", TokenType::KW_DEC},
            {"Cadena", TokenType::KW_CADENA_TYPE},
            {"Verdadero", TokenType::KW_VERDADERO},
            {"Falso", TokenType::KW_FALSO},
            {"Imprimir", TokenType::KW_IMPRIMIR},
            {"Leer", TokenType::KW_LEER},
            {"LimpiarPantalla", TokenType::KW_LIMPIAR_PANTALLA}
        };

        auto it = palabrasReservadas.find(lexema);
        if (it != palabrasReservadas.end()) {
            return it->second;
        }
        // Si no está en la lista, es un Identificador normal
        return TokenType::ID; 
    }
};
