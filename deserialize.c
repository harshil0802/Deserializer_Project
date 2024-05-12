#include "deserialize.h"

uint64_t readVarInt(uint8_t** buffer) {
    uint8_t firstByte = **buffer;
    (*buffer)++;
    if (firstByte < 0xfd) {
        return firstByte;
    } else if (firstByte == 0xfd) {
        uint16_t value;
        memcpy(&value, *buffer, sizeof(uint16_t));
        *buffer += sizeof(uint16_t);
        return value;
    } else if (firstByte == 0xfe) {
        uint32_t value;
        memcpy(&value, *buffer, sizeof(uint32_t));
        *buffer += sizeof(uint32_t);
        return value;
    } else {
        uint64_t value;
        memcpy(&value, *buffer, sizeof(uint64_t));
        *buffer += sizeof(uint64_t);
        return value;
    }
}

void deserializeTransaction(uint8_t* buffer) {
    BitcoinTransaction tx;

    tx.version = *((uint32_t*)buffer);
    buffer += 4;

    tx.numInputs = readVarInt(&buffer);
    tx.inputs = malloc(tx.numInputs * sizeof(Input));

    for (size_t i = 0; i < tx.numInputs; i++) {
        memcpy(tx.inputs[i].prevTxHash, buffer, 32);
        buffer += 32;
        tx.inputs[i].prevOutputIndex = *((uint32_t*)buffer);
        buffer += 4;
        tx.inputs[i].scriptLength = readVarInt(&buffer);
        tx.inputs[i].script = malloc(tx.inputs[i].scriptLength + 1);
        memcpy(tx.inputs[i].script, buffer, tx.inputs[i].scriptLength);
        tx.inputs[i].script[tx.inputs[i].scriptLength] = '\0';
        buffer += tx.inputs[i].scriptLength;
        tx.inputs[i].sequence = *((uint32_t*)buffer);
        buffer += 4;
    }

    tx.numOutputs = readVarInt(&buffer);
    tx.outputs = malloc(tx.numOutputs * sizeof(Output));

    for (size_t i = 0; i < tx.numOutputs; i++) {
        tx.outputs[i].value = *((uint64_t*)buffer);
        buffer += 8;
        tx.outputs[i].scriptLength = readVarInt(&buffer);
        tx.outputs[i].script = malloc(tx.outputs[i].scriptLength + 1);
        memcpy(tx.outputs[i].script, buffer, tx.outputs[i].scriptLength);
        tx.outputs[i].script[tx.outputs[i].scriptLength] = '\0';
        buffer += tx.outputs[i].scriptLength;
    }

    tx.lockTime = *((uint32_t*)buffer);

    printf("Transaction Version: %u\n", tx.version);
    printf("Inputs:\n");
    for (size_t i = 0; i < tx.numInputs; i++) {
        printf("  Input %zu: Previous Tx Hash: ", i);
        for (size_t j = 0; j < 32; j++) {
            printf("%02x", tx.inputs[i].prevTxHash[j]);
        }
        printf(", Previous Output Index: %u, Script Length: %llu, Sequence: %u\n",
               tx.inputs[i].prevOutputIndex, (unsigned long long)tx.inputs[i].scriptLength, tx.inputs[i].sequence);
    }
    printf("Outputs:\n");
    for (size_t i = 0; i < tx.numOutputs; i++) {
        printf("  Output %zu: Value: %llu, Script Length: %llu\n",
               i, (unsigned long long)tx.outputs[i].value, (unsigned long long)tx.outputs[i].scriptLength);
    }
    printf("Lock Time: %u\n", tx.lockTime);

    for (size_t i = 0; i < tx.numInputs; i++) {
        free(tx.inputs[i].script);
    }
    free(tx.inputs);
    for (size_t i = 0; i < tx.numOutputs; i++) {
        free(tx.outputs[i].script);
    }
    free(tx.outputs);
}

int main() {
    uint8_t buffer[] = {
        0x01, 0x00, 0x00, 0x00,             // Version
        0x02,                               // Number of inputs
        // Input 1
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Previous Tx Hash
        0x00, 0x00, 0x00, 0x00,             // Previous Output Index
        0x00,                               // Script Length
        0x00, 0x00, 0x00, 0x00,             // Sequence
        // Input 2
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Previous Tx Hash
        0x00, 0x00, 0x00, 0x00,             // Previous Output Index
        0x00,                               // Script Length
        0x00, 0x00, 0x00, 0x00,             // Sequence
        0x02,                               // Number of outputs
        // Output 1
        0x00, 0xe1, 0xf5, 0x05, 0x00, 0x00, 0x00, 0x00, // Value (50000000 Satoshi)
        0x00,                               // Script Length
        // Output 2
        0x00, 0xc2, 0xeb, 0x0b, 0x00, 0x00, 0x00, 0x00, // Value (30000000 Satoshi)
        0x00,                               // Script Length
        0x00, 0x00, 0x00, 0x00,             // Lock Time
    };

    deserializeTransaction(buffer);

    return 0;
}
