#include <netinet/in.h>
#include <strings.h>

int main() {
  struct sockaddr_storage storage;
  socklen_t len = sizeof(storage);

  // Initialize the storage structure to zero
  bzero(&storage, len);

  storage.ss_family = AF_INET; // Set the address family to IPv4

  return 0;
}
