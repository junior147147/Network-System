/*  
	Yu Zhou
    CSCI 4327
    PA1
	Socket:Connection
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

struct Config {
 int port;
 char DocumentRoot[256];
 char DirectoryIndex[256];
 char content_type[100];
} config;


/***********************************************************************
    Parse configuration file
    Reads in from the config file and behave according
***********************************************************************/
void parse_config() {
	FILE *config_file = NULL;
	/*Open Config File*/
	config_file = fopen("./ws.conf", "r");
	if (config_file == NULL) {
		printf("Error loading ws.conf.\n");
		exit(1);
	}

	char* line;
	size_t length; 
	int bytesRead = 0;

	while ((bytesRead = getline(&line, &length, config_file)) != -1) {
		/*Comment Dismiss*/
		if (line[0] == '#') {
            continue;
		}

		char key[64], value[256];
		sscanf(line, "%s %s", key, value);

		/*Check Listen , and convert string to int*/
        if (strcmp(key, "Listen") == 0) {  
            config.port = atoi(value);     
            printf("Port value from config: %d\n", config.port);
        }

		/*Check DocumentRoot*/
		/*Need to get rid of both quotation marks*/
        if (strcmp(key, "DocumentRoot") == 0) {
        	/*Look for the sentence starts with a quotation mark*/ 
            sscanf(line, "%*s \"%s", config.DocumentRoot);
            /*Setting the last quotation mark to null*/
            config.DocumentRoot[strlen(config.DocumentRoot)-1] = '\0';
            printf("DocRoot: %s\n", config.DocumentRoot);
        }
	
		/*Check DirectoryIndex*/
        if (strcmp(key, "DirectoryIndex") == 0) {
            sscanf(line, "%*s %s", config.DirectoryIndex);
            printf("DirIndex: %s\n", config.DirectoryIndex);
        }

        if (key[0] == '.') {
            strcat(config.content_type, key);
            strcat(config.content_type, " ");
        }

	}
	fclose(config_file);
}

/***********************************************************************
    Read line by line of what clients send to the server
***********************************************************************/

int read_line(int sock_fd, char *buf, int buf_size) {
    int i = 0;
    char c = '\0';
    int b;

    /*make sure not new line*/
    while (i < (buf_size - 1) && (c != '\n')) { 
    	/*recv reads byte by byte*/
        b = recv(sock_fd, &c, 1, 0);
        
        /*If there is more to read*/
        if (b > 0) {
            if (c == '\r') {
                b = recv(sock_fd, &c, 1, MSG_PEEK);
                if ((b > 0) && (c == '\n')) {
                    recv(sock_fd, &c, 1, 0);
                /*Else new line*/
                } else {
                    c = '\n';
                }
            }
            buf[i] = c;
            i++;
        } else {
            c = '\n';
        }
    }
    buf[i] = '\0';
    return i;
}

/***********************************************************************
    send error to client, and let the client knows error
***********************************************************************/

