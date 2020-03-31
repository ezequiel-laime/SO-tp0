/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	log_info(logger, "soy un log");

	config = leer_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	;
	log_info(logger, "IP = %s, PUERTO = %s", ip, puerto);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	conexion = crear_conexion(ip, puerto);

	//enviar mensaje
	char* mensaje_a_enviar = "Bienvenido a SO";
	char* mensaje_recibido;
	enviar_mensaje(mensaje_a_enviar, conexion);

	//recibir mensaje
	mensaje_recibido = recibir_mensaje(conexion);

	//loguear mensaje recibido
	log_info(logger, "Mensaje recibido: %s", mensaje_recibido);

	free(mensaje_recibido);
	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("tp0.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}

//TODO
t_config* leer_config(void)
{
	return config_create("tp0.config");
}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	config_destroy(config);
	log_destroy(logger);
}
