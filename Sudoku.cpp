/*Practica 3: Implementacion de un SUDOKU.
Curso de Fundamentos de la Programacion: 1ºE
Participantes:
Cesar Diaz-Faes Perez

Descripcion general del programa:
En esta practica se pide la implementacion de un conocido juego llamado Sudoku. El objetivo del sudoku es rellenar
una cuadricula de DIM × DIM casillas dividida en regiones de 3 por 3 con numeros del 1 al 9 partiendo de algunos
numeros ya dispuestos en algunas de las celdas. Para completar el juego con exito se debe cumplir que los numeros sean
nueve elementos diferenciados, que no se deben repetir en una misma fila, columna o region.
Este programa dispone de un menu de opciones en el cual el usuario podra elegir varias opciones tales como ver los valores
posibles de cada casilla, colocar o borrar numeros del tablero(siempre que no sean de la plantilla original), completar
los valores simples, detectar numeros mal introducidos, reiniciar el tablero o salir del programa cuando se desee.
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string nombreSudokuDefecto = "sudoku1.txt";
const string nombreSolucionDefecto = "solsdk1.txt";
const int CENTINELA = -1;
const int DIM = 9; //Esta es la dimension del sudoku
typedef int tEnteros[DIM][DIM]; //Array bidimensional de enteros que contendra los valores de las casillas de los sudokus
typedef int tArrValores[DIM]; //Array de enteros que contendra los valores posibles de las casillas
typedef bool tEstados[DIM][DIM]; //Array bidimensional de booleanos que nos ayudara a saber si la casilla tiene valor incial
//Estructura que contendra las caracteristicas del tablero
typedef struct {
	int contador; //Un contador de casillas rellenadas
	tEnteros casilla;
	tEstados inicial;
} tTablero;
//Estructura usada para formar arrays de valores posibles de cada casilla
typedef struct {
	int contador; //Contador de posicion del array de valores
	tArrValores valores;
} tValores;

//Prototipos

/*
Salida: un entero que sera la opcion del menu elegida por el usuario
Muestra el menu por pantalla y pide una opcion.
*/
void mostrarMenu(int &op);
/*
Salida: un array bidimensional que sera el tablero del programa
Inicializa el tablero poniendo a 0 y false las casillas.
*/
void inicializarTablero(tTablero &tablero);
/*
Salida: un array bidimensional que sera el tablero del programa y una variable que avisa si se ha producido algun error
Pide al usuario el nombre de un fichero que contenga una plantilla de un sudoku y si es correcto lo carga, en caso contrario,
		 muestra un mensaje de error y termina el programa.
*/
void cargarIniciales(tTablero &tablero, bool &error);
/*
Salida: un array bidimensional que sera el tablero del programa y una variable que avisa si se ha producido algun error
Pide al usuario el nombre de un fichero que contenga la solucion del sudoku anterior y si es correcto lo carga, en caso contrario,
		 muestra un mensaje de error y termina el programa.
*/
void cargarSolucion(tEnteros solucion, bool &error);
/*
Salida: un array bidimensional que sera la solucion del sudoku
Muestra por pantalla el tablero actualizado y el numero de casillas completadas y que faltan por completar.
*/
void dibujarTablero(tTablero &tablero);
/*
Salida: dos enteros que seran las coordenadas de una casilla
Pide al usuario que introduzca las coordenadas de una casilla, compuesta por una fila y una columna.
*/
void pedirCoordenadas(int &f, int &c);
/*
Entrada: un array bidimensional que sera el tablero del programa y dos enteros que seran las coordenadas de una casilla
Salida: un booleano
Comprueba si una casilla esta vacia o no, es decir, si existe en ella un valor distinto de cero.
*/
bool esVacio(tTablero tablero, int f, int c);
/*
Entrada: un array bidimensional que sera el tablero del programa, un entero que sera la fila de una casilla y un entero
		 que sera el valor destinado a una casilla
Salida: un booleano
Comprueba si el valor pedido se encuentra en la misma fila que la casilla de las coordenadas dadas.
*/
bool estaEnFila(tTablero tablero, int f, int valor);
/*
Entrada: un array bidimensional que sera el tablero del programa, un entero que sera la columna de una casilla y un entero
		 que sera el valor destinado a una casilla
Salida: un booleano
Comprueba si el valor pedido se encuentra en la misma columna que la casilla de las coordenadas dadas.
*/
bool estaEnColumna(tTablero tablero, int c, int valor);
/*
Entrada: un array bidimensional que sera el tablero del programa, dos enteros que seran las coordenadas de una casilla y un entero
		 que sera el valor destinado a una casilla
Salida: un booleano
Comprueba si el valor pedido se encuentra en la misma region que la casilla de las coordenadas dadas.
*/
bool estaEnRegion(tTablero tablero, int f, int c, int valor);
/*
Entrada: un entero que sera el valor destinado a una casilla
Salida: un array de valores que contendra los valores enteros
Inserta un numero dado en la ultima posicion disponible de un array de valores.
*/
void insertarFinal(tValores &candidatos, int valor);
/*
Entrada: un array bidimensional que sera el tablero del programa y dos enteros que seran las coordenadas de una casilla
Salida: un array de enteros
Comprueba que valores son posibles en una casilla dada y los guarda en un array de enteros. 
*/
void candidatosPosibles(tTablero tablero, tValores &candidatos, int f, int c);
/*
Entrada: un array de enteros
Muestra por pantalla los valores posibles de una casilla almacenados en un array de enteros.
*/
void mostrarCandidatos(tValores candidatos);
/*
Entrada: un array bidimensional que sera el tablero del programa
Implementa la opcion 1 del programa que se encarga de pedir las coordenadas de una casilla y a continuacion muestra
		 que valores son logicamente posibles en dicha casilla.
*/
void opcion1(tTablero tablero);
/*
Salida: un entero que sera el valor que el usuario desea introducir
Pide al usuario un valor del 1 al 9, que desee colocar en una casilla.
*/
void pedirValor(int &valor);
/*
Entrada: un array bidimensional que sera el tablero del programa y dos enteros que seran las coordenadas de una casilla
Salida: un booleano correspondiente al estado de la casilla dada
Comprueba si la casilla dada contiene un valor inicial o no.
*/
bool errorCasillaInicial(tTablero tablero, int f, int c);
/*
Entrada: dos enteros que seran las coordenadas de una casilla y un entero que sera el valor destinado a una casilla
Salida: un array bidimensional que sera el tablero del programa
Guarda en la casilla correspondiente a las coordenadas introducidas el valor dado por el usuario.
*/
void colocarCasilla(tTablero &tablero, int f, int c, int valor);
/*
Salida: un array bidimensional que sera el tablero del programa
Implementa la opcion 2 del programa que se encarga de pedir las coordenadas de una casilla y un valor que se quiera
			 introducir en dicha casilla, comprueba si esa casilla esta disponible o si el valor se puede introducir
			 y en dicho caso coloca dicho valor y dibuja el tablero, de lo contrario muestra un mensaje de error.
*/
void opcion2(tTablero &tablero);
/*
Entrada: dos enteros que seran las coordenadas de una casilla
Salida: un array bidimensional que sera el tablero del programa
Borra el valor que haya en la casilla poniendolo a 0.
*/
void borrarCasilla(tTablero &tablero, int f, int c);
/*
Salida: un array bidimensional que sera el tablero del programa
Implementa la opcion 3 del programa que se encarga de pedir las coordenadas de una casilla, comprobar si esa casilla
esta disponible y borrar el numero en dicho caso y mostrando el tablero. En caso contrario mostrara un mensaje de error.
*/
void opcion3(tTablero &tablero);
/*
Entrada: dos arrays bidimensionales que seran el tablero del programa y la solucion del sudoku y dos enteros que seran
		 las coordenadas de una casilla
Salida: un booleano
Comprueba si la casilla dada no esta vacia y si el valor que hay en ella es correcto, comparandolo con la solucion.
*/
bool valorErroneo(tTablero tablero, tEnteros solucion, int i, int j);
/*
Entrada: dos arrays bidimensionales que seran el tablero del programa y la solucion del sudoku
Implementa la opcion 4 del programa que se encarga de recorrer todo el tablero en busca de los valores de las casillas
		 erroneos, en cuyo caso se mostrara el mensaje de error y la localizacion de dicho error junto con el valor erroneo.
*/
void opcion4(tTablero tablero, tEnteros solucion);
/*
Entrada: un array bidimensional que sera el tablero del programa
Comprueba si las casillas de todo el tablero estan vacias o no son valores de la plantilla inicial y limpia dichas casillas
		 dejando el tablero como al inicio del programa, unicamente con los valores cargados inicialmente.
*/
void reiniciarTablero(tTablero &tablero);
/*
Entrada: un array bidimensional que sera el tablero del programa
Implementa la opcion 5 del programa que se encarga de reiniciar el tablero y despues lo dibuja.
*/
void opcion5(tTablero &tablero);
/*
Entrada: un array bidimensional que sera el tablero del programa y dos enteros que seran las coordenadas de una casilla
Salida: un array de enteros y un booleano
Comprueba si en la casilla dada existe la posibilidad de que solo haya un valor unico en ella.
*/
bool esCasillaSimple(tTablero tablero, tValores &candidatos, int i, int j);
/*
Salida: un array bidimensional que sera el tablero del programa
Implementa la opcion 6 del programa que se encarga de recorrer el tablero comprobando casilla por casilla si en ellas hay
		 valores simples, actualiza el tablero y el contador de casillas vacias y muestra el numero de valores
		 rellenados(cabe la posibilidad de que ese numero sea 0).
*/
void opcion6(tTablero &tablero);


