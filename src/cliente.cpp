#define _WIN32_WINNT 0x0600
#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PUERTO 12345
#define BUFFER_SIZE 1024

void enviarYMostrar(SOCKET sock, const std::string& comando) {
    char buffer[BUFFER_SIZE] = {0};
    send(sock, comando.c_str(), comando.size(), 0);
    Sleep(500);  // Espera 500 ms

    int bytesRecibidos = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytesRecibidos > 0) {
        std::cout << "Respuesta:\n" << buffer << std::endl;
    } else {
        std::cout << "No se recibió respuesta del servidor.\n";
    }
}

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al iniciar Winsock\n";
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error al crear socket\n";
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // versión compatible

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error al conectar con el servidor\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    int opcion;
    do {
        std::cout << "\n=== Menu Cliente ===\n";
        std::cout << "1. Iniciar sesion\n";
        std::cout << "2. Ver vehiculos\n";
        std::cout << "3. Salir\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                std::string user, pass;
                std::cout << "Usuario: ";
                std::getline(std::cin, user);
                std::cout << "Contraseña: ";
                std::getline(std::cin, pass);
                enviarYMostrar(sock, "LOGIN " + user + " " + pass);
                break;
            }
            case 2:
                enviarYMostrar(sock, "LISTAR");
                break;
            case 3:
                send(sock, "SALIR", 5, 0);
                std::cout << "Cerrando conexion...\n";
                break;
            default:
                std::cout << "Opción no válida\n";
        }

    } while (opcion != 3);

    closesocket(sock);
    WSACleanup();
    return 0;
}
