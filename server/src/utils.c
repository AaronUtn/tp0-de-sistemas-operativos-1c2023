#include"utils.h"

t_log* logger;
uint32_t handshake;
uint32_t resultOk = 0;
uint32_t resultError = -1;

int iniciar_servidor(void)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	//int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; //AF_INET probar cuando son los casos que tenemos una IP4v //antes estaba AF_UNSPEC cuando no sabemos si es una IP4v o IP6v;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int resultadoGetaddrinfo = getaddrinfo(NULL, PUERTO, &hints, &servinfo);

	if (resultadoGetaddrinfo < 0){
		fprintf(stderr,"Error en getaddrinfo: %s\n ", gai_strerror(resultadoGetaddrinfo));
		exit(-1);
	}
	// Creamos el socket de escucha del servidor
	int socket_servidor = socket( servinfo->ai_family,
								  servinfo->ai_socktype,
								  servinfo->ai_protocol);
	if (socket_servidor == -1){
		perror("Error en socket");
		exit(-1);
	}
	//permite re utilizar los sokects
	int activador = 1;
	setsockopt(socket_servidor,SOL_SOCKET,SO_REUSEADDR,&activador,sizeof(activador));

	if((bind(socket_servidor,servinfo->ai_addr,servinfo->ai_addrlen))!=0){
		perror("Falla el bind");
		return 1;
	}

	log_trace(logger, "Listo para escuchar a mi cliente");
	listen(socket_servidor,SOMAXCONN);


	freeaddrinfo(servinfo);

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	//t socket_cliente;
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	printf("Recibi una conexion en %d \n",socket_cliente);
	// Escuchamos las conexiones entrantes


	recv(socket_cliente, &handshake, sizeof(uint32_t), MSG_WAITALL);
	if(handshake == 1)
	   send(socket_cliente, &resultOk, sizeof(uint32_t), NULL);
	else
	   send(socket_cliente, &resultError, sizeof(uint32_t), NULL);

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	//log_info(logger, "Me llego el mensaje %s", buffer);
	printf("me llego el mensaje %s",buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