//Funciones y procedimientos

void mostrarMenu(int &op){

		cout << endl;
		cout << "***************************************************************** " << endl;
		cout << "* 0.- SALIR                                                     * " << endl;
		cout << "* 1.- Ver posibles valores de casilla                           * " << endl;
		cout << "* 2.- Colocar un valor en una casilla                           * " << endl;
		cout << "* 3.- Borrar el valor de una casilla                            * " << endl;
		cout << "* 4.- Mostrar valores incorrectos                               * " << endl;
		cout << "* 5.- Reiniciar tablero                                         * " << endl;
		cout << "* 6.- Completar casillas simples                                * " << endl;
		cout << "***************************************************************** " << endl;
		cout << "Opcion: ";
		cin >> op;
		cin.sync();
}

void inicializarTablero(tTablero &tablero){
	int i, j;

	tablero.contador = 0;
	for (i=0; i<DIM; i++){
		for (j=0; j<DIM; j++){
			tablero.casilla[i][j] = 0;
			tablero.inicial[i][j] = false;
		}
	}
}

void cargarIniciales(tTablero &tablero, bool &error){
	int i, j, valor;
	string nombreSudoku;
	ifstream archivo;
	
	cout << "Nombre del sudoku o <INTRO> para archivo por defecto '" << nombreSudokuDefecto << "': ";
	getline(cin, nombreSudoku);
	if (nombreSudoku == "")
		nombreSudoku = nombreSudokuDefecto;

	archivo.open(nombreSudoku);
	if (archivo.is_open()){ //En caso de que el archivo este abierto correctamente se procede a la lectura de el mismo
		do{
			archivo >> i;
			archivo >> j;
			archivo >> valor;
			if (i != -1){
				tablero.casilla[i-1][j-1] = valor;
				tablero.inicial[i-1][j-1] = true;
				tablero.contador++;
			}
			if ((i > DIM) || (i == 0) || (i < -1) || (j > DIM) || (j <= 0) || (valor > DIM) || (valor <= 0))
				error = true;
		
		} while ((i != CENTINELA) && (!error));
	archivo.close();
	}
	
	else
		error = true;
		
	if (error)
		cout << "Error en la carga de fichero inicial, fin del programa." << endl;
}

