#include "iMan.h"

// Function to perform DNS resolution
int resolveDNS(const char *hostname, const char *port, struct addrinfo **res)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    return getaddrinfo(hostname, port, &hints, res);
}

// Function to create and connect a TCP socket
int createAndConnectSocket(struct addrinfo *res)
{
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
    {
        perror("Socket error");
        exit(1);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("Connection error");
        close(sockfd);
        exit(1);
    }

    return sockfd;
}

// Function to send an HTTP GET request
void sendHttpGetRequest(int sockfd, const char *command, const char *hostname)
{
    char request[MAX_BUFFER_SIZE];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.0\r\nHost: %s\r\n\r\n", command, hostname);

    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("Send error");
        close(sockfd);
        exit(1);
    }
}

// Function to read and process the HTTP response
void readHTTPResponse(int sockfd)
{
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesReceived;
    bool insideTag = false;
    bool notFound = false; // Flag to check if the error message is received

    while ((bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data

        // Check if the error message is present in the response
        if (strstr(buffer, "No matches for") != NULL)
        {
            notFound = true;
            break; // Exit the loop
        }

        // Process each character in the buffer
        for (ssize_t i = 0; i < bytesReceived; i++)
        {
            if (buffer[i] == '<')
            {
                insideTag = true;
            }
            else if (buffer[i] == '>')
            {
                insideTag = false;
            }
            else if (!insideTag)
            {
                // Print characters only if not inside an HTML tag
                putchar(buffer[i]);
            }
        }
    }

    if (notFound)
    {
        printf("ERROR: No such command\n");
    }
    else if (bytesReceived == -1)
    {
        perror("Receive error");
    }
}

void man(char *command)
{
    const char *hostname = "man.he.net";
    const char *port = "80"; // HTTP port

    // Step 1: Do DNS resolution
    struct addrinfo *res;
    int status = resolveDNS(hostname, port, &res);
    if (status != 0)
    {
        fprintf(stderr, "DNS resolution error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Step 2: Create and connect a TCP socket
    int sockfd = createAndConnectSocket(res);

    // Step 3: Send an HTTP GET request
    sendHttpGetRequest(sockfd, command, hostname);

    // Step 4: Read and process the HTTP response
    readHTTPResponse(sockfd);

    // Step 5: Close the socket
    close(sockfd);
}
