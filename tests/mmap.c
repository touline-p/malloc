#include "utest.h"
#include "mymalloc.h"
#include <stdbool.h>

UTEST_MAIN()

	/*
UTEST(malloc_return_non_null, youpi) {
	ASSERT_TRUE(1);
	ASSERT_NE(mymalloc(27), NULL);
}

UTEST(malloc_return_readable_and_writable , youpi) {
	int *ptr = mymalloc(sizeof(int));
	int value = 5;
	*ptr = value;
	int bonjour = *ptr;
	ASSERT_EQ(bonjour, value);
}
*/

/*
#include <signal.h>
#include <setjmp.h>

static jmp_buf jump_buffer;
static int segfault_occurred = 0;

void segfault_handler(int signal) {
	segfault_occurred = 1;
	longjmp(jump_buffer, 1);
}

UTEST(SegfaultTest, ShouldCauseSegfault) {
	int *ptr = mymalloc(sizeof(int));
	*ptr = 3;
	signal(SIGSEGV, segfault_handler);
	if (setjmp(jump_buffer) == 0) {
		ptr[1026] = 2;
	}
	ASSERT_TRUE(segfault_occurred);
}
*/
