//int Test(int argc, char **argv);
// Librerías utilizadas proporcionadas por el profesor
#include <iostream>
#include <C_General.hpp>
#include <C_Trace.hpp>
#include <C_File.hpp>
#include <C_Arguments.hpp>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

// Inicializacion de los metodos que se utilizaran
void aplicarConvolucion(C_Image& imagen1, C_Image& imagen2);
void crearFiltro(C_Matrix &filtro);
double calcularConvolucion(C_Image imagenCalculo, C_Matrix filtro);

// Se declara la variable aqui para poder utilizarla a la hora de usarla en el switch que crea la matriz o kernel del filtro elegido
int numeroSeleccion;

// Metodo creado para ejecutar el contenido que se visualizara por la consola y con el cual se podra decidir que filtro utilizara el proceso de convolucion
int main(int argc, char** argv) {
	//return Test(argc, argv);
	C_Image imagen1;
	C_Image imagen2(imagen1);
	string nombreImagen;
	string nombreImagenNueva;

	// Pedimos al usuario que introduzca el nombre de una imagen
	cout << "Introduce el nombre de la imagen a la que quiera aplicarle un filtro: \n";
	cin >> nombreImagen;

	// Comprobamos si la imagen introducida existe 
	if (C_FileExists(nombreImagen.c_str())) {
		cout << "La imagen introducida ha sido encontrada.\n\n";
		// Si existe, pedimos al usuario que introduzca el tipo de filtro que se aplicará
		cout << "A continuacion, se mostraran los diferentes filtros disponibles: \n";
		cout << "1 para aplicar el filtro de difuminado.\n";
		cout << "2 para aplicar el filtro de nitidez.\n";
		cout << "3 para aplicar el filtro de contorno.\n";
		cout << "Introduce el numero de la opcion deseada: \n";
		cin >> numeroSeleccion;

		// Una vez seleccionado el filtro deseado, se aplica el filtro mediante el metodo aplicarConvolucion y se genera una nueva imagen con el resultado final
		switch (numeroSeleccion) {
		case 1: // Selección del filtro de difuminado
			cout << "Introduce el nombre de la imagen que se generara con el filtro elegido: \n";
			cin >> nombreImagenNueva;
			aplicarConvolucion(imagen1, imagen2);
			imagen2.WriteBMP(nombreImagenNueva.c_str());
			break;
		case 2: // Seleccion del filtro de nitidez
			cout << "Introduce el nombre de la imagen que se generara con el filtro elegido: \n ";
			cin >> nombreImagenNueva;
			aplicarConvolucion(imagen1, imagen2);
			imagen2.WriteBMP(nombreImagenNueva.c_str());
			break;
		case 3: // Seleccion del filtro de contorno
			cout << "Introduce el nombre de la imagen que se generara con el filtro elegido: \n";
			cin >> nombreImagenNueva;
			aplicarConvolucion(imagen1, imagen2);
			imagen2.WriteBMP(nombreImagenNueva.c_str());
			break;
		default:
			cout << "La opcion seleccionada no esta dentro de los parametros ofrecidos.\n";
			break;
		}
	}else { // Si no existe, se muestra un mensaje indicando que no se ha podido encontrar y se acaba el proceso
		cout << "La imagen introducida no ha sido encontrada.";
	}
	return 0;
}

// Metodo creado para aplicar la convolucion a la imagen tras haber hecho uso de los metodos crearFiltro y calcularConvolucion
void aplicarConvolucion(C_Image& imagen1, C_Image& imagen2) {
	// Inicializo la variable filtro que se usara a continuacion para poder generar la matriz o kernel del filtro deseado
	C_Matrix filtro(1, 3, 1, 3);
	// Inicializo la variableAuxiilar que se usara para almacenar los valores del pixel con el que se trabaja y los de sus alrededores(1 + 8) de la imagen
	C_Image matrizAuxiliar(1, 3, 1, 3);
	crearFiltro(filtro);
	// Recorro las filas y columnas de la matriz de la imagen
	for (int i = imagen1.FirstRow(); i <= imagen1.LastRow(); i++) {
		for (int j = imagen1.FirstCol(); j <= imagen1.LastCol(); j++) {
			// Se comprueba si el pixel selecionado esta dentro de la matriz
			if (i - 1 >= imagen1.FirstRow() && i + 1 <= imagen1.LastRow() && j - 1 >= imagen1.FirstCol() && j + 1 <= imagen1.LastCol()) {
				// En el caso de si estarlo, se recorro las filas y columnas de la matriz matrizAuxiliar
				for (int k = 1; k <= 3; k++) {
					for (int l = 1; l <= 3; l++) {
						// MatrizAuxiliar sera rellenada con los pixeles(1+8) de la imagen seleccionada
						matrizAuxiliar(k, l) = imagen1(i + k - 1 - 1, j + l - 1 - 1);
					}
				}
				// Se llama al metodo calcularConvolucion y se realiza la multiplicacion entre la matrizAuxiliar y la matriz o kernel filtro
				double valorResultado = calcularConvolucion(matrizAuxiliar, filtro);

				// Se realiza la suma de los valores de la matriz filtro para sacar el valor promedio y forzar que este siempre sea 1
				// El valor promedio se utiliza para que el resultado de la convolucion tenga una intensidad uniforme y que los detalles importantes de la imagen no se pierdan o distorsionen
				int valorPromedio = 0;
				valorPromedio = filtro.Sum();
				if (valorPromedio == 0) {
					valorPromedio = 1;
				}

				// Una vez obtenido el valor promedio de los valores de la matriz filtro, se divide el valorResultado entre el valorPromedio y el resultado estara limitado para que no salga de los limites de los colores (de 0 a 255)
				valorResultado = valorResultado / valorPromedio;
				if (valorResultado < 0) {
					valorResultado = 0;
				} else if (valorResultado > 255) {
					valorResultado = 255;
				}
				// Finalmente, el valor obtenido sera el que se cargue en la imagen2
				imagen2(i, j) = valorResultado;
			}
		}
	}
}

// Metodo creado para calcular la convolucion de la imagen matriz de la imagen obtenida 
double calcularConvolucion(C_Image matrizCalculo, C_Matrix filtro) {
	double suma = 0;

	// Se recorren las filas y columnas de la matriz de la imagen obtenida y se multiplica por cada valor correspondiente de la matriz del filtro elegido
	for (int i = matrizCalculo.FirstRow(); i <= matrizCalculo.LastRow(); i++) {
		for (int j = matrizCalculo.FirstCol(); j <= matrizCalculo.LastCol(); j++) {
			suma = suma + matrizCalculo(i, j) * filtro(i, j);
		}
	}
	return suma;
}

// Metodo creado para crear las diferentes matrices o kernels de los filtros que hay disponibles
void crearFiltro(C_Matrix &filtro) {
	switch (numeroSeleccion) {
	case 1:
		//Matriz o kernel del filtro de Difuminado
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
		//Matriz o kernel del filtro de Nitidez
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
		//Matriz o kernel del filtro de Contorno
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
