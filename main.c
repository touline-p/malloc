#include <dlfcn.h>
#include <unistd.h>
#include "malloc.h"

int main() {
	char * addr = malloc(25);
	free(addr);
}
