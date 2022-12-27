// file_manager.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_LEN 64
int send_fd;
// Struct for storing information about a file
typedef struct {
  char name[FILE_NAME_MAX_LEN];
  int fd;
} File;

// Array for storing information about all the files being managed
File files[10];

// Mutex for synchronizing access to the files array
pthread_mutex_t files_mutex;

// Function for handling communication with a single client
void* handle_client(void* arg) {
  int client_fd = *((int*)arg);

  char buffer[BUFFER_SIZE];

  while (1) {
    // Read a message from the client
    int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
    if (bytes_read == 0) {
      // Client disconnected
      break;
    }

    // Parse the message
    char* command = strtok(buffer, " ");
    printf("command=%s",command);
    char* file_name = strtok(NULL, " ");
    char* data = strtok(NULL, " ");

    int result = 0;
    int file_index = -1;

    // Find the index of the file in the files array
    pthread_mutex_lock(&files_mutex);
    for (int i = 0; i < 10; i++) {
      if (strcmp(files[i].name, file_name) == 0) {
        file_index = i;
        break;
      }
    }
    pthread_mutex_unlock(&files_mutex);

    if (strcmp(command, "create") == 0) {
        printf("create'e girdi");
      // Check if the file already exists
      if (file_index != -1) {
        result = -1;
      } else {
        // Create the file and add it to the files array
        int fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (fd < 0) {
          result = -1;
        } else {
          strcpy(files[file_index].name, file_name);
          files[file_index].fd = fd;
        }
      }
    } else if (strcmp(command, "read") == 0) {
      // Check if the file exists
      if (file_index == -1) {
        result = -1;
      } else {
        // Read the contents of the file
        lseek(files[file_index].fd, 0, SEEK_SET);
        int bytes_read = read(files[file_index].fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0) {
          result = -1;
        } else {
          buffer[bytes_read] = '\0';
        }
      }
    } else if (strcmp(command, "write") == 0) {
      // Check if the file exists
      if (file_index == -1) {
        result = -1;
      } else {
        // Write the data to the file
        lseek(files[file_index].fd, 0, SEEK_END);
        int bytes_written = write(files[file_index].fd, data, strlen(data));
        if (bytes_written < 0) {
          result = -1;
        }
      }
    } else if (strcmp(command, "exit") == 0) {
      // Disconnect the client
      printf("exita girdi");
      break;
    } else {
      // Invalid command
      result = -1;
    }

    // Send the result of the operation back to the client
    sprintf(buffer, "%d", result);
    write(client_fd, buffer, strlen(buffer));
  }

  // Close the client's file descriptor
  close(client_fd);

  return NULL;
}

int main() {
  // Initialize the files array
  for (int i = 0; i < 10; i++) {
    files[i].name[0] = '\0';
    files[i].fd = -1;
  }

  // Initialize the mutex
  pthread_mutex_init(&files_mutex, NULL);

  // Create the named pipe
  mkfifo("file_manager_named_pipe", 0666);
  mkfifo("file_manager_named_pipe2", 0666);
  while (1) {
    // Open the named pipe for reading
    int pipe_fd = open("file_manager_named_pipe", O_RDONLY);
    printf("pipe.fd:%d",pipe_fd);
    // Accept a connection from a client
    int client_fd = read(pipe_fd, NULL, 0);
     send_fd = read(pipe_fd, NULL, 0);
     printf("pipe.fd:%d",client_fd);
    // Create a new thread for handling the client's requests
    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, &client_fd);
  }

  return 0;
}
      