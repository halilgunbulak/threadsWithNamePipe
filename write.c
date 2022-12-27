#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE_NAME "my_named_pipe"
#define PIPE_NAME2 "my_named_pipe2"
#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
  // Open the named pipe for writing
  int fd = open(PIPE_NAME, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
 
  // Open the named pipe for reading
  int fd2 = open(PIPE_NAME2, O_RDONLY);
  if (fd2 < 0) {
    perror("open pipe2");
    return 1;
  }
  // Allocate a buffer for reading from stdin
  char* buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("malloc");
    return 1;
  }
    char command[BUFFER_SIZE];
    
  // Read data from stdin and write it to the named pipe
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    ssize_t bytes_written = write(fd, buffer, strlen(buffer));
    sscanf(buffer, "%s", command);
    if (bytes_written < 0 || strcmp(command,"exit")==0) {
      perror("write");
      break;
    }
    ssize_t bytes_read = read(fd2, buffer, BUFFER_SIZE);
    printf("buffer: %s",buffer);
    if (bytes_read < 0) {
      perror("read");
      break;
    }
    if (bytes_read == 0) {
      break;
    }
    char command[BUFFER_SIZE];
     sscanf(buffer, "%s", command);
    if(strcmp(command,"exit")==0){
        printf("Proggress exit : worked exit command");
        break;
    }
  }

  // Clean up resources
  free(buffer);
  close(fd);
  return 0;
}