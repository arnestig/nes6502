#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

// Test TAX immediate instruction
TEST(CPU_6502, TAX) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::TAX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x25);
    checkCyclesAndException();
}

// Test TAX immediate instruction, X = 0
TEST(CPU_6502, TAX_X_0) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::TAX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test TAX immediate instruction, negative flag
TEST(CPU_6502, TAX_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::TAX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0xFC);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}

// Test TAY immediate instruction
TEST(CPU_6502, TAY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::TAY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x25);
    checkCyclesAndException();
}

// Test TAY immediate instruction, Y = 0
TEST(CPU_6502, TAY_Y_0) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::TAY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test TAY immediate instruction, negative flag
TEST(CPU_6502, TAY_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::TAY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0xFC);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}

// Test TSX immediate instruction
TEST(CPU_6502, TSX) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::TXS_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.S,0x25);
    cpu.mem[0x1003] = INS::LDX_IM;
    cpu.mem[0x1004] = 0x10;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x10);
    cpu.mem[0x1005] = INS::TSX_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x25);
    checkCyclesAndException();
}

// Test TSX immediate instruction, X = 0
TEST(CPU_6502, TSX_X_0) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x0;
    cpu.mem[0x1002] = INS::TXS_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.S,0x0);
    cpu.mem[0x1003] = INS::LDX_IM;
    cpu.mem[0x1004] = 0x10;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x10);
    cpu.mem[0x1005] = INS::TSX_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test TSX immediate instruction, negative flag
TEST(CPU_6502, TSX_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::TXS_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.S,0xFC);
    cpu.mem[0x1003] = INS::LDX_IM;
    cpu.mem[0x1004] = 0x10;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x10);
    cpu.mem[0x1005] = INS::TSX_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0xFC);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}

// Test TXA immediate instruction
TEST(CPU_6502, TXA) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::TXA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x25);
    checkCyclesAndException();
}

// Test TXA immediate instruction, A = 0
TEST(CPU_6502, TXA_A_0) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::TXA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,0);
    EXPECT_EQ(cpu.A,0x10);
    cpu.mem[0x1003] = INS::LDX_IM;
    cpu.mem[0x1004] = 0x0;
    cpu.mem[0x1005] = INS::TXA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,1);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.X,0x0);
    checkCyclesAndException();
}

// Test TXA immediate instruction, negative flag
TEST(CPU_6502, TXA_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::TXA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.A,0xFC);
    checkCyclesAndException();
}

// Test TXS immediate instruction
TEST(CPU_6502, TXS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::TXS_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.S,0x25);
    checkCyclesAndException();
}

// Test TYA immediate instruction
TEST(CPU_6502, TYA) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = INS::TYA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x25);
    checkCyclesAndException();
}

// Test TYA immediate instruction, A = 0
TEST(CPU_6502, TYA_A_0) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::TYA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,0);
    EXPECT_EQ(cpu.A,0x10);
    cpu.mem[0x1003] = INS::LDY_IM;
    cpu.mem[0x1004] = 0x0;
    cpu.mem[0x1005] = INS::TYA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,1);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.X,0x0);
    checkCyclesAndException();
}

// Test TYA immediate instruction, negative flag
TEST(CPU_6502, TYA_NEGATIVE_FLAG) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::TYA_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.A,0xFC);
    checkCyclesAndException();
}

