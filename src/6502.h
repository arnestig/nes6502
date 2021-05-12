#ifndef __6502_H__
#define __6502_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 0x10000

enum INS
{
    // LDA
    LDA_IM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZP_X = 0xB5,
    LDA_ABS = 0xAD,
    LDA_ABS_X = 0xBD,
    LDA_ABS_Y = 0xB9,
    LDA_IND_X = 0xA1,
    LDA_IND_Y = 0xB1,

    // LDX
    LDX_IM = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZP_Y = 0xB6,
    LDX_ABS = 0xAE,
    LDX_ABS_Y = 0xBE,

    // LDY
    LDY_IM = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZP_X = 0xB4,
    LDY_ABS = 0xAC,
    LDY_ABS_X = 0xBC,

    // STA
    STA_ZP = 0x85,
    STA_ZP_X = 0x95,
    STA_ABS = 0x8D,
    STA_ABS_X = 0x9D,
    STA_ABS_Y = 0x99,
    STA_IND_X = 0x81,
    STA_IND_Y = 0x91,

    // STX
    STX_ZP = 0x86,
    STX_ZP_Y = 0x96,
    STX_ABS = 0x8E,

    // STY
    STY_ZP = 0x84,
    STY_ZP_X = 0x94,
    STY_ABS = 0x8C,

    // Clear/set flags
    CLC_IM = 0x18,
    CLD_IM = 0xD8,
    CLI_IM = 0x58,
    CLV_IM = 0xB8,
    SEC_IM = 0x38,
    SED_IM = 0xF8,
    SEI_IM = 0x78,

    // NOP
    NOP_IM = 0xEA,

    // Transfer instructions
    TAX_IM = 0xAA,
    TAY_IM = 0xA8,
    TSX_IM = 0xBA,
    TXA_IM = 0x8A,
    TXS_IM = 0x9A,
    TYA_IM = 0x98,

    //dec instructions
    DEX_IM = 0xCA,
    DEY_IM = 0x88,
    DEC_ZP = 0xC6,
    DEC_ZP_X = 0xD6,
    DEC_ABS = 0xCE,
    DEC_ABS_X = 0xDE,

    //dec instructions
    INX_IM = 0xE8,
    INY_IM = 0xC8,
    INC_ZP = 0xE6,
    INC_ZP_X = 0xF6,
    INC_ABS = 0xEE,
    INC_ABS_X = 0xFE,

    // Jump instructions
    JMP_ABS = 0x4C,
    JMP_IND = 0x6C,
    JSR_ABS = 0x20,
    RTS_IMP = 0x60,
    RTI_IMP = 0x40,

    // Push instructions
    PHA_IMP = 0x48,
    PHP_IMP = 0x08,

    // pull instructions
    PLA_IMP = 0x68,
    PLP_IMP = 0x28,

    // branch instructions
    BCC_REL = 0x90,
    BCS_REL = 0xB0,
    BEQ_REL = 0xF0,
    BMI_REL = 0x30,
    BNE_REL = 0xD0,
    BPL_REL = 0x10,
    BVC_REL = 0x50,
    BVS_REL = 0x70,

    // BRK
    BRK_IMP = 0x00,

    // Arithmetic Shift Left
    ASL_ACC = 0x0A,
    ASL_ZP = 0x06,
    ASL_ZP_X = 0x16,
    ASL_ABS = 0x0E,
    ASL_ABS_X = 0x1E,

    // Logical Shift Right
    LSR_ACC = 0x4A,
    LSR_ZP = 0x46,
    LSR_ZP_X = 0x56,
    LSR_ABS = 0x4E,
    LSR_ABS_X = 0x5E,

    // Rotate Left
    ROL_ACC = 0x2A,
    ROL_ZP = 0x26,
    ROL_ZP_X = 0x36,
    ROL_ABS = 0x2E,
    ROL_ABS_X = 0x3E,

    // Rotate Right
    ROR_ACC = 0x6A,
    ROR_ZP = 0x66,
    ROR_ZP_X = 0x76,
    ROR_ABS = 0x6E,
    ROR_ABS_X = 0x7E,
};

struct CPU
{
    uint8_t mem[MEM_SIZE];
    uint16_t PC; // program counter
    uint8_t S; // stack pointer

    uint8_t cycles;

    bool exception; // flag only used for unit tests

    // Processor status bits
    struct {
        uint8_t N : 1; // negative
        uint8_t V : 1; // overflow
        uint8_t B : 1; // break
        uint8_t D : 1; // deccimal mode
        uint8_t I : 1; // interrupt disable mode
        uint8_t Z : 1; // zero flag
        uint8_t C : 1; // carry flag
    } P;

    uint8_t A; // accumulator
    uint8_t X; // X register
    uint8_t Y; // Y register


    void branchInstruction( bool takeBranch );

    void setStatusBits( uint8_t byte );

    uint8_t getStatusByte();

    void reset();

    // read one byte from mem and increment program counter
    uint8_t readByte();

    // dump memory at address + 20 bytes
    void dumpMemory( uint16_t addr );

    // dump memory at current PC +- 10 bytes
    void dumpRegister();

    // dump the stack
    void dumpStack();

    void A_status_flags();
    void X_status_flags();
    void Y_status_flags();
    void M_status_flags( uint8_t M );

    void execute(int8_t c);
};

#endif
