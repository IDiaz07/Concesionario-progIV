#define _WIN32_WINNT 0x0600
#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include "sockets.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PUERTO 12345
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al iniciar Winsock\n";
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Error al crear socket\n";
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error al conectar con el servidor\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "Conectado al servidor.\n";

    int opcion;
    char buffer[BUFFER_SIZE] = {0};
    bool salir = false;

    do {
        cout << "\n=== Menu Cliente ===\n";
        cout << "1. Registrar usuario\n";
        cout << "2. Iniciar sesion\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                char usuario[50], contrasena[50], email[100], mensaje[256];

                printf("Nuevo usuario: ");
                scanf("%s", usuario);

                printf("Nueva contrasena: ");
                scanf("%s", contrasena);

                printf("Email: ");
                scanf("%s", email);

                sprintf(mensaje, "REGISTRAR %s %s %s", usuario, contrasena, email);
                send(sock, mensaje, strlen(mensaje), 0);

                int bytesRecibidos = recv(sock, buffer, sizeof(buffer) - 1, 0);
                if (bytesRecibidos > 0) {
                    buffer[bytesRecibidos] = '\0';
                    printf("Respuesta:\n%s\n", buffer);
                }
                break;
            }
            case 2: {
                char usuario[50], contrasena[50], mensaje[150];
                char buffer[BUFFER_SIZE] = {0};

                printf("Usuario: ");
                scanf("%s", usuario);

                printf("Contrasenya: ");
                scanf("%s", contrasena);

                sprintf(mensaje, "LOGIN %s %s", usuario, contrasena);
                send(sock, mensaje, strlen(mensaje), 0);

                int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
                if (bytes <= 0) break;

                buffer[bytes] = '\0';
                printf("Respuesta:\n%s\n", buffer);

                if (strstr(buffer, "Exito|Login correcto|")) {
                    while (true) {
                        memset(buffer, 0, sizeof(buffer));
                        int b = recv(sock, buffer, sizeof(buffer) - 1, 0);
                        if (b <= 0) break;

                        buffer[b] = '\0';
                        printf("%s", buffer);

                        // --- NUEVO: manejar filtrado ---
                        if (strstr(buffer, "Filtrar vehiculos por:") != nullptr) {
                            string criterio;
                            cout << "Introduce criterio: ";
                            getline(cin, criterio);
                            criterio += '\n';
                            send(sock, criterio.c_str(), criterio.length(), 0);
                            continue;
                        }

                        if (strstr(buffer, "Ingrese el valor a buscar:") != nullptr) {
                            string valor;
                            cout << "(cliente) Introduce valor: ";
                            getline(cin, valor);
                            valor += '\n';
                            send(sock, valor.c_str(), valor.length(), 0);

                           string resultado;
                            while (true) {
                                memset(buffer, 0, sizeof(buffer));
                                int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
                                if (n <= 0) break;

                                buffer[n] = '\0';
                                if (strcmp(buffer, "FIN\n") == 0 || strstr(buffer, "FIN") != nullptr) break;

                                resultado += buffer;
                            }

                            cout << "\n--- Resultados del filtrado ---\n";
                            cout << resultado << endl;
                            continue;
                        }

                        if (strstr(buffer, "Saliendo del menu administrativo") ||
                            strstr(buffer, "Saliendo del menu DeustoMotors")) {
                            break;
                        }

                        string input;
                        getline(cin, input);
                        if (input.empty()) continue;
                        input += '\n';
                        send(sock, input.c_str(), input.length(), 0);
                    }

                    cout << "\nHas salido del menu.\n";
                }
                break;
            }
            case 3: {
                send(sock, "SALIR", 5, 0);
                int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
                if (bytes > 0) {
                    buffer[bytes] = '\0';
                    printf("Respuesta:\n%s\n", buffer);

                    if (strstr(buffer, "Saliendo del programa")) {
                        salir = true;
                    }
                }
                break;
            }
            default:
                cout << "Opción no válida\n";
        }

    } while (opcion != 3);

    closesocket(sock);
    WSACleanup();
    return 0;
}
