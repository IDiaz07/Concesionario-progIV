    #ifndef PRUEBAMANEJO_H
#define PRUEBAMANEJO_H

#include <winsock2.h>
#include "sqlite3.h"

void pruebaManejo(sqlite3 *db, SOCKET cliente_fd, const char *usuario);

#endif  
