#include "utest.h"
#include <stdint.h>
#include "type.h"
#include "maskmanipulation.h"

UTEST_MAIN();

struct chunkInfoFixture {
	chunk_info_t *a;
	chunk_info_t *b;
};

UTEST_F_SETUP(chunkInfoFixture) {
	utest_fixture->a = malloc(sizeof(uint64_t) * 3);
	utest_fixture->b = malloc(sizeof(uint64_t) * 3);
	memset(utest_fixture->a, 0, sizeof(chunk_info_t));
	CHNK_CLR(utest_fixture->b);
	ASSERT_EQ(memcmp(utest_fixture->a, utest_fixture->b, sizeof(chunk_info_t)), 0);
}

UTEST_F_TEARDOWN(chunkInfoFixture) {
	free(utest_fixture->a);
	free(utest_fixture->b);
}


UTEST_F(chunkInfoFixture, assign_size) {
	uint64_t cmp = 5;
	set_size(utest_fixture->a, cmp);
	ASSERT_EQ(get_size(utest_fixture->a), cmp);
}

UTEST_F(chunkInfoFixture, get_size) {
	uint64_t cmp = 5;
	set_size(utest_fixture->a, cmp);
	ASSERT_EQ(get_size(utest_fixture->a), cmp);
}

UTEST_F(chunkInfoFixture, get_size_with_masked_info) {
	uint64_t cmp = 5;
	set_size(utest_fixture->a, cmp);
	set_size(utest_fixture->b, cmp);
	toggle_mask(utest_fixture->a, PREV_IN_USE);
	ASSERT_TRUE(mask_is_set(utest_fixture->a, PREV_IN_USE));
	ASSERT_FALSE(mask_is_set(utest_fixture->b, PREV_IN_USE));
	ASSERT_EQ(get_size(utest_fixture->a),get_size(utest_fixture->b));
}

UTEST_F(chunkInfoFixture, use_size_is_working) {
}







