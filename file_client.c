#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define NAMED_PIPE_NAME "file_manager_named_pipe"
#define NAMED_PIPE_NAME2 "file_manager_named_pipe2"

int main(int argc, char *argv[]) {
    int fd = open(NAMED_PIPE_NAME, O_WRONLY);
    int fd2 = open(NAMED_PIPE_NAME, O_WRONLY);
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Enter command (create, delete, read, write, exit): ");
        scanf("%s", buffer);
        write(fd, buffer, BUFFER_SIZE);
        if (strcmp(buffer, "create") == 0) {
            printf("Enter file name: ");
            scanf("%s", buffer);
            write(fd, buffer, BUFFER_SIZE);
            read(fd2, buffer, BUFFER_SIZE);
            printf("%s\n", buffer);
        } else if (strcmp(buffer, "delete") == 0) {
            printf("Enter file name: ");
            scanf("%s", buffer);
            write(fd, buffer, BUFFER_SIZE);
            read(fd2, buffer, BUFFER_SIZE);
            printf("%s\n", buffer);
        } else if (strcmp(buffer, "read") == 0) {
            printf("Enter file name: ");
            scanf("%s", buffer);
            write(fd, buffer, BUFFER_SIZE);
            read(fd2, buffer, BUFFER_SIZE);
            printf("%s\n", buffer);
        } else if (strcmp(buffer, "write") == 0) {
            printf("Enter file name: ");
            scanf("%s", buffer);
            write(fd, buffer, BUFFER_SIZE);
            printf("Enter data: ");
            scanf("%s", buffer);
            write(fd, buffer, BUFFER_SIZE);
            read(fd2, buffer, BUFFER_SIZE);
            printf("%s\n", buffer);
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }
    close(fd);
    return 0;
}
