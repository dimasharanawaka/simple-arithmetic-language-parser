# Simple Arithmetic Language Parser

A C-based arithmetic language parser that performs lexical analysis, syntax analysis, semantic checking, and expression evaluation. The program reads source code from a file, validates its structure using a Context-Free Grammar (CFG), evaluates arithmetic expressions, and executes print statements.

Developed as an individual coursework project for **SCS 2312 – Computational Models and Programming Language Concepts** at the University of Colombo School of Computing (UCSC).

## Overview

This project demonstrates the fundamental concepts involved in processing a simple programming language. It follows a top-down parsing approach using a Recursive Descent Parser, with individual parsing functions corresponding to the grammar rules.

The implementation includes a tokenizer, syntax analyzer, symbol table, semantic checking, and arithmetic expression evaluation.

## Key Features

- **Lexical Analysis:** Reads source code character by character and converts it into tokens, identifying keywords, identifiers, integer literals, operators, and special symbols.
- **Syntax Analysis:** Implements a Recursive Descent Parser to validate source code according to a defined Context-Free Grammar (CFG).
- **Symbol Table Management:** Stores variable names and their corresponding integer values, allowing values to be retrieved during expression evaluation.
- **Semantic Checking:** Detects references to variables that have not been previously declared.
- **Expression Evaluation:** Evaluates integer expressions containing variables, numeric values, and addition operations.
- **Error Handling:** Identifies syntax errors and semantic errors during program execution.
- **Command-Line Execution:** Accepts a source-code file as a command-line argument for processing.

## Technologies & Concepts

- **Programming Language:** C
- **Compiler Concepts:** Lexical Analysis, Syntax Analysis, Semantic Analysis
- **Parsing Technique:** Recursive Descent Parsing
- **Formal Languages:** Context-Free Grammar (CFG)
- **Data Structures:** Symbol Table
- **Expression Evaluation:** Arithmetic Expression Parsing

## Context-Free Grammar (CFG)

The parser uses the following Context-Free Grammar to define the structure of the supported language.

```text
<Program>          → <StatementList>

<StatementList>    → <Statement> <StatementList>
                   | ε

<Statement>        → <Declaration>
                   | <PrintStatement>

<Declaration>      → int <identifier> = <Expression> ;

<PrintStatement>   → print ( <identifier> ) ;

<Expression>       → <Term> <Expression'>

<Expression'>      → + <Term> <Expression'>
                   | ε

<Term>             → <identifier>
                   | <number>
```

**Start Symbol:** `<Program>`

## How It Works

The program processes the input source file through the following stages:

### 1. Lexical Analysis (Tokenizer)

The `getNextToken()` function reads the source file character by character and identifies individual tokens.

The tokenizer categorises input into:

- Keywords: `int`, `print`
- Identifiers
- Integer numbers
- Operators: `+`, `=`
- Special symbols: `;`, `(`, `)`

### 2. Syntax Analysis (Recursive Descent Parser)

The parser follows a top-down parsing approach. Each major grammar rule is implemented as a corresponding C function.

| Grammar Component | C Function |
|---|---|
| Program | `Program()` |
| Statement List | `StatementList()` |
| Statement | `Statement()` |
| Declaration | `Declaration()` |
| Print Statement | `PrintStatement()` |
| Expression | `Expression()` |
| Term | `Term()` |

The `match()` function checks whether the current token matches the expected token type and advances to the next token. Unexpected tokens trigger syntax error handling.

### 3. Symbol Table & Semantic Checking

A symbol table maintains variable names and their corresponding integer values.

- `storeVariable()` stores variable declarations and their evaluated values.
- `getVariable()` retrieves the value associated with a variable.
- Undeclared variable references are identified as semantic errors.

### 4. Arithmetic Expression Evaluation

Expressions are evaluated during parsing.

The `Expression()` function processes terms and performs addition operations, while `Term()` handles integer literals and previously declared variables.

### 5. Error Handling

The program identifies syntax errors when the input does not follow the defined grammar and semantic errors when an undeclared variable is referenced.

## Example

### Input (`input.txt`)

```c
int y = 5;
int x = 20;
int z = x + y;
print(z);
```

### Execution

```bash
./parser input.txt
```

### Output

```text
25
```

## Project Structure

```text
simple-arithmetic-language-parser/
│
├── parser.c
├── input.txt
├── assignment-report.pdf
├── README.md
└── .gitignore
```

## Getting Started

### Prerequisites

- GCC compiler
- Terminal or command-line interface

### 1. Clone the Repository

```bash
git clone https://github.com/dimasharanawaka/simple-arithmetic-language-parser.git
```

### 2. Navigate to the Project Directory

```bash
cd simple-arithmetic-language-parser
```

### 3. Compile the Source Code

**Linux / macOS / WSL:**

```bash
gcc parser.c -o parser
```

**Windows (MinGW GCC):**

```bash
gcc parser.c -o parser.exe
```

### 4. Run the Program

**Linux / macOS / WSL:**

```bash
./parser input.txt
```

**Windows:**

```powershell
.\parser.exe input.txt
```

The program accepts the source file path as its command-line argument.

 
