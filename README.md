# Analizador Léxico - Software de Sistemas

Este repositorio contiene la implementación de un **Analizador Léxico** (Lexer) desarrollado en C++. El proyecto es parte de la asignatura de Software de Sistemas de la carrera de Ingeniería de Software en la Universidad Autónoma de Sinaloa.

##  Información del Proyecto

* **Universidad:** Universidad Autónoma de Sinaloa (UAS)
* **Facultad:** Ingeniería de Software
* **Materia:** Software de Sistemas
* **Profesor:** M.C. Edgar Omar Pérez Contreras
* **Grupo:** 3-02
* **Actividad:** Especificación y Diseño Léxico

### Autores
* **Laurean Acosta Luis Donaldo**
* **Liera Payan Diego**
* **Rosas Castro Manuel Ernesto**

---

## Descripción General

El objetivo de este analizador es leer un archivo de texto fuente y convertir la secuencia de caracteres en una secuencia de **Tokens**. El lexer valida las reglas léxicas definidas para el lenguaje, ignorando espacios en blanco y tabuladores (excepto en cadenas), y contando saltos de línea para el reporte de errores.

El analizador está basado en **Autómatas Finitos Deterministas (AFD)** para el reconocimiento de patrones complejos como identificadores, números y cadenas.

## Especificación del Lenguaje

El analizador reconoce los siguientes componentes léxicos:

### 1. Palabras Reservadas
Estas palabras definen la estructura y control del programa:
* **Estructura:** `Inicio`, `Fin`, `Funcion`, `Retornar`
* **Control de Flujo:** `si`, `sino`, `mientras`, `Para`, `Romper`, `Continuar`
* **Tipos de Datos y Valores:** `ent`, `Dec`, `Cadena`, `Verdadero`, `Falso`
* **E/S y Utilerías:** `Imprimir`, `Leer`, `LimpiarPantalla`

### 2. Identificadores (Variables y Funciones)
* **Regla:** Inician con letra (A-Z, a-z) o guion bajo (`_`), seguidos de letras, dígitos o guiones bajos.
* **Expresión Regular:** `[A-Za-z_][A-Za-z0-9_]*`

### 3. Literales (Valores)
* **Enteros:** Secuencia de dígitos, opcionalmente con signo (`+` o `-`).
* **Decimales:** Soporta parte fraccionaria y notación científica (ej. `1.5`, `2.4E-3`).
* **Cadenas:** Texto delimitado por comillas dobles (`"`). Soporta cualquier carácter excepto salto de línea sin escapar.

### 4. Operadores y Símbolos
* **Aritméticos:** `+`, `-`, `*`, `/`, `%`
* **Relacionales:** `==`, `!=`, `<`, `<=`, `>`, `>=`
* **Agrupación:** `(`, `)`, `{`, `}`, `[`, `]`
* **Puntuación:** `;`, `,`

### 5. Comentarios
El analizador ignora el texto contenido en:
* Comentarios de línea: `// ...`
* Comentarios de bloque: `/* ... */`

---

## Estructura del Código

El proyecto está estructurado en C++ utilizando las siguientes definiciones clave:

### Estructura del Token
Cada token identificado se almacena en una estructura que contiene:
* `TokenType`: Enum que clasifica el token (ej. `KW_INICIO`, `LIT_ENTERO`).
* `Lexema`: El texto exacto extraído del archivo fuente.
* `Línea`: Número de línea donde se encontró (útil para depuración).

### Lógica de Reconocimiento
Se implementa una máquina de estados para transitar carácter por carácter, identificando si la entrada corresponde a un operador simple, una palabra reservada o si se debe entrar a una sub-rutina de autómata (para números o cadenas).

---

## Compilación y Ejecución

Para compilar el proyecto, asegúrate de tener un compilador de C++ (como `g++`) instalado.

```bash
# Compilar el proyecto
g++ main.cpp -o lexer

# Ejecutar con un archivo de prueba
./lexer prueba.txt
