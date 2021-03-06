/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

void* deserializar_buffer(t_buffer* buffer){
	char* msj;
	int length = 0;

	void* stream = buffer->stream;

	memcpy(&length, stream, sizeof(int));
	stream += sizeof(int);

	msj = malloc(length);
	memcpy(msj, stream, length);

	return msj;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	t_buffer* buffer = malloc(sizeof(t_buffer));
	void* a_enviar;
	int mensaje_length = strlen(mensaje) + 1;

	buffer->size = sizeof(int)
				 + mensaje_length;

	void* stream = malloc(buffer->size);
	int offset = 0;


	memcpy(stream + offset, &mensaje_length, sizeof(int));
	offset += sizeof(int);
	memcpy(stream + offset, mensaje, mensaje_length);

	buffer->stream = stream;

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = buffer;
	int bytes = buffer->size + (sizeof(int) * 2);

	a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

//TODO
char* recibir_mensaje(int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	char* mensaje_recibido;
	int size_buffer = 0;

	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(int), 0);
	recv(socket_cliente, &(size_buffer), sizeof(int), 0);
	paquete->buffer->size = size_buffer;
	paquete->buffer->stream = malloc(size_buffer);
	recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, 0);

	switch(paquete->codigo_operacion) {
	    case MENSAJE:
	        mensaje_recibido = deserializar_buffer(paquete->buffer);
	        break;
	    default:
	    	break;
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return mensaje_recibido;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
