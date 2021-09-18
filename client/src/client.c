#include "globals.h"
#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	paquete = crear_paquete();


	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_debug(logger, "Hola! Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	char *ip = config_get_string_value(config, "IP");
	int puerto = config_get_int_value(config, "PUERTO");
	char *valor = config_get_string_value(config, "VALOR");

	/*printf("La ip es %s \n", ip);
	printf("El puerto es %i \n", puerto);
	printf("El valor es %i \n", valor);*/

	//Loggear valor de config
	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//enviar CLAVE al servidor (asumo que CLAVE es valor)
	enviar_mensaje(valor, conexion);

	//Envio el paquete con las cosas ingresadas por consola
	enviar_paquete(paquete, conexion);


	//free(ip);
	//free(valor);
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente

	return 0;
}

t_log* iniciar_logger(void)
{
	return log_create("tp0.log", "tp0", false, LOG_LEVEL_DEBUG);
}

t_config* iniciar_config(void)
{
	return config_create("tp0.config");
}

void leer_consola(t_log* logger)
{
	char* leido;

	while(1) {
		leido = readline(">");

		if(string_length(leido) == 0)
			break;

		//Lo guardo en el log
		log_debug(logger, leido);

		//Agrego el input al paquete que se va a enviar al server
		agregar_a_paquete(paquete, (void*)leido, strlen(leido) + 1);

		free(leido);
	}

	free(leido);
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	config_destroy(config);
	log_destroy(logger);
	liberar_conexion(conexion);
	eliminar_paquete(paquete);
}
