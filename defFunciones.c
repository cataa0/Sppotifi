#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "defFunciones.h"

const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char *ret = (char *) malloc (150 * sizeof(char));
    int ini_i = 0, i = 0, j = 0;
    while(tmp[i+1] != '\0'){

        if(tmp[i] == '\"'){
            open_mark = 1 - open_mark;
            if(open_mark) ini_i = i + 1;
            i++;
            continue;
        }

        if(open_mark || tmp[i] != ','){
            if(k == j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i] == ','){
            if(k == j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i + 1;
        }
        i++;
    }

    if(k == j){

        i = i - 1;
        for (i; tmp[i] != '\0'; i++)
        {
            ret[i - ini_i] =  tmp[i];
        }
       ret[i - ini_i] = 0;
       return ret;
    }
    return NULL;
}

Biblioteca* crearBiblioteca()
{
    Biblioteca* biblioteca = (Biblioteca*)malloc(sizeof(Biblioteca));
    biblioteca->cantidadListas = 0;
    biblioteca->ListaCanciones = createList();
    biblioteca->ListaGeneral = createList();
    return biblioteca;
}

void imprimirCancion(Cancion* a){
    printf("------------------------------------------------\n");

    printf("Nombre : %s \n", a->Nombre);
    printf("Artista : %s \n", a->Artista);
    printf("Genero : %s \n", a->Genero);
    printf("Anyo : %s \n", a->year);
    printf("Lista : %s \n", a->NombreLista);

    return;
}

void ImprimirMenu(Biblioteca *biblioteca, FILE *archivo)
{
    int op, op2;
    do{
        printf("\nopcion 1. Importar\n");
        printf("opcion 2. Agregar Cancion\n");
        printf("opcion 3. Buscar Cancion\n");
        printf("opcion 4. Eliminar Cancion\n");
        printf("opcion 5. Mostrar Nombres Listas\n");
        printf("opcion 6. Mostrar Lista\n");
        printf("opcion 7. Mostrar todas las Canciones\n");
        printf("opcion 8. Exportar\n");
        printf("opcion 9. Salir\n");
        scanf("%d", &op);

        while(op < 1 || op > 9)
        {
            printf("ingrese una opcion valida\n");
            scanf("%d", &op);
        }

        switch(op)
        {
            case 1:
                importar(archivo, biblioteca);
                break;
            case 2:
                AgregarCancion(biblioteca);
                break; 
            case 3:
                printf("opcion 1. Nombre\n");
                printf("opcion 2. Artista\n");
                printf("opcion 3. Genero\n");
                scanf("%d", &op2);
                switch(op2)
                {
                    case 1:
                        BuscarPorNombre(biblioteca -> ListaCanciones);
                        break;
                    case 2:
                        Buscar_artista(biblioteca -> ListaCanciones);
                        break;
                    case 3:
                        BuscarPorGenero(biblioteca -> ListaCanciones);
                        break;
                }
                break;
            case 4:
                EliminarCancion(biblioteca);
                break;
            case 5:
                mostrarLista(biblioteca);
                break;
            case 6:
                mostrarReproduccion(biblioteca);
                break;
            case 7:
                mostrarCanciones(biblioteca -> ListaCanciones);
                break;
            case 8:
                printf("Aun no se programa\n");
                break;
            case 9:
                break;
            printf("\n\n");
        }

    } while(op > 0 && op < 9);
}


