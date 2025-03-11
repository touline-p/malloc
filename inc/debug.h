#ifndef DEBUG_H

# define DEBUG_H

#define DEBUG 1

#ifdef DEBUG
# define DEBUG(x) x
#else
# define DEBUG(x) ;
#endif


/*
#define QUOTE(x) #x
#define STRINGIFY(x) QUOTE(x)
#define CONCATENATE_HELPER(a, b, c, d) a b c d
#define CONCATENATE(a, b, c, d) CONCATENATE_HELPER(a, b, c, d)

#define BASIC_FORMAT "%s:%d: %s "
#define FULL_FORMAT(custom_format) CONCATENATE(STRINGIFY(BASIC_FORMAT), custom_format, "\n", )

char *file_name(char *filename) {return strrchr(filename, '/');}

#define TRACE(format, level, ...) \
    printf(FULL_FORMAT(format), file_name(__FILE__), __LINE__, level, __VA_ARGS__)


#define DEBUG(format, ...) TRACE(format, "DEBUG", __VA_ARGS__)
#define INFO(format, ...)  TRACE(format, "INFO ", __VA_ARGS__)
#define WARN(format, ...)  TRACE(format, "WARN ", __VA_ARGS__)
#define ERROR(format, ...) TRACE(format, "ERROR", __VA_ARGS__)
*/

#endif // DEBUG_H

