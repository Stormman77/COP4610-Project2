#include <unistd.h> 

int main() {
    pid_t pid = getpid();
    getppid();

    const char *msg = "Hello from write()\n";
    write(1, msg, 18);

    char buffer[50];
    ssize_t bytes = read(0, buffer, sizeof(buffer)-1);

    close(2);  

    return 0;
}