void importar(FILE *archivo, Biblioteca* biblioteca)
{
    int i;
    char linea[1024];

    while (fgets(linea, 1024, archivo) != NULL)
    {
        linea[strcspn(linea, "\n")] = 0;
        Cancion* cancion = (Cancion *) malloc(sizeof(Cancion));
        cancion->Artista = (char*) malloc(sizeof(char) * 35);
        cancion->Genero = (char*) malloc(sizeof(char) * 35);
        cancion->Nombre = (char*) malloc(sizeof(char) * 35);
        cancion->year = (char*) malloc(sizeof(char) * 4);
        cancion->NombreLista = (char*) malloc(sizeof(char) * 35);

        for (int i = 0; i < 5; i++)
        {
            const char* aux = get_csv_field(linea, i);

            switch (i)
            {
                case 0:
                    strcpy(cancion->Nombre, aux);
                    break;
                case 1:
                    strcpy(cancion->Artista, aux); 
                    break;
                case 2:
                    strcpy(cancion->Genero, aux);
                    break;
                case 3:
                    strcpy(cancion->year, aux);
                    break;
                case 4:
                    strcpy(cancion->NombreLista, aux);
                    break;
            }
        }

        pushBack(biblioteca->ListaCanciones, cancion);

        Reproduccion* reproAux = existeReproduccion(biblioteca, cancion->NombreLista);
        if (reproAux != NULL)
        {           
            reproAux->cantidadCanciones += 1;
            pushFront(reproAux->ListaReprod, cancion);
        }
        else
        {
            Reproduccion* repro = (Reproduccion *) malloc (sizeof(Reproduccion));
            repro->cantidadCanciones = 1;
            repro->ListaReprod = createList();
            repro->NombreList = (char*)malloc(sizeof(char) * 35);
            strcpy(repro->NombreList, cancion->NombreLista);
            pushFront(repro->ListaReprod, cancion);

            pushFront(biblioteca->ListaGeneral, repro);
        }
    }
    
    fclose(archivo);
    printf("Archivo importado!\n\n");
}

void AgregarCancion(Biblioteca *biblioteca)
{
    Cancion *c_ingresada = crearCancion();

    // ingreso
    printf("Introduzca el nombre de la cancion: \n");
    getchar();
    scanf("%[^\n]s", c_ingresada->Nombre);
    if (cancionExiste(c_ingresada->Nombre, biblioteca->ListaCanciones) == 0)
    {

        printf("Introduzca el artista o banda : \n");
        getchar();
        scanf("%[^\n]s", c_ingresada->Artista);

        printf("Introduzca el/los genero/s : \n");
        getchar();
        scanf("%[^\n]s", c_ingresada->Genero);

        printf("Introduzca el año de creacion : \n");
        getchar();
        scanf("%[^\n]s", c_ingresada->year);

        printf("¿A que lista le gustaria agregar esta cancion?\n");
        getchar();
        scanf("%[^\n]s", c_ingresada->NombreLista);

        // ingreso a lista de canciones
        pushFront(biblioteca->ListaCanciones, c_ingresada);
        // ingreso a su respectiva lista
        Reproduccion *listaAux = existeReproduccion(biblioteca, c_ingresada->NombreLista);
        if (listaAux != NULL)
        { // si existe la lista
            listaAux->cantidadCanciones += 1;
            pushFront(listaAux->ListaReprod, c_ingresada);
        }
        else
        {
            Reproduccion *lista = (Reproduccion *)malloc(sizeof(Reproduccion));
            lista->cantidadCanciones = 1;
            lista->ListaReprod = createList();
            lista->NombreList = (char *)malloc(sizeof(char) * 35);
            strcpy(lista->NombreList, c_ingresada->NombreLista);
            pushFront(lista->ListaReprod, c_ingresada);
        }
    }
    else
        printf("Ya existe una cancion con ese nombre\n");
}

Reproduccion* existeReproduccion(Biblioteca* biblioteca, char* nombreList)
{
    Reproduccion* repro = firstList(biblioteca->ListaGeneral);
    while (repro != NULL)
    {
        if (strcmp(repro->NombreList, nombreList) == 0)
        {
            //printf("* %s\n", repro->NombreList);
            break;
        }
        repro = nextList(biblioteca->ListaGeneral);
    }
    return repro;
}

void mostrarReproduccion(Biblioteca* biblioteca)
{
    char nombreLista[100];
    printf("Ingrese el nombre de la lista: ");
    getchar();
    scanf("%[^\n]s",nombreLista);
    printf("\n");
    Reproduccion* repro = existeReproduccion(biblioteca, nombreLista);
    
    if (repro == NULL)
        printf("La lista no existe\n");

    if (repro != NULL)
    {
        printf("--- %s ---\n", nombreLista);

        Cancion* cancion = (Cancion*)firstList(repro->ListaReprod);
        while (cancion != NULL)
        {
            imprimirCancion(cancion);
            cancion = (Cancion*)nextList(repro->ListaReprod);
        }
    }
}

void mostrarCanciones(List* listaCanciones)
{
    Cancion* cancion = (Cancion*)firstList(listaCanciones);
    while (cancion != NULL)
    {
        imprimirCancion(cancion);
        cancion = (Cancion*)nextList(listaCanciones);
    }
}

