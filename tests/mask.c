#include "utest.h"
#include "mymalloc.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct MaskTestFixture {
	uint64_t header;	
};

UTEST_MAIN()

UTEST_F_SETUP(MaskTestFixture) {
	memset(&utest_fixture, 0, sizeof(struct MaskTestFixture));
	uint64_t hdr = 0;

	ASSERT_EQ(memcmp(&hdr, &utest_fixture, sizeof(struct MaskTestFixture)), 0);
}

UTEST_F(MaskTestFixture, mask_is_set_is_false) {
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_TRUE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
}

UTEST_F(MaskTestFixture, multiple_mask_can_be_set) {
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, MMAPED));
	ASSERT_TRUE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
	toggle_mask(&utest_fixture->header, PREV_IN_USE);
	ASSERT_FALSE(mask_is_set(&utest_fixture->header, PREV_IN_USE));
}

UTEST_F_TEARDOWN(MaskTestFixture) {
}
