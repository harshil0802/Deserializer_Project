#ifndef DESERIALIZE_H
#define DESERIALIZE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "deserializeApi.h"

uint64_t readVarInt(uint8_t** buffer);

void deserializeTransaction(uint8_t* buffer);

#endif