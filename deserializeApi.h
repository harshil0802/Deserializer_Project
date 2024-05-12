#ifndef DESERIALIZE_API_H
#define DESERIALIZE_API_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a Bitcoin transaction input
typedef struct {
    uint8_t prevTxHash[32];
    uint32_t prevOutputIndex;
    uint64_t scriptLength;
    uint8_t* script;
    uint32_t sequence;
} Input;

// Structure to represent a Bitcoin transaction output
typedef struct {
    uint64_t value;
    uint64_t scriptLength;
    uint8_t* script;
} Output;

// Structure to represent a Bitcoin transaction
typedef struct {
    uint32_t version;
    size_t numInputs;
    Input* inputs;
    size_t numOutputs;
    Output* outputs;
    uint32_t lockTime;
} BitcoinTransaction;

#endif /* DESERIALIZE_API_H */