void BuscarPorNombre(List *ListaCanciones){
    printf("Ingrese el nombre de la cancion: ");
    char cancionBuscada[100];
    getchar();
    scanf("%[^\n]s",cancionBuscada);
    Cancion *canciones = (Cancion *) firstList(ListaCanciones);
    int flag = 0;
    
    while(canciones != NULL){    
        if(strcmp(canciones -> Nombre, cancionBuscada) == 0)
        {
            imprimirCancion(canciones);
            flag = 1;            
            break;
        }
        canciones = nextList(ListaCanciones);
    }
    if (flag == 0)
        printf("La cancion buscada no se encuentra");
    printf("\n");
}

void Buscar_artista(List *ListaCanciones){
    printf("Ingrese el nombre del artista: ");
    char art_buscado[100];
    getchar();
    scanf("%[^\n]s",art_buscado);
    int flag = 0;

    Cancion *canciones = firstList(ListaCanciones);

    while(canciones != NULL ){
        if (strcmp(canciones -> Artista, art_buscado) == 0){
            imprimirCancion(canciones);
            flag = 1;
        }
        canciones = nextList(ListaCanciones);
    }
    if (flag == 0) printf("No se encontro ninguna cancion de %s.\n" , art_buscado);
}

Cancion *crearCancion()
{
    Cancion *cancion = (Cancion *)malloc(sizeof(Cancion));
    cancion->Artista = (char *)malloc(sizeof(char) * 35);
    cancion->Genero = (char *)malloc(sizeof(char) * 35);
    cancion->Nombre = (char *)malloc(sizeof(char) * 35);
    cancion->year = (char *)malloc(sizeof(char) * 4);
    cancion->NombreLista = (char *)malloc(sizeof(char) * 35);
    return cancion;
}

int cancionExiste(char *c_buscada, List *listaCanciones){

    Cancion *aux = crearCancion();
    aux = firstList(listaCanciones);
    while (aux != 0)
    {
        if (strcmp(aux->Nombre, c_buscada) == 0)
            return 1;
        aux = nextList(listaCanciones);
    }
    return 0;
}

void EliminarCancion(Biblioteca *biblioteca)
{
    printf("Introduzca el nombre de la cancion que desea eliminar\n");
     char *c_eliminada = (char *)malloc(sizeof(char) * 35);
    getchar();
    scanf("%[^\n]s", c_eliminada);
    if (cancionExiste(c_eliminada, biblioteca->ListaCanciones))
    {
        // eliminar de lista de global y obtener nombre lista

        Cancion *aux1 = crearCancion();
        Cancion *aux2 = crearCancion();

        aux1 = firstList(biblioteca->ListaCanciones);
        while (aux1 != NULL)
        {
            if (strcmp(aux1->Nombre, c_eliminada) == 0)
            {
                popCurrent(biblioteca->ListaCanciones);
                break;
            }
            aux1 = nextList(biblioteca->ListaCanciones);
        }
        // eliminar de su lista especifica
        Reproduccion *reproAux = existeReproduccion(biblioteca, aux1->NombreLista);
        if (reproAux != NULL)
        {
            aux2 = firstList(reproAux->ListaReprod);
            while (aux2 != NULL)
            {
                if (strcmp(aux2->Nombre, c_eliminada) == 0)
                {
                    reproAux->cantidadCanciones -= 1;
                    popCurrent(reproAux->ListaReprod);
                    break;
                }
                aux2 = nextList(reproAux->ListaReprod);
            }
        }
        else
            printf("lista de reproduccion NO existe");
    }
    else
        printf("Cancion que quiere eliminar no existe");
}

void BuscarPorGenero(List *canciones)
{
    printf("Ingrese el genero: ");
    char *generoBuscado = (char *)malloc(sizeof(char) * 35);
    scanf("%s", generoBuscado);
    Cancion *cancion = firstList(canciones);

    while (cancion != NULL)
    {
        if (strstr(cancion->Genero, generoBuscado) != NULL)
        {
            imprimirCancion(cancion);
        }
        cancion = nextList(canciones);
    }
}

void mostrarLista(Biblioteca *biblioteca)
{
    Reproduccion *listas = firstList(biblioteca -> ListaGeneral);
    printf("Informacion listas de reproduccion =\n");
    while(listas != NULL){
        printf("Nombre de la Lista : %s\n", listas -> NombreList);
        printf("Cantidad de canciones = %d\n", listas -> cantidadCanciones);
        printf("-------------------------------\n");
        listas = nextList(biblioteca -> ListaGeneral);
    }
}