//int Test(int argc, char **argv);
#include <C_Image.hpp>

#include <iostream>
#include <string>
#include <filesystem>
#include <C_General.hpp>
#include <C_Trace.hpp>
#include <C_File.hpp>
#include <C_Arguments.hpp>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

using namespace std::filesystem;

int main(int argc, char** argv) {
	//return Test(argc, argv);
	C_Image imagen1;
	C_Image imagen2(imagen1);
	char nombreImagen[50];
	char nombreImagenNueva[50];
	int numeroSeleccion;

	// Pedimos al usuario que introduzca el nombre de una imagen
	cout << "Introduce el nombre de una imagen: \n";
	cin >> nombreImagen;

	// Comprobamos si la imagen introducida existe 
	if (C_FileExists(nombreImagen)) {
		cout << "La imagen introducida ha sido encontrada.\n\n";
		// Si existe, pedimos al usuario que introduzca el tipo de filtro que se aplicará
		cout << "A continuacion, se mostraran los diferentes filtros disponibles:\n";
		cout << "1 para aplicar filtro de difuminado.\n";
		cout << "2 para aplicar filtro de nitidez.\n";
		cout << "3 para aplicar filtro de contorno.\n";
		cout << "Introduce el numero de la opcion deseada: \n";
		cin >> numeroSeleccion;
		cout << "La imagen nueva ya ha sido generada.\n";

		switch (numeroSeleccion) {
		case 1: // Selección del filtro de difuminado
			cout << "Introduce nombre de la imagen resultado: ";
			cin >> nombreImagenNueva;
			aplicarConvolucion(imagen1, imagen2);
			imagen2.WriteBMP(nombreImagenNueva);
			break;
		case 2:
		}
	}
	// Si no existe, se muestra un mensaje indicando que no se ha podido encontrar
	else {
		cout << "La imagen introducida no ha sido encontrada.";
	}

	return 0;

}

void aplicarConvolucion(C_Image imagen, C_Image& imagen2) {
	int i, j, k, l;
	C_Matrix filtro(1, 3, 1, 3);//creo una matriz llamada filtro de tamnio 3x3

	creacionFiltro(filtro);//Llamo al metodo donde se ecuentran los tipos de filtros
	for (i = imagen.FirstRow(); i <= imagen.LastRow(); i++) {//Recorro filas de la imagen
		//C_Trace2Num("Estado",i,imagen.LastRow());
		for (j = imagen.FirstCol(); j <= imagen.LastCol(); j++){//Recorro columnas de la imagen 
			if (i - 1 >= imagen.FirstRow() && i + 1 <= imagen.LastRow() && j - 1 >= imagen.FirstCol() && j + 1 <= imagen.LastCol()) {//Compruebo si esta dentro de la matriz, solo
				C_Image m(1, 3, 1, 3);
				for (k = 1; k <= 3; k++) {
					for (l = 1; l <= 3; l++) {
						m(k, l) = imagen(i + k - 1 - 1, j + l - 1 - 1);
					}
				}
				double valor = calcularConvolucion(m, filtro);//llamo al metodo calcularconvolucion con mi submatriz de
				//la imagen original y mi filtro elegido y lo guardo en la variable valor

				int sumaf = 0;
				sumaf = filtro.Sum();

				//Guardo la suma total de cada valor de cada posicion del filtro elegido
				if (sumaf == 0) {
					sumaf = 1;
				}
				//como al dividir por 0 la imagen apareceria negra, le digo que el valor es 1, vease la formula
				;
				valor = valor / sumaf;
				//aplico la formula de la convolucion
				if (valor < 0) {
					valor = 0;
				}
				if (valor > 255) {
					valor = 255;
				}
				//si mi valor fuese negativo o superase la escala de grises de maximo 255 dejo ese valor como 255
				imagen2(i, j) = valor;
				//guardo la variable valor en cada posicion de la imagen
			}
		}
	}
}

double calcularConvolucion(C_Image m, C_Matrix filtro) {
	int i, j;
	double suma;

	suma = 0;
	for (i = m.FirstRow(); i <= m.LastRow(); i++) {
		for (j = m.FirstCol(); j <= m.LastCol(); j++) {
			suma = suma + m(i, j) * filtro(i, j);
		}
	}

	return suma;
}

void creacionFiltro(C_Matrix& filtro) {
	int seleccionFiltro, i, j;

	switch (seleccionFiltro) {
	case 1:
		//Filtro de Difuminado
		filtro(1, 1) = 1/9;
		filtro(1, 2) = 1/9;
		filtro(1, 3) = 1/9;
		filtro(2, 1) = 1/9;
		filtro(2, 2) = 1/9;
		filtro(2, 3) = 1/9;
		filtro(3, 1) = 1/9;
		filtro(3, 2) = 1/9;
		filtro(3, 3) = 1/9;
		break;
	case 2:
		//Filtro de Nitidez
		filtro(1, 1) = 0;
		filtro(1, 2) = -1;
		filtro(1, 3) = 0;
		filtro(2, 1) = -1;
		filtro(2, 2) = 5;
		filtro(2, 3) = -1;
		filtro(3, 1) = 0;
		filtro(3, 2) = -1;
		filtro(3, 3) = 0;
		break;
	case 3:
		//Filtro de Contorno
		filtro(1, 1) = -1;
		filtro(1, 2) = -1;
		filtro(1, 3) = -1;
		filtro(2, 1) = -1;
		filtro(2, 2) = 8;
		filtro(2, 3) = -1;
		filtro(3, 1) = -1;
		filtro(3, 2) = -1;
		filtro(3, 3) = -1;
		break;
	}
}
