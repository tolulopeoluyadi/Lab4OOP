#include <netinet/in.h> // Structure for storing address information
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> // For socket APIs 
#include <sys/types.h> 
#include <unistd.h>     // For close function
#include "list.h"       // Assume list.h has the linked list implementation

#define PORT 9001
#define ACK "ACK"

int main(int argc, char const* argv[]) 
{ 
    int n, val, idx;

    // Create server socket
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    if (servSockD == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Buffer to store data to/from client
    char buf[1024];
    char sbuf[1024];
    char* token;

    // Define server address
    struct sockaddr_in servAddr; 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // Bind socket to IP and port
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        perror("Bind failed");
        close(servSockD);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(servSockD, 1) == -1) {
        perror("Listen failed");
        close(servSockD);
        exit(1);
    }

    // Accept client connection
    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket == -1) {
        perror("Client connection failed");
        close(servSockD);
        exit(1);
    }

    // Initialize the linked list
    list_t *mylist = list_alloc();

    while(1) {
        // Receive messages from client
        n = recv(clientSocket, buf, sizeof(buf), 0);
        if (n <= 0) {
            perror("Receive failed or client disconnected");
            break;
        }
        buf[n] = '\0';  // Null-terminate the received data

        // Parse command from client
        token = strtok(buf, " ");
        if (strcmp(token, "exit") == 0) {
            list_free(mylist);
            close(clientSocket);
            close(servSockD);
            printf("Server exiting...\n");
            exit(0);
        } 
        else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            sprintf(sbuf, "Length = %d", val);
        } 
        else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_front(mylist, val);
            sprintf(sbuf, "%s %d added to front", ACK, val);
        } 
        else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_to_back(mylist, val);
            sprintf(sbuf, "%s %d added to back", ACK, val);
        } 
        else if (strcmp(token, "add_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            token = strtok(NULL, " ");
            val = atoi(token);
            list_add_at_index(mylist, val, idx);
            sprintf(sbuf, "%s %d added at position %d", ACK, val, idx);
        } 
        else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            sprintf(sbuf, "Removed %d from front", val);
        } 
        else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            sprintf(sbuf, "Removed %d from back", val);
        } 
        else if (strcmp(token, "remove_position") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_remove_at_index(mylist, idx);
            sprintf(sbuf, "Removed %d from position %d", val, idx);
        } 
        else if (strcmp(token, "get") == 0) {
            token = strtok(NULL, " ");
            idx = atoi(token);
            val = list_get_elem_at(mylist, idx);
            sprintf(sbuf, "Element at index %d is %d", idx, val);
        } 
        else if (strcmp(token, "print") == 0) {
            sprintf(sbuf, "%s", listToString(mylist));
        } 
        else {
            sprintf(sbuf, "Unknown command");
        }

        // Send response back to the client
        send(clientSocket, sbuf, sizeof(sbuf), 0);

        // Clear the buffer
        memset(buf, '\0', sizeof(buf));
    }

    // Cleanup in case of client disconnection or error
    list_free(mylist);
    close(clientSocket);
    close(servSockD);

    return 0;
}
