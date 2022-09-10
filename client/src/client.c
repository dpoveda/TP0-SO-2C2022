#include "client.h"



int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();


	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");



	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config, "IP");
	valor = config_get_string_value(config, "CLAVE");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config
	log_info(logger, ip);
	log_info(logger, valor);
	log_info(logger, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	if((nuevo_logger = log_create("tp0.log","creacionLogger", true, LOG_LEVEL_INFO)) == NULL){
		printf("¡No se pudo crear el logger!");
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create("./cliente.config")) == NULL){
		printf("¡No se pudo crear el config!");
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "")){ //si es igual me devuelve 0, caso contrario me duelve 1 y sigue el bucle
		log_info(logger, leido);

	// ¡No te olvides de liberar las lineas antes de regresar!
		free(leido);
		leer_consola(logger);
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	//creo el paquete
	//paquete = crear_paquete();
	paquete = crear_super_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	//aca podia haber usado leer_consola(logger), pero tenia que definir un logger y se hubiera mandado un logger, y me parecio mejor esta solucion
	leido = readline("> ");

	while(strcmp(leido, "")){
		int tamanio = strlen(leido) + 1;
		agregar_a_paquete(paquete, leido , tamanio);
		free(leido);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
