#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"

#define MEMSZ		512

static void mem_extend(struct mem *mem)
{
	char *s = mem->s;
	mem->sz = mem->sz ? mem->sz + mem->sz : MEMSZ;
	mem->s = malloc(mem->sz);
	if (mem->n)
		memcpy(mem->s, s, mem->n);
	free(s);
}

void mem_init(struct mem *mem)
{
	memset(mem, 0, sizeof(*mem));
}

void mem_done(struct mem *mem)
{
	free(mem->s);
	memset(mem, 0, sizeof(*mem));
}

void mem_cut(struct mem *mem, int pos)
{
	mem->n = pos < mem->n ? pos : mem->n;
}

void mem_cpy(struct mem *mem, int off, void *buf, int len)
{
	memcpy(mem->s + off, buf, len);
}

void mem_put(struct mem *mem, void *buf, int len)
{
	while (mem->n + len + 1 >= mem->sz)
		mem_extend(mem);
	mem_cpy(mem, mem->n, buf, len);
	mem->n += len;
}

void mem_putc(struct mem *mem, int c)
{
	if (mem->n + 2 >= mem->sz)
		mem_extend(mem);
	mem->s[mem->n++] = c;
}

void mem_putz(struct mem *mem, int sz)
{
	while (mem->n + sz + 1 >= mem->sz)
		mem_extend(mem);
	memset(mem->s + mem->n, 0, sz);
	mem->n += sz;
}

/* return a pointer to mem's buffer; valid as long as mem is not modified */
void *mem_buf(struct mem *mem)
{
	if (!mem->s)
		return "";
	mem->s[mem->n] = '\0';
	return mem->s;
}

int mem_len(struct mem *mem)
{
	return mem->n;
}