void cargarSolucion(tEnteros solucion, bool &error){
	int i, j, valor;
	string nombreSudoku;
	ifstream archivo;

	cout << "Nombre de la solucion o <INTRO> para archivo por defecto '" << nombreSolucionDefecto << "': ";
	getline(cin, nombreSudoku);
	if (nombreSudoku == "")
		nombreSudoku = nombreSolucionDefecto;
		
	archivo.open(nombreSudoku);
	if (archivo.is_open()){ //En caso de que el archivo este abierto correctamente se procede a la lectura de el mismo
		for (i=0; i<DIM; i++){
			for (j=0; j<DIM; j++){
				archivo >> valor;
				solucion[i][j] = valor;
			}
		}
		
	archivo.close();
	}
	
	else{
		error = true;
		cout << "Error en la carga del fichero solucion, fin del programa." << endl;
	}
}

void dibujarTablero(tTablero &tablero){
	int i, j, k;
	int cont = 0;
	
	cout << "   ";
	for (i=0; i<DIM; i++){ //Dibujamos el indice de las filas
		if (i < 9)
			cout << "  " << i + 1;
		else
			cout << " " << i + 1;
		if (((i + 1) % 3) == 0)
			cout << " ";
	}
	
	cout << endl;
	cout << "   -";
	for (i=0; i<=DIM; i++)
		cout << "---";
	for (i=0; i<DIM; i++){ //Dibujamos el indice de las columnas
		cout << endl;
		if (i < 9)
			cout << " " << i + 1 << " |";
		else
			cout << " " << i + 1 << "|";
		for (j=0; j<DIM; j++){
			if (tablero.casilla[i][j] == 0)
				cout << "   "; //Espacio_Numero(vacio)_Espacio
				
			else
				cout << " " << tablero.casilla[i][j] << " ";
				
			if (((j + 1) % 3) == 0)
				cout << "|";
		}
		
		if (((i + 1) % 3) == 0){
			cout << endl;
			cout << "   -";
			for (k=0; k<=DIM; k++)
				cout << "---";
		}
	}
	cout << endl;
	cout << "Casillas completadas: " << tablero.contador << endl;
	cout << "Casillas restantes: " << (DIM * DIM) - tablero.contador << endl;
}

