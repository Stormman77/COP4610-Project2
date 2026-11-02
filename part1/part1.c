#include <unistd.h> 

int main() {
    pid_t pid = getpid();
    getppid();

    const char *msg = "Hello from write()\n";
    write(1, msg, 19);

    getuid();

    close(2);  

    return 0;
}