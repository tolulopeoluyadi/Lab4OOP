#include <netinet/in.h> // Structure for storing address information
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h> // For socket APIs
#include <sys/types.h> 
#include <unistd.h>     // For close function

#define PORT 9001
#define MAX_COMMAND_LINE_LEN 1024

// Function to get command input from user
char* getCommandLine(char *command_line) {
    do { 
        if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
            fprintf(stderr, "fgets error");
            exit(1);
        }
    } while (command_line[0] == 0x0A);  // Repeat if only ENTER was pressed

    command_line[strlen(command_line) - 1] = '\0';  // Remove newline character
    return command_line;
}

int main(int argc, char const* argv[]) 
{ 
    int sockID = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockID == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    char buf[MAX_COMMAND_LINE_LEN];
    char responseData[MAX_COMMAND_LINE_LEN];

    struct sockaddr_in servAddr; 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 

    int connectStatus = connect(sockID, (struct sockaddr*)&servAddr, sizeof(servAddr)); 
    if (connectStatus == -1) { 
        perror("Connection to the server failed");
        close(sockID);
        exit(1);
    } 

    while (1) {
        printf("Enter Command (or type 'menu' for options): ");
        getCommandLine(buf);

        // Send command to the server
        send(sockID, buf, strlen(buf), 0);

        // Display menu options if "menu" command is entered
        if (strcmp(buf, "menu") == 0) {
            printf("COMMANDS:\n---------\n");
            printf("1. print\n2. get_length\n3. add_back <value>\n4. add_front <value>\n");
            printf("5. add_position <index> <value>\n6. remove_back\n7. remove_front\n");
            printf("8. remove_position <index>\n9. get <index>\n10. exit\n");
            continue;
        }

        // Exit if "exit" command is entered
        if (strcmp(buf, "exit") == 0) {
            close(sockID);
            printf("Client exiting...\n");
            exit(0);
        }

        // Receive response from server
        int receivedBytes = recv(sockID, responseData, sizeof(responseData) - 1, 0);
        if (receivedBytes > 0) {
            responseData[receivedBytes] = '\0';  // Null-terminate the response
            printf("\nSERVER RESPONSE: %s\n", responseData);
        } else {
            printf("Server disconnected or error occurred.\n");
            break;
        }

        // Clear the buffer for the next command
        memset(buf, '\0', MAX_COMMAND_LINE_LEN);
    }

    // Close socket after loop ends
    close(sockID);
    return 0; 
}
