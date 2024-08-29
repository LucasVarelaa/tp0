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
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "Lei la Ip: %s, el puerto: %s y la clave: %s", ip, puerto, valor);


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

	printf("Programa terminado\n");
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger ;
	
	if ((nuevo_logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG)) == NULL)
	{
		printf("No se pudo iniciar el logger\n");
		exit(1); // si falla terminar el programa
	}
	
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if((nuevo_config = config_create("cliente.config")) == NULL)
	{
		printf("No se pudo iniciar el config\n");
		exit(1); // si falla terminar el programa
	}

	return nuevo_config;
}
// con #include <readline/readline.h>
void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa, va a hacer que el programa espere a que se ingrese una línea y devolverla en un string ya listo para loggear.
	leido = readline("> ");
	log_info(logger, ">> %s", leido); // loggeamos la linea leida

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (strcmp(leido, "") != 0)
	{
		free(leido);
		leido = readline("> ");
		log_info(logger, ">> %s", leido);
	}
	

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	leido = readline("> ");

	while (strcmp(leido, "") != 0)
	{
		agregar_a_paquete(paquete, leido, sizeof(char*) + 1 + strlen(leido)); // el tamaño del puntero, un carácter adicional para el cero de finalización de la cadena y la longitud de la cadena en sí.
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);

	//enviar paquete
	enviar_paquete(paquete, conexion);

	//eliminamos paquete
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  
	  if (logger != NULL) {
		log_destroy(logger);
	  }

	  if (config != NULL) {
		config_destroy(config); //Destruye la estructura de config en memoria
	  }

	  liberar_conexion(conexion);
}
