#include "utest.h"
#include "mymalloc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "resetMalloc.h"

struct MaskTestFixture {
	uint64_t header;	
};

UTEST_MAIN()

UTEST_F_SETUP(MaskTestFixture) {
	memset(utest_fixture, 0, sizeof(struct MaskTestFixture));
	uint64_t hdr = 0;

	ASSERT_EQ(memcmp(&hdr, utest_fixture, sizeof(struct MaskTestFixture)), 0);
}

UTEST_F(MaskTestFixture, mask_is_set_is_false) {
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_TRUE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
}

#define TEST_MASK MASK(0x02)

UTEST_F(MaskTestFixture, multiple_mask_can_be_set) {
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, TEST_MASK));
	ASSERT_TRUE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
}

UTEST_F_TEARDOWN(MaskTestFixture) {
}

UTEST_F(resetMalloc, tiny_top_is_not_in_use) {
	char *str = mymalloc(TINY_TEST_SIZE);
	chunk_info_t info = *(chunk_info_t *)arena_g.tiny;

	myfree(str);
	ASSERT_FALSE(mask_is_set(&info, CHUNK_IN_USE));
}

UTEST_F(resetMalloc, allocated_chunk_is_in_use) {
	chunk_info_t *malloced = mymalloc(TINY_TEST_SIZE);
	dprintf(2, "%p\n", malloced);

	malloced = get_header_from_addr(malloced);
	ASSERT_TRUE(mask_is_set(malloced, CHUNK_IN_USE));
}
