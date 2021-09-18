#include"utils.h"

int iniciar_servidor(void)
{
	int socket_servidor, socket_cliente;

    struct addrinfo hints, *servinfo, *p, client;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

    // Creamos el socket de escucha del servidor

	// Asociamos el socket a un puerto

	// Escuchamos las conexiones entrantes

    listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);
	return accept(socket_servidor, (struct sockaddr *)&dir_cliente, (socklen_t*)&tam_direccion);
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
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
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio = 1;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		if(strlen(valor) && !string_equals_ignore_case(valor, "\n"))
			list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
