#pragma once

#include <stdint.h>
#include <stdbool.h>

uint8_t disassemble(uint8_t *buf, uint32_t pc, bool *ret_reached);
