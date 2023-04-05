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
	//t_config* auxConfig;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	//logger = log_create ("tp0.log","TP0Prueba",1, LOG_LEVEL_INFO);

	log_info(logger,"Soy un log");

	printf("hola que tal\n");


	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	valor = config_get_string_value(config,"CLAVE");
	printf("la clave de config tiene lo siguiente: %s\n",valor);
	printf("la ip es tal: %s y el puerto es tal: %s\n",ip,puerto);

	log_info(logger, "\n la ip es tal: %s y el puerto es tal: %s\n",ip,puerto);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	/*auxConfig = iniciar_config();






	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto); //me conecto con el socket

	uint32_t handshake = 1;
	uint32_t result;
	int resSend = send(conexion, &handshake, sizeof(uint32_t), NULL);


	//printf("que hace send %d\n",resSend );
	int resRecv = recv(conexion, &result, sizeof(uint32_t), MSG_WAITALL);
	//printf("que hace recv %d\n",resRecv);
	enviar_mensaje( valor, conexion);

	//recibir_mensaje(conexion);


	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion);

	//liberar_conexion(conexion);

	log_info(logger,"Se logro enviar paquetes");
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	//config_destroy(config);
	//config_destroy(auxConfig);
	//return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	if ((nuevo_logger = log_create ("tp0.log","TP0Prueba",1, LOG_LEVEL_INFO)) == NULL){
		printf("no se pudo crear un logger\n");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create("./cliente.config") )== NULL){
		printf("algo salio mal");
		exit(2);
	}
		//config = auxConfig;
	return nuevo_config;

}

void leer_consola(t_log* logger)
{
	char* leido;
	while (1){
		leido = readline("> ");

		if (!leido){
			break;
		}
        if (leido) {
            add_history(leido);
        }
        if (!strncmp(leido, "" ,4)){
            free(leido);
            break;
        }

		log_info(logger, leido);
		free(leido);
	}
	// La primera te la dejo de yapa


	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	//char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();
	char* leido;
	leido = readline(">");
	while (strcmp(leido,"\0")){

		scanf("%s",leido);
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		enviar_paquete(paquete, conexion);
		//free(leido);
		leido = readline(">");
		/*leido = readline("> ");

		if (!leido){
			break;
		}
        if (leido) {
            //add_history(leido);
            scanf("%s",leido);
            agregar_a_paquete(paquete, leido, strlen(leido)+1);
            free(leido);
            enviar_paquete(paquete, conexion);
            leido = readline("> ");


        }
        if (!strncmp(leido, "", 4)) {
            free(leido);
            break;
        }*/
	}
		//log_info(logger, leido);
		free(leido);
		free(paquete);
	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{

	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if (logger != NULL){
		log_destroy(logger);
	}

	if (config != NULL){
		config_destroy(config);
	}
	liberar_conexion(conexion);
}
