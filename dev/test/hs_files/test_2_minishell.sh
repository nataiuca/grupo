#!/bin/bash


# Definir colores
BLUE='\033[1;34m'
RESET='\033[0m'

# Definir rutas
MINISHELL_PATH="./minishell"
COMMANDS_FILE="test/test_commands/command_list_test_2.txt"
OUTPUT_DIR="test/test_output"

# Crear directorio de salida si no existe
mkdir -p test/
mkdir -p test/test_files 
mkdir -p "$OUTPUT_DIR"

# Leer el archivo línea por línea
while IFS= read -r command; do
    # Imprimir el comando en azul
    echo -e "${BLUE}🔹 Testeando: $command${RESET}"

    # Ejecutar en minishell y guardar salida
    echo "$command" | "$MINISHELL_PATH" > "$OUTPUT_DIR/out_minishell.txt"


    # Comparar la salida con errores esperados
	if grep -qE "open quotes|open pipe" "$OUTPUT_DIR/out_minishell.txt"; then
		echo "✅ Test Passed!"
	else
		echo "❌ Test Failed!"
	fi


    echo "--------------------------------------"
done < "$COMMANDS_FILE"
