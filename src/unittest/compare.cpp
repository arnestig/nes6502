#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();
// Test CMP immediate instruction, carry set
TEST(CPU_6502, CMP_IM_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CMP_IM;
    cpu.mem[0x1003] = 0x8;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP immediate instruction, zero set
TEST(CPU_6502, CMP_IM_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CMP_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP ZeroPage instruction, carry set
TEST(CPU_6502, CMP_ZP_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CMP_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0x8;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP ZeroPage instruction, zero set
TEST(CPU_6502, CMP_ZP_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CMP_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0x10;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP ZeroPage X instruction, carry set
TEST(CPU_6502, CMP_ZP_X_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ZP_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0025] = 0x8;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP ZeroPage X instruction, zero set
TEST(CPU_6502, CMP_ZP_X_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ZP_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0025] = 0x10;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Absolute instruction, carry set
TEST(CPU_6502, CMP_ABS_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1920] = 0x8;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Absolute instruction, zero set
TEST(CPU_6502, CMP_ABS_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1920] = 0x10;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Absolute X instruction, carry set
TEST(CPU_6502, CMP_ABS_X_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1925] = 0x8;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Absolute X instruction, carry set, page cross
TEST(CPU_6502, CMP_ABS_X_CARRY_PAGE_CROSS ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x2019] = 0x8;
    cpu.execute(9);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}


// Test CMP AbsoluteX  instruction, zero set
TEST(CPU_6502, CMP_ABS_X_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1925] = 0x10;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Absolute Y instruction, carry set
TEST(CPU_6502, CMP_ABS_Y_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1925] = 0x8;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Absolute Y instruction, carry set, page cross
TEST(CPU_6502, CMP_ABS_Y_CARRY_PAGE_CROSS ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x2019] = 0x8;
    cpu.execute(9);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}


// Test CMP Absolute Y instruction, zero set
TEST(CPU_6502, CMP_ABS_Y_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_ABS_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1925] = 0x10;
    cpu.execute(8);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Indirect X instruction, carry set
TEST(CPU_6502, CMP_IND_X_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_IND_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0025] = 0x25;
    cpu.mem[0x0026] = 0x19;
    cpu.mem[0x1925] = 0x8;
    cpu.execute(10);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Indirect X instruction, zero set
TEST(CPU_6502, CMP_IND_X_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_IND_X;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0025] = 0x25;
    cpu.mem[0x0026] = 0x19;
    cpu.mem[0x1925] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Indirect Y instruction, carry set
TEST(CPU_6502, CMP_IND_Y_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_IND_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0020] = 0x25;
    cpu.mem[0x0021] = 0x19;
    cpu.mem[0x192A] = 0x8;
    cpu.execute(9);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Indirect Y instruction, zero set
TEST(CPU_6502, CMP_IND_Y_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_IND_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0020] = 0x25;
    cpu.mem[0x0021] = 0x19;
    cpu.mem[0x192A] = 0x10;
    cpu.execute(9);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CMP Indirect Y instruction, zero set, page cross
TEST(CPU_6502, CMP_IND_Y_ZERO_PAGE_CROSS ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.mem[0x1004] = INS::CMP_IND_Y;
    cpu.mem[0x1005] = 0x20;
    cpu.mem[0x0020] = 0x25;
    cpu.mem[0x0021] = 0x19;
    cpu.mem[0x1A24] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPX immediate instruction, carry set
TEST(CPU_6502, CPX_IM_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPX_IM;
    cpu.mem[0x1003] = 0x8;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPX immediate instruction, zero set
TEST(CPU_6502, CPX_IM_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPX_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPX ZeroPage instruction, carry set
TEST(CPU_6502, CPX_ZP_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPX_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0x8;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPX ZeroPage instruction, zero set
TEST(CPU_6502, CPX_ZP_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPX_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0x10;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPX Absolute instruction, carry set
TEST(CPU_6502, CPX_ABS_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPX_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.mem[0x1920] = 0x8;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPX Absolute instruction, zero set
TEST(CPU_6502, CPX_ABS_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPX_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.mem[0x1920] = 0x10;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPY immediate instruction, carry set
TEST(CPU_6502, CPY_IM_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPY_IM;
    cpu.mem[0x1003] = 0x8;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPY immediate instruction, zero set
TEST(CPU_6502, CPY_IM_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPY_IM;
    cpu.mem[0x1003] = 0x10;
    cpu.execute(4);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPY ZeroPage instruction, carry set
TEST(CPU_6502, CPY_ZP_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPY_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0x8;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPY ZeroPage instruction, zero set
TEST(CPU_6502, CPY_ZP_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPY_ZP;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0020] = 0x10;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPY Absolute instruction, carry set
TEST(CPU_6502, CPY_ABS_CARRY ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPY_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.mem[0x1920] = 0x8;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test CPY Absolute instruction, zero set
TEST(CPU_6502, CPY_ABS_ZERO ) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::CPY_ABS;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x19;
    cpu.mem[0x1920] = 0x10;
    cpu.execute(6);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