void pedirCoordenadas(int &f, int &c){

	do{
		cout << "Introduce fila y columna [1..9]: ";
		cin >> f;
		cin >> c;
	} while ((f <= 0) || (f > DIM) || (c <= 0) || (c > DIM));
}

bool esVacio(tTablero tablero, int f, int c){

	return (tablero.casilla[f-1][c-1] == 0);
}

bool estaEnFila(tTablero tablero, int f, int valor){
	int cont = 0;
	bool existe = false;

	while ((cont < DIM) && (!existe)){
		if (tablero.casilla[f-1][cont] == valor)
			existe = true;
		cont++;
	}
	return existe;
}

bool estaEnColumna(tTablero tablero, int c, int valor){
	int cont = 0;
	bool existe = false;

	while ((cont < DIM) && (!existe)){
		if (tablero.casilla[cont][c-1] == valor)
			existe = true;
		cont++;
	}
	return existe;
}

bool estaEnRegion(tTablero tablero, int f, int c, int valor){
	int contFila = 0;
	int contColumna = 0;
	int fila = f - 1;
	int columna = c - 1;
	bool existe = false;

	while ((contFila < 3) && (!existe)){
		while ((contColumna < 3) && (!existe)){
			if (tablero.casilla[(fila / 3) * 3 + contFila][(columna / 3) * 3 + contColumna] == valor)
				existe = true;
			contColumna++;
		}
		contFila++;
		contColumna = 0;
	}
	return existe;
}

void insertarFinal(tValores &candidatos, int valor){
	
	candidatos.valores[candidatos.contador] = valor;
	candidatos.contador++;
}

void candidatosPosibles(tTablero tablero, tValores &candidatos, int f, int c){
	int i;

	candidatos.contador = 0;  //Reiniciamos el array y borramos todo lo que contiene
	for (i=1; i<=DIM; i++){
		if((!estaEnFila(tablero, f, i)) && (!estaEnColumna(tablero, c, i)) && (!estaEnRegion(tablero, f, c, i)))
			insertarFinal(candidatos, i);
	}
}

void mostrarCandidatos(tValores candidatos){
	int i;

	for (i=0; i<candidatos.contador; i++)
		cout << candidatos.valores[i] << " ";
	cout << endl;
}

void opcion1(tTablero tablero){
	tValores candidatos;
	int f, c;

	pedirCoordenadas(f, c);
	if (esVacio(tablero, f, c)){
		candidatosPosibles(tablero, candidatos, f, c);
		mostrarCandidatos(candidatos);
	}

	else
		cout << "Dicha casilla esta ya ocupada" << endl;
}

void pedirValor(int &valor){

	do{
		cout << "Introduce un valor que este en el intervalo [1..9]: ";
		cin >> valor;
	} while ((valor <= 0) || (valor > 9));
}

bool errorCasillaInicial(tTablero tablero, int f, int c){
	
	return (tablero.inicial[f-1][c-1]);
}

void colocarCasilla(tTablero &tablero, int f, int c, int valor){
	
	tablero.casilla[f-1][c-1] = valor;
	tablero.contador++;
}

void opcion2(tTablero &tablero){
	int f, c, valor;

	pedirCoordenadas(f, c);
	pedirValor(valor);
	if (esVacio(tablero, f, c)){
		if((!estaEnFila(tablero, f, valor)) && (!estaEnColumna(tablero, c, valor)) && (!estaEnRegion(tablero, f, c, valor))){
			colocarCasilla(tablero, f, c, valor);
			dibujarTablero(tablero);
		}

		else
			cout << "Digito NO VALIDO!!!" << endl;
	}

	else if (errorCasillaInicial(tablero, f, c))
		cout << "La casilla tiene un valor incial, NO MODIFICABLE" << endl;

	else
		cout << "La casilla ya tiene un valor!!!" << endl;
}

