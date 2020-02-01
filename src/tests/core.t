#pragma once

#define SET_BITS(N, V, E, B, D, I, Z, C)\
	sixtyfiveohtwo.c(C);\
	sixtyfiveohtwo.z(Z);\
	sixtyfiveohtwo.i(I);\
	sixtyfiveohtwo.d(D);\
	sixtyfiveohtwo.b(B);\
	sixtyfiveohtwo.e(E);\
	sixtyfiveohtwo.v(V);\
	sixtyfiveohtwo.n(N);

#define ASSERT_BITS(N, V, E, B, D, I, Z, C)\
	assert(sixtyfiveohtwo.c() == C);\
	assert(sixtyfiveohtwo.z() == Z);\
	assert(sixtyfiveohtwo.i() == I);\
	assert(sixtyfiveohtwo.d() == D);\
	assert(sixtyfiveohtwo.b() == B);\
	assert(sixtyfiveohtwo.e() == E);\
	assert(sixtyfiveohtwo.v() == V);\
	assert(sixtyfiveohtwo.n() == N);


	SET_BITS(0,0,0,0,0,0,0,0)
	assert(sixtyfiveohtwo.p() == 0x00u);
	SET_BITS(1,1,1,1,1,1,1,1)
	assert(sixtyfiveohtwo.p() == 0xFFu);

	SET_BITS(0,0,0,0,1,1,1,1)
	assert(sixtyfiveohtwo.p() == 0x0Fu);
	SET_BITS(1,1,1,1,0,0,0,0)
	assert(sixtyfiveohtwo.p() == 0xF0u);


	SET_BITS(0,0,1,1,0,0,1,1)
	assert(sixtyfiveohtwo.p() == 0x33u);
	SET_BITS(1,1,0,0,1,1,0,0)
	assert(sixtyfiveohtwo.p() == 0xCCu);


	SET_BITS(0,1,0,1,0,1,0,1)
	assert(sixtyfiveohtwo.p() == 0x55u);
	SET_BITS(1,0,1,0,1,0,1,0)
	assert(sixtyfiveohtwo.p() == 0xAAu);


	sixtyfiveohtwo.p(0xff);
	ASSERT_BITS(1,1,1,1,1,1,1,1);
	sixtyfiveohtwo.p(0x00);
	ASSERT_BITS(0,0,0,0,0,0,0,0);

	sixtyfiveohtwo.p(0xf0);
	ASSERT_BITS(1,1,1,1,0,0,0,0);
	sixtyfiveohtwo.p(0x0f);
	ASSERT_BITS(0,0,0,0,1,1,1,1);

	sixtyfiveohtwo.p(0xCC);
	ASSERT_BITS(1,1,0,0,1,1,0,0);
	sixtyfiveohtwo.p(0x33);
	ASSERT_BITS(0,0,1,1,0,0,1,1);

	sixtyfiveohtwo.p(0xAA);
	ASSERT_BITS(1,0,1,0,1,0,1,0);
	sixtyfiveohtwo.p(0x55);
	ASSERT_BITS(0,1,0,1,0,1,0,1);
