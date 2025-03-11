#include <dlfcn.h>
#include <unistd.h>

void *load_dynamic_function(const char* library_path, const char* function_name) {
    void* handle = dlopen(library_path, RTLD_LAZY);
    void *func = (void *)dlsym(handle, function_name);
    return func;
}

typedef int (*printf_fn)(const char *fmt, ...);

int main() {
    printf_fn ft_printf = load_dynamic_function("libmymalloc.so", "ft_printf");
    if (ft_printf) {
        write(1, "here it is\n", 11);
	    ft_printf("bonjour a tous");
    } else {
        write(1, "none\n", 5);
    }
    return 0;
}
