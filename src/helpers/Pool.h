#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

const size_t DEFAULT_ALLIGNMENT = 2 * sizeof(void*);

struct Pool
{
public:
	char* buffer;
	size_t offset;
	size_t buffer_size;

	Pool(const size_t& size);
	void* alloc(const size_t& size, const size_t& allignment);
};