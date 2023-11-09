#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLUMNAS 3100
#define FILAS 51
#define BUFFER 1024 * 1024

int main(){
    const char *archivoCsv = "emails.csv";
    FILE *archCsv = fopen(archivoCsv, "r");
    if (!archCsv) {
        perror("Error al abrir el archivo CSV");
        return 1;
    }
    char buffer[BUFFER];
    int numPalabras[COLUMNAS] = {0};
    char *nomPalabras[COLUMNAS];
    int empezarConteo;
    int saltoPrimerColumna = -1;
    
    printf("Ingrese un numero de cuenta: ");
    scanf("%d", &empezarConteo);
    empezarConteo %= 1000;
    empezarConteo -= 1;
    
    if (fgets(buffer, sizeof(buffer), archCsv) != NULL) {
        char *token = strtok(buffer, ",");
        int indColumna = 0;
        while (token && indColumna < COLUMNAS) {
            if (strcmp(token, "Email No.") == 0) {
                saltoPrimerColumna = indColumna;
            } else {
                nomPalabras[indColumna] = strdup(token);
            }
            token = strtok(NULL, ",");
            indColumna++;
        }
    } else {
        fclose(archCsv);
        return 1;
    }
    for (int i = 0; i < empezarConteo; ++i) {
        if (fgets(buffer, sizeof(buffer), archCsv) == NULL) {
            fclose(archCsv);
            return 1;
        }
    }
    for (int i = 0; i < FILAS; ++i) {
        if (fgets(buffer, sizeof(buffer), archCsv) == NULL) {
            break;
        }
        char *token = strtok(buffer, ",");
        int indColumna = 0;
        while (token && indColumna < COLUMNAS) {
            if (indColumna != saltoPrimerColumna) {
                long val = strtol(token, NULL, 10);
                if (val != 0 || (val == 0 && token[0] == '0')) {
                    numPalabras[indColumna] += val;
                }
            }
            token = strtok(NULL, ",");
            indColumna++;
        }
    }
    fclose(archCsv);
    
    FILE *archivo_salida = fopen("Resultado_emails.txt", "w");
    if (!archivo_salida) {
        perror("Error al abrir 'Resultado_emails.txt'");
        return 1;
    }
    for (int i = 0; i < COLUMNAS; i++) {
        if (i != saltoPrimerColumna && nomPalabras[i] != NULL) {
            fprintf(archivo_salida, "La palabra'%s'\t\tse repite %d veces\n", nomPalabras[i], numPalabras[i]);
            free(nomPalabras[i]);
        }
    }
    fclose(archivo_salida);
    printf("El conteo de las palabras fueron transferidos a 'Resultado_emails.txt'\n");
    return 0;
}