void send_error(int sock_fd, const char *str, int err_code) {
    char buffer[1024];

    if (err_code == 400) {
    	/*Write Message to socket*/
        send(sock_fd, "HTTP/1.1 400 BAD REQUEST\r\n", strlen("HTTP/1.1 400 BAD REQUEST\r\n"), 0);
        send(sock_fd, "Content-Type: text/html\r\n", strlen("Content-type: text/html\r\n"), 0);
        /*Follow http protocol*/
        send(sock_fd, "\r\n", strlen("\r\n"), 0);

    } else if (err_code == 404) {
        send(sock_fd, "HTTP/1.1 404 NOT FOUND\r\n", strlen("HTTP/1.1 404 NOT FOUND\r\n"), 0);
        send(sock_fd, "Content-Type: text/html\r\n", strlen("Content-Type: text/html\r\n"), 0);
        send(sock_fd, "\r\n", strlen("\r\n"), 0);
        send(sock_fd, "<HTML><TITLE>404 NOT FOUND</TITLE>\r\n", strlen("<HTML><TITLE>404 NOT FOUND</TITLE>\r\n"), 0);
        sprintf(buffer, "<BODY><P>HTTP/1.1 404 Not Found: %s \r\n", str);
        send(sock_fd, buffer, strlen(buffer), 0);
        send(sock_fd, "</BODY></HTML>\r\n", strlen("</BODY></HTML>\r\n"), 0);

    } else if (err_code == 500) {
        send(sock_fd, "HTTP/1.0 500 Internal Server Error\r\n", strlen("HTTP/1.0 500 Internal Server Error\r\n"), 0);
        send(sock_fd, "Content-Type: text/html\r\n", strlen("Content-type: text/html\r\n"), 0);
        send(sock_fd, "\r\n", strlen("\r\n"), 0);
        send(sock_fd, "HTTP/1.1 500  Internal  Server  Error\r\n", strlen("HTTP/1.1 500  Internal  Server  Error\r\n"), 0);

    } else if (err_code == 501) {
        send(sock_fd, "HTTP/1.1 501 Method Not Implemented\r\n", strlen("HTTP/1.1 501 Method Not Implemented\r\n"), 0);
        send(sock_fd, "Content-Type: text/html\r\n", strlen("Content-Type: text/html\r\n"), 0);
        send(sock_fd, "\r\n", strlen("\r\n"), 0);
        send(sock_fd, "<HTML><HEAD><TITLE>Method Not Implemented\r\n", strlen("<HTML><HEAD><TITLE>Method Not Implemented\r\n"), 0);
        send(sock_fd, "</TITLE></HEAD>\r\n", strlen("</TITLE></HEAD>\r\n"), 0);
        sprintf(buffer, "<BODY><P>HTTP/1.1 501  Not Implemented:  %s\r\n", str);
        send(sock_fd, buffer, strlen(buffer), 0);
        send(sock_fd, "</BODY></HTML>\r\n", strlen("</BODY></HTML>\r\n"), 0);
    }
}

/***********************************************************************
    Handle all different file types
***********************************************************************/

