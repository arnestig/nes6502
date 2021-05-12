#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

// Test STA zero page instruction
TEST(CPU_6502,STA_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::STA_ZP;
    cpu.mem[0x1003] = 0x35;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x35],0x23);
    checkCyclesAndException();
}

// Test STA zero page X instruction
TEST(CPU_6502,STA_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0x8;
    cpu.mem[0x1004] = INS::STA_ZP_X;
    cpu.mem[0x1005] = 0x35;
    cpu.execute(8);
    EXPECT_EQ(cpu.mem[0x3D],0x23);
    checkCyclesAndException();
}

// Test STA absolute instruction
TEST(CPU_6502,STA_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::STA_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x1920],0x23);
    checkCyclesAndException();
}

// Test STA absolute X instruction
TEST(CPU_6502,STA_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0x8;
    cpu.mem[0x1004] = INS::STA_ABS_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x1928],0x23);
    checkCyclesAndException();
}

// Test STA absolute Y instruction
TEST(CPU_6502,STA_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xC;
    cpu.mem[0x1004] = INS::STA_ABS_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x192C],0x23);
    checkCyclesAndException();
}

// Test STA indirect X instruction
TEST(CPU_6502,STA_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xC;
    cpu.mem[0x1004] = INS::STA_IND_X;
    cpu.mem[0x1005] = 0xC0;
    cpu.mem[0x00CC] = 0x40;
    cpu.mem[0x00CD] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.mem[0x1040],0x23);
    checkCyclesAndException();
}

// Test STA indirect Y instruction
TEST(CPU_6502,STA_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xC;
    cpu.mem[0x1004] = INS::STA_IND_Y;
    cpu.mem[0x1005] = 0xC0;
    cpu.mem[0x00C0] = 0x40;
    cpu.mem[0x00C1] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.mem[0x104C],0x23);
    checkCyclesAndException();
}

// Test STX zero page instruction
TEST(CPU_6502,STX_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::STX_ZP;
    cpu.mem[0x1003] = 0x35;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x35],0x23);
    checkCyclesAndException();
}

// Test STX zero page Y instruction
TEST(CPU_6502,STX_ZP_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::STX_ZP_Y;
    cpu.mem[0x1005] = 0x35;
    cpu.execute(8);
    EXPECT_EQ(cpu.mem[0x45],0x23);
    checkCyclesAndException();
}

// Test STX absolute instruction
TEST(CPU_6502,STX_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::STX_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x1920],0x23);
    checkCyclesAndException();
}

// Test STY zero page instruction
TEST(CPU_6502,STY_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::STY_ZP;
    cpu.mem[0x1003] = 0x35;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x35],0x23);
    checkCyclesAndException();
}

// Test STY zero page X instruction
TEST(CPU_6502,STY_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::STY_ZP_X;
    cpu.mem[0x1005] = 0x35;
    cpu.execute(8);
    EXPECT_EQ(cpu.mem[0x45],0x23);
    checkCyclesAndException();
}

// Test STY absolute instruction
TEST(CPU_6502,STY_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x23;
    cpu.mem[0x1002] = INS::STY_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x1920],0x23);
    checkCyclesAndException();
}

