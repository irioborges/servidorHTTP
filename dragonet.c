#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 80

typedef struct {
    int new_socket;
} thread_arg, *ptr_thread_arg;

void *trata_requisicao(void *arg){
        int valread;
        char buffer2[1024], buffer[1024];
        FILE * fp;
        ptr_thread_arg targ = (ptr_thread_arg)arg;

        valread = read(targ->new_socket, buffer, 1024);
        printf("%s\n", buffer);
        printf("\nEntrou na thread\n");
        fp = fopen("htdocs/index.html", "r");

        if (fp == NULL)
        {
            printf("Erro de abertura no arquivo\n");
        }

        while (fgets(buffer2, 1000, fp) != NULL)
            send(targ->new_socket, buffer2, strlen(buffer2), 0);

}

int main(int argc, char **argv)
{

    pthread_t threads[20000];
    thread_arg argumentos[20000];

    struct sockaddr_in address;

    int server_fd, new_socket, valread;
    int opt = 1;
    int addrlen = sizeof(address);
    int i = 0;

    char buffer[1024] = {0};
    char *hello = "<h1>Oi Roner, meu filho.</h1>";
    char buffer2[1000];

    FILE *fp;
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        printf("Chegou na requisição de conexão entrante\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("passou da requisição de conexão entrante\n");

        argumentos[i].new_socket = new_socket;

        pthread_create(&(threads[i]), NULL, trata_requisicao, &(argumentos[i]));
        i++;

        
    }

    return 0;
}

