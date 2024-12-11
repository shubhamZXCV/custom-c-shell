#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "color.h"
#include <stdbool.h>

#define BUFFER_SIZE 4096

// Function to create and connect a socket to the server
int create_socket(const char *host, int port)
{
    struct sockaddr_in server_addr;
    struct hostent *server;

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return -1;
    }

    // Resolve hostname
    server = gethostbyname(host);
    if (server == NULL)
    {
        fprintf(stderr, "No such host found\n");
        return -1;
    }

    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    return sock;
}

// Function to perform the GET request and fetch the man page
void fetch_man_page(const char *command_name)
{
    const char *host = "man.he.net";
    int port = 80;
    char buffer[BUFFER_SIZE];

    // Create a socket and connect to the server
    int sock = create_socket(host, port);

    // Form the GET request for the man page
    sprintf(buffer, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", command_name, host);

    // Send the GET request
    if (send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("Send failed");
        close(sock);
        return;
    }

    // Receive the response
    int received_bytes;
  
   

    // Loop to receive the response
    while ((received_bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {

        int i = 0;
        bool inside_tag = false; // Keep track of whether we're inside an HTML tag

        while(buffer[i]!='<'){
            i++;
        }

        while (i < received_bytes)
        {
            
            if (buffer[i] == '<')
            {
                inside_tag = true; // Entering an HTML tag
            }

            if (!inside_tag)
            {
                printf("%c", buffer[i]); // Only print characters outside of tags
            }

            if (buffer[i] == '>')
            {
                inside_tag = false; // Exiting an HTML tag
            }

            i++;
        }
    }

    if (received_bytes < 0)
    {
        perror("Receive failed");
    }

    // Close the socket
    close(sock);
}

void iman(char *cmd)
{
    char *token = strtok(cmd, " ");

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        printf(RED "Insufficient args!!!\n" RESET);
        return;
    }

    fetch_man_page(token);
}