void handle_filetype(int client, const char *filename){
    char buf[1024];
    const char* filetype;
    off_t size;
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);
    }
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        filetype = "";
    } else {
        filetype = dot + 1;
    }

    if (strcmp(filetype, "html") == 0) {
        send(client, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"), 0);
        send(client, "Content-Type: text/html\r\n", strlen("Content-Type: text/html\r\n"), 0);
        send(client, "\r\n", strlen("\r\n"), 0);

    } else if (strcmp(filetype, "png") == 0){
        send(client, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"), 0);
        send(client, "Content-Type: image/png\r\n", strlen("Content-Type: image/png\r\n"), 0);
        sprintf(buf, "Content-Length: %ld \r\n", size);
        send(client, buf, strlen(buf), 0);
        send(client, "Content-Transfer-Encoding: binary\r\n", strlen("Content-Transfer-Encoding: binary\r\n"), 0);
        send(client, "\r\n", strlen("\r\n"), 0);

    } else if (strcmp(filetype, "gif") == 0) {
        send(client, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"), 0);
        send(client, "Content-Type: image/gif\r\n", strlen("Content-Type: image/gif\r\n"), 0);
        sprintf(buf, "Content-Length: %ld \r\n", size);
        send(client, buf, strlen(buf), 0);
        send(client, "Content-Transfer-Encoding: binary\r\n", strlen("Content-Transfer-Encoding: binary\r\n"), 0);
        send(client, "\r\n", strlen("\r\n"), 0);
    }
}

/***********************************************************************
    Send a file to client
***********************************************************************/
void send_file(int sock_fd, const char *filepath) {
    char *send_buffer;
    FILE *file;
    long fileLength;
    file = fopen(filepath, "rb");
    if (file == NULL) {
        // file not exist
        send_error(sock_fd, filepath, 404);
        return;
    }
    if (strstr(filepath, ".html") != NULL) {
        FILE *resource = NULL;
        int numchars = 1;
        char buffer[1024];
        char buffer2[1024];
        // fill
        buffer[0] = 'A';
        buffer[1] = '\0';
        while ((numchars > 0) && strcmp("\n", buffer)) {
            numchars = read_line(sock_fd, buffer, sizeof(buffer));
        }
        resource = fopen(filepath, "r");
        handle_filetype(sock_fd, filepath);
        // Read file
        // Send buffer each loop
        fgets(buffer2, sizeof(buffer2), resource);
        while (!feof(resource)) {
            send(sock_fd, buffer2, strlen(buffer2), 0);
            fgets(buffer2, sizeof(buffer2), resource);
        }
        fclose(resource);
    } else {
        // Find file size
        fseek(file, 0, SEEK_END);
        fileLength = ftell(file);
        rewind(file);
        // malloc file size
        send_buffer = (char*) malloc(sizeof(char) * fileLength);
        size_t sb = fread(send_buffer, 1, fileLength, file);
        if (sb > 0) {
            handle_filetype(sock_fd, filepath);
            send(sock_fd, send_buffer, sb, 0);
        } else { // problem read file, error
            send_error(sock_fd, NULL, 500);
        }
    }

    fclose(file);
}

/***********************************************************************
  Handle HTTP request
***********************************************************************/
void handle_request(int conn_req) {
    char buffer[1024];
    char method[32]; // HTTP Method
    char path[1024];
    char protocol[32];
    char *file_ext;
    struct stat statt;
    int read = read_line(conn_req, buffer, sizeof(buffer));
    if (read <= 0) {
        return;
    }
    sscanf(buffer, "%s %s %s", method, path, protocol);
    file_ext = strrchr(path, '.');
    if (file_ext != NULL && strstr(config.content_type, file_ext) == NULL) {
        send_error(conn_req, path, 501);
        return;
    }
    if (strcasecmp(method, "POST") == 0) {
        send_error(conn_req, "Unimplemented Method", 501);
    }
    char full_path[512];
    sprintf(full_path, "%s%s", config.DocumentRoot, path);
    if (full_path[strlen(full_path) - 1] == '/') {
        // if the client requested /, get the directory index and append to current path
        strcat(full_path, config.DirectoryIndex);
    }
    char s1[64], s2[256];
    int numchars;
    if (stat(full_path, &statt) == -1) {
        while ((numchars > 0) && strcmp("\n", buffer)) {
            numchars = read_line(conn_req, buffer, sizeof(buffer));
            sscanf(buffer, "%s %s", s1, s2);
            if (!strcmp("Connection:", s1)){
                printf("%s\n", s2);
            }
        }
        send_error(conn_req, path, 404);
    } else {
        send_file(conn_req, full_path);
    }
    close(conn_req);
}

/***********************************************************************
    Bind to ports and listen to connection request
***********************************************************************/
int listen_port(unsigned int portNum) {
    printf("Listen on port %d\n", portNum);
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd <= 0) {
        printf("Error open socket.\n");
        return -1;
    }
    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        printf("Error in setsockopt.\n");
        return -1;
    }
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNum);
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, serv_addr_len) < 0) {
        close(sock_fd);
        printf("Failed to bind.\n");
        return -1;
    }
    listen(sock_fd, 7);
    while (true) {
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len = sizeof(clnt_addr);
        int conn_req = accept(sock_fd, (struct sockaddr *) &clnt_addr, &clnt_addr_len);
        if (conn_req == -1) {
            printf("Unable accept connection!\n");
        }
        int pid = fork();
        if (pid == -1) {
            printf("Fork process fail!\n");
        } else if (pid == 0) {
            handle_request(conn_req);
            exit(0);
        }
        close(conn_req);
    }
    close(sock_fd);
    return 0;
}

int main(int argc, char* argv[]) {
    printf("Loading config.\n");
    parse_config();
    int ret = -1;
    ret = listen_port(config.port);
    if (ret == -1) {
        printf("Error bind to port %d\n", config.port);
        return 1;
    }
    return 0;
}
