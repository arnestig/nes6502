#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

// Test DEC ZeroPage instruction
TEST(CPU_6502, DEC_ZP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0x23;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x00BF],0x22);
    checkCyclesAndException();
}
// Test DEC ZeroPage instruction, zero flag
TEST(CPU_6502, DEC_ZP_0) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0x01;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.Z,1);
    EXPECT_EQ(cpu.mem[0x00BF],0x0);
    checkCyclesAndException();
}
// Test DEC ZeroPage instruction, negative flag
TEST(CPU_6502, DEC_ZP_NEGATIVE_FLAG) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0x00;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.mem[0x00BF],0xFF);
    checkCyclesAndException();
}
// Test DEC ZeroPage,X instruction
TEST(CPU_6502, DEC_ZP_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::DEC_ZP_X;
    cpu.mem[0x1003] = 0xBF;
    cpu.mem[0x00CE] = 0x23;
    cpu.execute(8);
    EXPECT_EQ(cpu.mem[0x00CE],0x22);
    checkCyclesAndException();
}
// Test DEC Absolute instruction
TEST(CPU_6502, DEC_ABS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEC_ABS;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x2040] = 0x35;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2040],0x34);
    checkCyclesAndException();
}
// Test DEC Absolute,X instruction
TEST(CPU_6502, DEC_ABS_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x0F;
    cpu.mem[0x1002] = INS::DEC_ABS_X;
    cpu.mem[0x1003] = 0x40;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x204F] = 0x35;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x204F],0x34);
    checkCyclesAndException();
}
// Test DEX immediate instruction
TEST(CPU_6502, DEX_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEX_IM;
    cpu.mem[0x1001] = INS::DEX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0xFE);
    checkCyclesAndException();
}
// Test DEX immediate instruction, X = 0
TEST(CPU_6502, DEX_IM_X_0) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x1;
    cpu.mem[0x1002] = INS::DEX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}
// Test DEX immediate instruction, negative flag
TEST(CPU_6502, DEX_IM_NEGATIVE_FLAG) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEX_IM;
    cpu.mem[0x1001] = INS::DEX_IM;
    cpu.mem[0x1002] = INS::DEX_IM;
    cpu.execute(6);
    EXPECT_EQ(cpu.X,0xFD);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}
// Test DEY immediate instruction
TEST(CPU_6502, DEY_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEY_IM;
    cpu.mem[0x1001] = INS::DEY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0xFE);
    checkCyclesAndException();
}
// Test DEY immediate instruction, Y = 0
TEST(CPU_6502, DEY_IM_Y_0) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x1;
    cpu.mem[0x1002] = INS::DEY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}
// Test DEY immediate instruction, negative flag
TEST(CPU_6502, DEY_IM_NEGATIVE_FLAG) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::DEY_IM;
    cpu.mem[0x1001] = INS::DEY_IM;
    cpu.mem[0x1002] = INS::DEY_IM;
    cpu.execute(6);
    EXPECT_EQ(cpu.Y,0xFD);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}

// Test INC ZeroPage instruction
TEST(CPU_6502, INC_ZP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::INC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0x23;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x00BF],0x24);
    checkCyclesAndException();
}
// Test INC ZeroPage instruction, zero flag
TEST(CPU_6502, INC_ZP_0) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::INC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0xFF;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.Z,1);
    EXPECT_EQ(cpu.mem[0x00BF],0x0);
    checkCyclesAndException();
}
// Test INC ZeroPage instruction, negative flag
TEST(CPU_6502, INC_ZP_NEGATIVE_FLAG) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::INC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0xF0;
    cpu.execute(5);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.mem[0x00BF],0xF1);
    checkCyclesAndException();
}
// Test INC ZeroPage,X instruction
TEST(CPU_6502, INC_ZP_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::INC_ZP_X;
    cpu.mem[0x1003] = 0xBF;
    cpu.mem[0x00CE] = 0x23;
    cpu.execute(8);
    EXPECT_EQ(cpu.mem[0x00CE],0x24);
    checkCyclesAndException();
}
// Test INC Absolute instruction
TEST(CPU_6502, INC_ABS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::INC_ABS;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x2040] = 0x35;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2040],0x36);
    checkCyclesAndException();
}
// Test INC Absolute,X instruction
TEST(CPU_6502, INC_ABS_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x0F;
    cpu.mem[0x1002] = INS::INC_ABS_X;
    cpu.mem[0x1003] = 0x40;
    cpu.mem[0x1004] = 0x20;
    cpu.mem[0x204F] = 0x35;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x204F],0x36);
    checkCyclesAndException();
}
// Test INX immediate instruction
TEST(CPU_6502, INX_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::INX_IM;
    cpu.mem[0x1001] = INS::INX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x02);
    checkCyclesAndException();
}
// Test INX immediate instruction, X = 0
TEST(CPU_6502, INX_IM_X_0) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::INX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}
// Test INX immediate instruction, negative flag
TEST(CPU_6502, INX_IM_NEGATIVE_FLAG) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x7f;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x7f);
    EXPECT_EQ(cpu.P.N,0);
    cpu.mem[0x1002] = INS::INX_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x80);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}
// Test INY immediate instruction
TEST(CPU_6502, INY_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::INY_IM;
    cpu.mem[0x1001] = INS::INY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x02);
    checkCyclesAndException();
}
// Test INY immediate instruction, Y = 0
TEST(CPU_6502, INY_IM_Y_0) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::INY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}
// Test INY immediate instruction, negative flag
TEST(CPU_6502, INY_IM_NEGATIVE_FLAG) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x7f;
    cpu.execute(2);
    EXPECT_EQ(cpu.Y,0x7f);
    EXPECT_EQ(cpu.P.N,0);
    cpu.mem[0x1002] = INS::INY_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.Y,0x80);
    EXPECT_EQ(cpu.P.N,1);
    checkCyclesAndException();
}

// Test JMP absolute instruction
TEST(CPU_6502, JMP_ABS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::JMP_ABS;
    cpu.mem[0x1001] = 0x08;
    cpu.mem[0x1002] = 0x10;
    cpu.mem[0x1008] = INS::INX_IM;
    cpu.execute(5);
    EXPECT_EQ(cpu.X,0x1);
    checkCyclesAndException();
}

// Test INY indirect instruction
TEST(CPU_6502, JMP_IND) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::JMP_IND;
    cpu.mem[0x1001] = 0x08;
    cpu.mem[0x1002] = 0x10;
    cpu.mem[0x1008] = 0x10;
    cpu.mem[0x1009] = 0x12;
    cpu.mem[0x1210] = INS::INY_IM;
    cpu.execute(7);
    EXPECT_EQ(cpu.Y,0x1);
    checkCyclesAndException();
}

