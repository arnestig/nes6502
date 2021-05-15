#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();
// Test ADC immediate instruction
TEST(CPU_6502, ADC_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::ADC_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC immediate instruction with overflow
TEST(CPU_6502, ADC_IM_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_IM;
    cpu.mem[0x1003] = 0xFF;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC immediate instruction with overflow resulting in 0
TEST(CPU_6502, ADC_IM_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC ZeroPage
TEST(CPU_6502, ADC_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::ADC_ZP;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x00AC] = 0x4;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC ZeroPage with overflow
TEST(CPU_6502, ADC_ZP_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ZP;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x00AC] = 0xFF;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC ZeroPage with overflow resulting in 0
TEST(CPU_6502, ADC_ZP_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ZP;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x00AC] = 0x1;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test ADC ZeroPage X
TEST(CPU_6502, ADC_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ZP_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x4;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC ZeroPage X with overflow
TEST(CPU_6502, ADC_ZP_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ZP_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC ZeroPage X with overflow resulting in 0
TEST(CPU_6502, ADC_ZP_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ZP_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC Absolute
TEST(CPU_6502, ADC_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::ADC_ABS;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x1004] = 0x10;
    cpu.mem[0x10AC] = 0x4;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute with overflow
TEST(CPU_6502, ADC_ABS_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ABS;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x1004] = 0x10;
    cpu.mem[0x10AC] = 0xFF;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Absolute with overflow resulting in 0
TEST(CPU_6502, ADC_ABS_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ABS;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x1004] = 0x10;
    cpu.mem[0x10AC] = 0x1;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC Absolute X
TEST(CPU_6502, ADC_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x4;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute X, page crossed
TEST(CPU_6502, ADC_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0x27;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xFE;
    cpu.mem[0x1006] = 0x00;
    cpu.mem[0x0125] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute X with overflow
TEST(CPU_6502, ADC_ABS_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Absolute X with overflow resulting in 0
TEST(CPU_6502, ADC_ABS_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test ADC Absolute Y
TEST(CPU_6502, ADC_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x4;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute Y, page crossed
TEST(CPU_6502, ADC_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0x27;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xFE;
    cpu.mem[0x1006] = 0x00;
    cpu.mem[0x0125] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute Y with overflow
TEST(CPU_6502, ADC_ABS_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Absolute Y with overflow resulting in 0
TEST(CPU_6502, ADC_ABS_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test ADC Indirect X
TEST(CPU_6502, ADC_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x001B] = 0x25;
    cpu.mem[0x001C] = 0x40;
    cpu.mem[0x4025] = 0x4;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect X, ZP wrap
TEST(CPU_6502, ADC_IND_X_ZP_WRAP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xCD;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0x32;
    cpu.mem[0x00FF] = 0x25;
    cpu.mem[0x0000] = 0x40;
    cpu.mem[0x4025] = 0x4;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect X with overflow
TEST(CPU_6502, ADC_IND_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x25;
    cpu.mem[0x00BC] = 0x40;
    cpu.mem[0x4025] = 0xFF;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Indirect X with overflow resulting in 0
TEST(CPU_6502, ADC_IND_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x25;
    cpu.mem[0x00BC] = 0x40;
    cpu.mem[0x4025] = 0x1;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC Indirect Y
TEST(CPU_6502, ADC_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4034] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect Y, ZP wrap
TEST(CPU_6502, ADC_IND_Y_ZP_WRAP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xCD;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0x32;
    cpu.mem[0x0032] = 0x80;
    cpu.mem[0x0033] = 0x40;
    cpu.mem[0x414D] = 0x4;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect Y with overflow
TEST(CPU_6502, ADC_IND_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00AC] = 0x25;
    cpu.mem[0x00AD] = 0x40;
    cpu.mem[0x4034] = 0xFF;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Indirect Y with overflow resulting in 0
TEST(CPU_6502, ADC_IND_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00AC] = 0x25;
    cpu.mem[0x00AD] = 0x40;
    cpu.mem[0x4034] = 0x1;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test SBC immediate instruction
TEST(CPU_6502, SBC_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::SBC_IM;
    cpu.mem[0x1003] = 0x2;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC immediate instruction with overflow
TEST(CPU_6502, SBC_IM_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x02;
    cpu.mem[0x1002] = INS::SEC_IM;
    cpu.mem[0x1003] = INS::SBC_IM;
    cpu.mem[0x1004] = 0x10;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC immediate instruction with margin
TEST(CPU_6502, SBC_IM_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC immediate instruction with margin
TEST(CPU_6502, SBC_IM_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_IM;
    cpu.mem[0x1003] = 0xAC;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC ZeroPage instruction
TEST(CPU_6502, SBC_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::SBC_ZP;
    cpu.mem[0x1003] = 0x8B;
    cpu.mem[0x008B] = 0x2;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC ZeroPage instruction with overflow
TEST(CPU_6502, SBC_ZP_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x02;
    cpu.mem[0x1002] = INS::SEC_IM;
    cpu.mem[0x1003] = INS::SBC_ZP;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x0025] = 0x10;
    cpu.execute(7);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage instruction with margin
TEST(CPU_6502, SBC_ZP_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_ZP;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x0025] = 0xAD;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage instruction overflow, result 0
TEST(CPU_6502, SBC_ZP_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_ZP;
    cpu.mem[0x1003] = 0xF0;
    cpu.mem[0x00F0] = 0xAC;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC ZeroPage X instruction
TEST(CPU_6502, SBC_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ZP_X;
    cpu.mem[0x1005] = 0x8B;
    cpu.mem[0x008F] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC ZeroPage X instruction with overflow
TEST(CPU_6502, SBC_ZP_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ZP_X;
    cpu.mem[0x1006] = 0x25;
    cpu.mem[0x0028] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage X instruction with margin
TEST(CPU_6502, SBC_ZP_X_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ZP_X;
    cpu.mem[0x1005] = 0x25;
    cpu.mem[0x0029] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage X instruction overflow, result 0
TEST(CPU_6502, SBC_ZP_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ZP_X;
    cpu.mem[0x1005] = 0xF0;
    cpu.mem[0x00F4] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC Absolute instruction
TEST(CPU_6502, SBC_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2019] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Absolute instruction with overflow
TEST(CPU_6502, SBC_ABS_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ABS;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1007] = 0x20;
    cpu.mem[0x2019] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute instruction with margin
TEST(CPU_6502, SBC_ABS_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2019] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute instruction overflow, result 0
TEST(CPU_6502, SBC_ABS_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2019] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC Absolute X instruction
TEST(CPU_6502, SBC_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Absolute X instruction with page cross
TEST(CPU_6502, SBC_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2110] = 0x2;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}


// Test SBC Absolute X instruction with overflow
TEST(CPU_6502, SBC_ABS_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ABS_X;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1007] = 0x20;
    cpu.mem[0x201C] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute X instruction with margin
TEST(CPU_6502, SBC_ABS_X_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute X instruction overflow, result 0
TEST(CPU_6502, SBC_ABS_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}



// Test SBC Absolute Y instruction
TEST(CPU_6502, SBC_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}


// Test SBC Absolute Y instruction with page cross
TEST(CPU_6502, SBC_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xF7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2110] = 0x2;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Absolute Y instruction with overflow
TEST(CPU_6502, SBC_ABS_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ABS_Y;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1007] = 0x20;
    cpu.mem[0x201C] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute Y instruction with margin
TEST(CPU_6502, SBC_ABS_Y_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute Y instruction overflow, result 0
TEST(CPU_6502, SBC_ABS_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction
TEST(CPU_6502, SBC_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x0010] = 0x25;
    cpu.mem[0x0011] = 0x40;
    cpu.mem[0x4025] = 0x2;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction with overflow
TEST(CPU_6502, SBC_IND_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_IND_X;
    cpu.mem[0x1006] = 0x0C;
    cpu.mem[0x000F] = 0x25;
    cpu.mem[0x0010] = 0x40;
    cpu.mem[0x4025] = 0x10;
    cpu.execute(12);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction with margin
TEST(CPU_6502, SBC_IND_X_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x0010] = 0x25;
    cpu.mem[0x0011] = 0x40;
    cpu.mem[0x4025] = 0xAD;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction overflow, result 0
TEST(CPU_6502, SBC_IND_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x0010] = 0x25;
    cpu.mem[0x0011] = 0x40;
    cpu.mem[0x4025] = 0xAC;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC Indirect Y instruction
TEST(CPU_6502, SBC_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4029] = 0x2;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction with page cross
TEST(CPU_6502, SBC_IND_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xED;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0xBC;
    cpu.mem[0x00BC] = 0x25;
    cpu.mem[0x00BD] = 0x40;
    cpu.mem[0x4112] = 0x2;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction with overflow
TEST(CPU_6502, SBC_IND_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_IND_Y;
    cpu.mem[0x1006] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4028] = 0x10;
    cpu.execute(11);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction with margin
TEST(CPU_6502, SBC_IND_Y_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4029] = 0xAD;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction overflow, result 0
TEST(CPU_6502, SBC_IND_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4029] = 0xAC;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test AND immediate instruction
TEST(CPU_6502, AND_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::AND_IM;
    cpu.mem[0x1003] = 0x11;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND immediate instruction
TEST(CPU_6502, AND_IM_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2A;
    cpu.mem[0x1002] = INS::AND_IM;
    cpu.mem[0x1003] = 0x15;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND immediate instruction
TEST(CPU_6502, AND_IM_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x90;
    cpu.mem[0x1002] = INS::AND_IM;
    cpu.mem[0x1003] = 0xf1;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test AND ZeroPage instruction
TEST(CPU_6502, AND_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::AND_ZP;
    cpu.mem[0x1003] = 0x40;
    cpu.mem[0x0040] = 0x11;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND ZeroPage instruction
TEST(CPU_6502, AND_ZP_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2A;
    cpu.mem[0x1002] = INS::AND_ZP;
    cpu.mem[0x1003] = 0x40;
    cpu.mem[0x0040] = 0x11;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND ZeroPage instruction
TEST(CPU_6502, AND_ZP_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x90;
    cpu.mem[0x1002] = INS::AND_ZP;
    cpu.mem[0x1003] = 0x40;
    cpu.mem[0x0040] = 0xf1;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test AND ZeroPage X instruction
TEST(CPU_6502, AND_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_ZP_X;
    cpu.mem[0x1005] = 0x40;
    cpu.mem[0x0045] = 0x11;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND ZeroPage X instruction
TEST(CPU_6502, AND_ZP_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x2A;
    cpu.mem[0x1004] = INS::AND_ZP_X;
    cpu.mem[0x1005] = 0x40;
    cpu.mem[0x0045] = 0x11;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND ZeroPage X instruction
TEST(CPU_6502, AND_ZP_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x90;
    cpu.mem[0x1004] = INS::AND_ZP_X;
    cpu.mem[0x1005] = 0x40;
    cpu.mem[0x0045] = 0xf1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test AND Absolute instruction
TEST(CPU_6502, AND_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::AND_ABS;
    cpu.mem[0x1003] = 0x15;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x2015] = 0x11;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Absolute instruction
TEST(CPU_6502, AND_ABS_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2A;
    cpu.mem[0x1002] = INS::AND_ABS;
    cpu.mem[0x1003] = 0x15;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x2015] = 0x11;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND Absolute instruction
TEST(CPU_6502, AND_ABS_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x90;
    cpu.mem[0x1002] = INS::AND_ABS;
    cpu.mem[0x1003] = 0x15;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x2015] = 0xf1;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test AND Absolute X instruction
TEST(CPU_6502, AND_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_ABS_X;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2021] = 0x11;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Absolute X instruction with page cross
TEST(CPU_6502, AND_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_ABS_X;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2114] = 0x11;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Absolute X instruction zero result
TEST(CPU_6502, AND_ABS_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x2A;
    cpu.mem[0x1004] = INS::AND_ABS_X;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2024] = 0x11;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND Absolute X instruction negative flag
TEST(CPU_6502, AND_ABS_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x90;
    cpu.mem[0x1004] = INS::AND_ABS_X;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2038] = 0xf1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test AND Absolute Y instruction
TEST(CPU_6502, AND_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_ABS_Y;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2021] = 0x11;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Absolute Y instruction with page cross
TEST(CPU_6502, AND_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_ABS_Y;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2114] = 0x11;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Absolute Y instruction zero result
TEST(CPU_6502, AND_ABS_Y_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x2A;
    cpu.mem[0x1004] = INS::AND_ABS_Y;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2024] = 0x11;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND Absolute Y instruction negative flag
TEST(CPU_6502, AND_ABS_Y_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x90;
    cpu.mem[0x1004] = INS::AND_ABS_Y;
    cpu.mem[0x1005] = 0x15;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2038] = 0xf1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test AND Indirect X instruction
TEST(CPU_6502, AND_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_IND_X;
    cpu.mem[0x1005] = 0x21;
    cpu.mem[0x002D] = 0x25;
    cpu.mem[0x002E] = 0x20;
    cpu.mem[0x2025] = 0x11;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Indirect X instruction zero result
TEST(CPU_6502, AND_IND_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x2A;
    cpu.mem[0x1004] = INS::AND_IND_X;
    cpu.mem[0x1005] = 0x0F;
    cpu.mem[0x001E] = 0x24;
    cpu.mem[0x001F] = 0x20;
    cpu.mem[0x2024] = 0x15;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND Indirect X instruction negative flag
TEST(CPU_6502, AND_IND_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x90;
    cpu.mem[0x1004] = INS::AND_IND_X;
    cpu.mem[0x1005] = 0xF1;
    cpu.mem[0x0014] = 0x15;
    cpu.mem[0x0015] = 0x20;
    cpu.mem[0x2015] = 0xF1;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test AND Indirect Y instruction
TEST(CPU_6502, AND_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_IND_Y;
    cpu.mem[0x1005] = 0xBE;
    cpu.mem[0x00BE] = 0x15;
    cpu.mem[0x00BF] = 0x20;
    cpu.mem[0x2021] = 0x11;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Indirect Y instruction with page cross
TEST(CPU_6502, AND_IND_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x1004] = INS::AND_IND_Y;
    cpu.mem[0x1005] = 0xDA;
    cpu.mem[0x00DA] = 0x15;
    cpu.mem[0x00DB] = 0x20;
    cpu.mem[0x2114] = 0x11;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x1);
    checkCyclesAndException();
}

// Test AND Indirect Y instruction zero result
TEST(CPU_6502, AND_IND_Y_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x2A;
    cpu.mem[0x1004] = INS::AND_IND_Y;
    cpu.mem[0x1005] = 0x8F;
    cpu.mem[0x008F] = 0x15;
    cpu.mem[0x1090] = 0x20;
    cpu.mem[0x2024] = 0x11;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test AND Indirect Y instruction negative flag
TEST(CPU_6502, AND_IND_Y_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x90;
    cpu.mem[0x1004] = INS::AND_IND_Y;
    cpu.mem[0x1005] = 0x05;
    cpu.mem[0x0005] = 0x15;
    cpu.mem[0x0006] = 0x20;
    cpu.mem[0x2038] = 0xf1;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x90);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test BIT ZeroPage instruction, overflow flag
TEST(CPU_6502, BIT_ZP_BIT_6) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xE0;
    cpu.mem[0x1002] = INS::BIT_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0xCF;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.V,0x1);
    checkCyclesAndException();
}

// Test BIT ZeroPage instruction, negative flag
TEST(CPU_6502, BIT_ZP_BIT_7) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x80;
    cpu.mem[0x1002] = INS::BIT_ZP;
    cpu.mem[0x1003] = 0x03;
    cpu.mem[0x0003] = 0x81;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test BIT ZeroPage instruction, zero flag
TEST(CPU_6502, BIT_ZP_BIT_ZERO_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::BIT_ZP;
    cpu.mem[0x1003] = 0x09;
    cpu.mem[0x0009] = 0x04;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test BIT Absolute instruction, overflow flag
TEST(CPU_6502, BIT_ABS_BIT_6) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xE0;
    cpu.mem[0x1002] = INS::BIT_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x2020] = 0xCF;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.V,0x1);
    checkCyclesAndException();
}

// Test BIT Absolute instruction, negative flag
TEST(CPU_6502, BIT_ABS_BIT_7) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x80;
    cpu.mem[0x1002] = INS::BIT_ABS;
    cpu.mem[0x1003] = 0x03;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x2003] = 0x81;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test BIT Absolute instruction, zero flag
TEST(CPU_6502, BIT_ABS_BIT_ZERO_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::BIT_ABS;
    cpu.mem[0x1003] = 0x09;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x2009] = 0x04;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA immediate instruction
TEST(CPU_6502, ORA_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::ORA_IM;
    cpu.mem[0x1003] = 0x09;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA immediate instruction, result zero
TEST(CPU_6502, ORA_IM_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::ORA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA immediate instruction, negative flag
TEST(CPU_6502, ORA_IM_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::ORA_IM;
    cpu.mem[0x1003] = 0xFF;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ORA ZeroPage instruction
TEST(CPU_6502, ORA_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::ORA_ZP;
    cpu.mem[0x1003] = 0x13;
    cpu.mem[0x0013] = 0x09;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA ZeroPage instruction, result zero
TEST(CPU_6502, ORA_ZP_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::ORA_ZP;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x0025] = 0x0;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA ZeroPage instruction, negative flag
TEST(CPU_6502, ORA_ZP_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::ORA_ZP;
    cpu.mem[0x1003] = 0x75;
    cpu.mem[0x0075] = 0xFF;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ORA ZeroPage, X instruction
TEST(CPU_6502, ORA_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_ZP_X;
    cpu.mem[0x1005] = 0x13;
    cpu.mem[0x001A] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA ZeroPage, X instruction, result zero
TEST(CPU_6502, ORA_ZP_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ZP_X;
    cpu.mem[0x1005] = 0x25;
    cpu.mem[0x002C] = 0x0;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA ZeroPage, X instruction, negative flag
TEST(CPU_6502, ORA_ZP_X_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ZP_X;
    cpu.mem[0x1005] = 0x75;
    cpu.mem[0x007C] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ORA Absolute instruction
TEST(CPU_6502, ORA_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_ABS;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3010] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Absolute instruction, result zero
TEST(CPU_6502, ORA_ABS_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ABS;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3010] = 0x0;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA Absolute instruction, negative flag
TEST(CPU_6502, ORA_ABS_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ABS;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3010] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ORA Absolute X instruction
TEST(CPU_6502, ORA_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Absolute X instruction, page cross
TEST(CPU_6502, ORA_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x310C] = 0x09;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}


// Test ORA Absolute X instruction, result zero
TEST(CPU_6502, ORA_ABS_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x0;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA Absolute X instruction, negative flag
TEST(CPU_6502, ORA_ABS_X_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test ORA Absolute Y instruction
TEST(CPU_6502, ORA_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Absolute Y instruction, page cross
TEST(CPU_6502, ORA_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x310C] = 0x09;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Absolute Y instruction, result zero
TEST(CPU_6502, ORA_ABS_Y_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x0;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA Absolute Y instruction, negative flag
TEST(CPU_6502, ORA_ABS_Y_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}



// Test ORA Indirect X instruction
TEST(CPU_6502, ORA_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_IND_X;
    cpu.mem[0x1005] = 0xB5;
    cpu.mem[0x00BC] = 0x10;
    cpu.mem[0x00BD] = 0x30;
    cpu.mem[0x3010] = 0x09;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Indirect X instruction, result zero
TEST(CPU_6502, ORA_IND_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_IND_X;
    cpu.mem[0x1005] = 0xB5;
    cpu.mem[0x00BC] = 0x10;
    cpu.mem[0x00BD] = 0x30;
    cpu.mem[0x3010] = 0x0;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA Indirect X instruction, negative flag
TEST(CPU_6502, ORA_IND_X_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_IND_X;
    cpu.mem[0x1005] = 0xB5;
    cpu.mem[0x00BC] = 0x10;
    cpu.mem[0x00BD] = 0x30;
    cpu.mem[0x3010] = 0xFF;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ORA Indirect Y instruction
TEST(CPU_6502, ORA_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x3017] = 0x09;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Indirect Y instruction, page cross
TEST(CPU_6502, ORA_IND_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::ORA_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x310C] = 0x09;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x2B);
    checkCyclesAndException();
}

// Test ORA Indirect Y instruction, result zero
TEST(CPU_6502, ORA_IND_Y_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x3017] = 0x0;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test ORA Indirect Y instruction, negative flag
TEST(CPU_6502, ORA_IND_Y_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::ORA_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x3017] = 0xFF;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test EOR immediate instruction
TEST(CPU_6502, EOR_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::EOR_IM;
    cpu.mem[0x1003] = 0x09;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR immediate instruction, result zero
TEST(CPU_6502, EOR_IM_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x1;
    cpu.mem[0x1002] = INS::EOR_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR immediate instruction, negative flag
TEST(CPU_6502, EOR_IM_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xF0;
    cpu.mem[0x1002] = INS::EOR_IM;
    cpu.mem[0x1003] = 0x70;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test EOR ZeroPage instruction
TEST(CPU_6502, EOR_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::EOR_ZP;
    cpu.mem[0x1003] = 0x13;
    cpu.mem[0x0013] = 0x09;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR ZeroPage instruction, result zero
TEST(CPU_6502, EOR_ZP_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x1;
    cpu.mem[0x1002] = INS::EOR_ZP;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x0025] = 0x1;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR ZeroPage instruction, negative flag
TEST(CPU_6502, EOR_ZP_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xF0;
    cpu.mem[0x1002] = INS::EOR_ZP;
    cpu.mem[0x1003] = 0x75;
    cpu.mem[0x0075] = 0x70;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test EOR ZeroPage, X instruction
TEST(CPU_6502, EOR_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_ZP_X;
    cpu.mem[0x1005] = 0x13;
    cpu.mem[0x001A] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR ZeroPage, X instruction, result zero
TEST(CPU_6502, EOR_ZP_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.mem[0x1004] = INS::EOR_ZP_X;
    cpu.mem[0x1005] = 0x25;
    cpu.mem[0x002C] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR ZeroPage, X instruction, negative flag
TEST(CPU_6502, EOR_ZP_X_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xF0;
    cpu.mem[0x1004] = INS::EOR_ZP_X;
    cpu.mem[0x1005] = 0x75;
    cpu.mem[0x007C] = 0x70;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test EOR Absolute instruction
TEST(CPU_6502, EOR_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_ABS;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3010] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Absolute instruction, result zero
TEST(CPU_6502, EOR_ABS_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.mem[0x1004] = INS::EOR_ABS;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3010] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR Absolute instruction, negative flag
TEST(CPU_6502, EOR_ABS_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xF0;
    cpu.mem[0x1004] = INS::EOR_ABS;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3010] = 0x70;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test EOR Absolute X instruction
TEST(CPU_6502, EOR_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Absolute X instruction, page cross
TEST(CPU_6502, EOR_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x310C] = 0x09;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}


// Test EOR Absolute X instruction, result zero
TEST(CPU_6502, EOR_ABS_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.mem[0x1004] = INS::EOR_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR Absolute X instruction, negative flag
TEST(CPU_6502, EOR_ABS_X_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xF0;
    cpu.mem[0x1004] = INS::EOR_ABS_X;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x70;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test EOR Absolute Y instruction
TEST(CPU_6502, EOR_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x09;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Absolute Y instruction, page cross
TEST(CPU_6502, EOR_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x310C] = 0x09;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Absolute Y instruction, result zero
TEST(CPU_6502, EOR_ABS_Y_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.mem[0x1004] = INS::EOR_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR Absolute Y instruction, negative flag
TEST(CPU_6502, EOR_ABS_Y_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xF0;
    cpu.mem[0x1004] = INS::EOR_ABS_Y;
    cpu.mem[0x1005] = 0x10;
    cpu.mem[0x1006] = 0x30;
    cpu.mem[0x3017] = 0x70;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test EOR Indirect X instruction
TEST(CPU_6502, EOR_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_IND_X;
    cpu.mem[0x1005] = 0xB5;
    cpu.mem[0x00BC] = 0x10;
    cpu.mem[0x00BD] = 0x30;
    cpu.mem[0x3010] = 0x09;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Indirect X instruction, result zero
TEST(CPU_6502, EOR_IND_X_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.mem[0x1004] = INS::EOR_IND_X;
    cpu.mem[0x1005] = 0xB5;
    cpu.mem[0x00BC] = 0x10;
    cpu.mem[0x00BD] = 0x30;
    cpu.mem[0x3010] = 0x1;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR Indirect X instruction, negative flag
TEST(CPU_6502, EOR_IND_X_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::EOR_IND_X;
    cpu.mem[0x1005] = 0xB5;
    cpu.mem[0x00BC] = 0x10;
    cpu.mem[0x00BD] = 0x30;
    cpu.mem[0x3010] = 0xFF;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test EOR Indirect Y instruction
TEST(CPU_6502, EOR_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x3017] = 0x09;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Indirect Y instruction, page cross
TEST(CPU_6502, EOR_IND_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x23;
    cpu.mem[0x1004] = INS::EOR_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x310C] = 0x09;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x2A);
    checkCyclesAndException();
}

// Test EOR Indirect Y instruction, result zero
TEST(CPU_6502, EOR_IND_Y_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.mem[0x1004] = INS::EOR_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x3017] = 0x1;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.Z,0x1);
    checkCyclesAndException();
}

// Test EOR Indirect Y instruction, negative flag
TEST(CPU_6502, EOR_IND_Y_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x0;
    cpu.mem[0x1004] = INS::EOR_IND_Y;
    cpu.mem[0x1005] = 0xA2;
    cpu.mem[0x00A2] = 0x10;
    cpu.mem[0x00A3] = 0x30;
    cpu.mem[0x3017] = 0xFF;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

