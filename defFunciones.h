#ifndef defFunciones_h
#define defFunciones_h 
#include "list.h"

typedef struct{          // Corresponde a la estructura de cada lista
    int cantidadCanciones;       
    List* ListaReprod;
    char* NombreList;
} Reproduccion;

typedef struct{         // Estructura que guarda la infromación de cada canción
    char* Nombre;
    char* Artista;
    char* Genero;
    char* year;
    char* NombreLista;
} Cancion;

typedef struct{          // Es la estructura que guarda cada lista creada.
    int cantidadListas;   // cantidad de listas guardadas
    List *ListaGeneral;   // Guarda las listas de reproducción creadas
    List *ListaCanciones; // Cada posición contiene la información de una lista
}   Biblioteca;

void ImprimirMenu(Biblioteca *biblioteca, FILE *archivo);
void importar(FILE *archivo, Biblioteca* biblioteca);
void mostrarCanciones(List* listaCanciones);
void mostrarReproduccion(Biblioteca* biblioteca);
Reproduccion* existeReproduccion(Biblioteca* biblioteca, char* nombreList);
Biblioteca *crearBiblioteca();
void AgregarCancion(Biblioteca *biblioteca);
void imprimirCancion(Cancion * lista);
void BuscarPorNombre(List *ListaCanciones);
void Buscar_artista(List* ListaCanciones);
void BuscarPorGenero(List *canciones);
void EliminarCancion(Biblioteca *biblioteca);
int cancionExiste(char *c_buscada, List *listaCanciones);
Cancion *crearCancion();
void LlenarLista(Biblioteca *listGeneral, char *linea);
void guardarGenero(Biblioteca *listGeneral, const char *atributo);
void guardarLista(Biblioteca *listGeneral, const char *atributo);
void obtenerGeneros(List * genero, const char *dato);
void mostrarLista(Biblioteca *biblioteca);
const char *get_csv_field (char * tmp, int k);

#endif