void borrarCasilla(tTablero &tablero, int f, int c){

	tablero.casilla[f-1][c-1] = 0;
	tablero.contador--;
}

void opcion3(tTablero &tablero){
	int f, c;

	pedirCoordenadas(f, c);
	if (!esVacio(tablero, f, c)){
		if (!errorCasillaInicial(tablero, f, c)){
			borrarCasilla(tablero, f, c);
			dibujarTablero(tablero);
		}

		else
			cout << "Casilla con valor inicial, NO SE PUEDE BORRAR" << endl;
	}

	else
		cout << "La casilla ya esta vacia!!!" << endl;
}

bool valorErroneo(tTablero tablero, tEnteros solucion, int i, int j){
	bool error = false;

	if (!esVacio(tablero, i, j)){
		if (tablero.casilla[i-1][j-1] != solucion[i-1][j-1])
			error = true;
	}
	return error;
}

void opcion4(tTablero tablero, tEnteros solucion){
	int i, j;
	int cont = 0; // Usamos este contador simplemente para mostrar mensaje de que no hay ningun error o de que va a haber errores

	for (i=1; i<=DIM; i++){
		for (j=1; j<=DIM; j++){
			if (valorErroneo(tablero, solucion, i, j)){
				cont++;
				if (cont == 1)
					cout << "Errores: " << endl;
				cout << "[" << i << "," << j << "]" << " --- " << "(" << tablero.casilla[i-1][j-1] << ")" << endl;
			}	
		}
	}
	if (cont == 0)
		cout << "Todo CORRECTO, no hay ERRORES" << endl;
}

void reiniciarTablero(tTablero &tablero){
	int i, j;

	for (i=1; i<=DIM; i++){
		for (j=1; j<=DIM; j++){
			if (!errorCasillaInicial(tablero, i, j)  && (!esVacio(tablero, i , j))){
				tablero.casilla[i-1][j-1] = 0;
				tablero.contador--;
			}
		}
	}
}

void opcion5(tTablero &tablero){

	reiniciarTablero(tablero);
	dibujarTablero(tablero);
}

bool esCasillaSimple(tTablero tablero, tValores &candidatos, int i, int j){
	bool simple = false;

	if ((esVacio(tablero, i, j)) && (!errorCasillaInicial(tablero, i, j))){
		candidatosPosibles(tablero, candidatos, i, j);
		if (candidatos.contador == 1)
			simple = true;
	}
	return simple;
}

void opcion6(tTablero &tablero){
	int i, j;
	int cont = 0; // Cuenta las casillas simples que se han rellenado
	tValores candidatos;

	for (i=1; i<=DIM; i++){
		for (j=1; j<=DIM; j++){
			if (esCasillaSimple(tablero, candidatos, i, j)){
				tablero.casilla[i-1][j-1] = candidatos.valores[0];
				tablero.contador++;
				cont++;
			}
		}
	}
	dibujarTablero(tablero);
	cout << "Se han completado " << cont << " casillas" << endl;
}

int main(){
	int opcion;
	bool error = false;
	char fin; //Lo usamos simplemente para que el usuario salga con un INTRO.
	tTablero tablero;
	tEnteros solucion;

	inicializarTablero(tablero);
	cargarIniciales(tablero, error);
	if (!error){ //Si no hay error en la carga, el programa continua con normalidad, en caso contrario sale del programa
		cargarSolucion(solucion, error);
		dibujarTablero(tablero);
		do{
			mostrarMenu(opcion);
			switch (opcion){

				case 1: {
					opcion1(tablero);
				} break;

				case 2: {
					opcion2(tablero);
				} break;

				case 3: {
					opcion3(tablero);
				} break;

				case 4: {
					opcion4(tablero, solucion);
				} break;

				case 5: {
					opcion5(tablero);		
				} break;

				case 6: {
					opcion6(tablero);	
				} break;
			}
		} while ((opcion != 0) && ((DIM * DIM) != tablero.contador));
	}

	if (tablero.contador == (DIM * DIM))
		cout << "Enhorabuena, ha completado satisfactoriamente el SUDOKU" << endl;

	cout << "Pulse <INTRO> para terminar.";
	cin.sync();
	cin.get(fin);

return 0;
}