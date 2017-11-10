#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

uint8_t disassemble(uint8_t *buf, uint32_t pc, bool *ret_reached, int32_t addr_offset);

#ifdef __cplusplus
}
#endif
