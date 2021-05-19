#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

// test LDA Immediate instruction
TEST(CPU_6502, LDA_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.execute(2);
    EXPECT_EQ(cpu.A,0x10);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Immediate instruction with Zero flag set
TEST(CPU_6502, LDA_IM_Zero_Flag_Set) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x00;
    cpu.execute(2);
    EXPECT_EQ(cpu.A,0x00);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Immediate instruction with Negative flag set
TEST(CPU_6502, LDA_IM_Negative_Flag_Set) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x81;
    cpu.execute(2);
    EXPECT_EQ(cpu.A,0x81);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// test LDA ZeroPage instruction
TEST(CPU_6502, LDA_ZP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ZP;
    cpu.mem[0x1001] = 0xCA;
    cpu.mem[0x00CA] = 0x23;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x23);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA ZeroPage,X instruction
TEST(CPU_6502, LDA_ZP_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ZP_X;
    cpu.mem[0x1001] = 0xC0;
    cpu.mem[0x00CF] = 0x23;
    cpu.X = 0x0F;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x23);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Absolute instruction
TEST(CPU_6502, LDA_ABS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ABS;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x2000] = 0x35;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Absolute,X instruction
TEST(CPU_6502, LDA_ABS_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ABS_X;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x20CA] = 0x35;
    cpu.X = 0xCA;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Absolute,X instruction, page crossing
TEST(CPU_6502, LDA_ABS_X_PAGE_CROSS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ABS_X;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x210A] = 0x35;
    cpu.X = 0xCA;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Absolute,Y instruction
TEST(CPU_6502, LDA_ABS_Y) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ABS_Y;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x20DE] = 0x35;
    cpu.Y = 0xDE;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Absolute,Y instruction, page crossing
TEST(CPU_6502, LDA_ABS_Y_PAGE_CROSS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_ABS_Y;
    cpu.mem[0x1001] = 0x30;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x210E] = 0x35;
    cpu.Y = 0xDE;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Indirect,X instruction
TEST(CPU_6502, LDA_IND_X) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IND_X;
    cpu.mem[0x1001] = 0xC0;
    cpu.mem[0x00DF] = 0x45;
    cpu.mem[0x00E0] = 0x21;
    cpu.mem[0x2145] = 0x1A;
    cpu.X = 0x1F;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x1A);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Indirect,X instruction, with ZeroPage wrap around
TEST(CPU_6502, LDA_IND_X_ZP_WRAP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IND_X;
    cpu.mem[0x1001] = 0xC0;
    cpu.mem[0x0078] = 0x45;
    cpu.mem[0x0079] = 0x21;
    cpu.mem[0x2145] = 0x1A;
    cpu.X = 0xB7;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x1A);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Indirect,Y instruction
TEST(CPU_6502, LDA_IND_Y) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IND_Y;
    cpu.mem[0x1001] = 0xDC;
    cpu.mem[0x00DC] = 0xB3;
    cpu.mem[0x00DD] = 0x19;
    cpu.mem[0x19B6] = 0x27;
    cpu.Y = 0x03;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x27);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDA Indirect,Y instruction, page crossing
// Disabled for now, need to fix border crossing
// TEST(CPU_6502, LDA_IND_Y_PAGE_CROSS) {
//     cpu.powerOn( 0x1000 );
//     cpu.mem[0x1000] = INS::LDA_IND_Y;
//     cpu.mem[0x1001] = 0xDC;
//     cpu.mem[0x00DC] = 0xB3;
//     cpu.mem[0x00DD] = 0x19;
//     cpu.mem[0x19B6] = 0x27;
//     cpu.Y = 0x03;
//     cpu.execute(6);
//     EXPECT_EQ(cpu.A,0x27);
//     EXPECT_EQ(cpu.P.Z,0x0);
//     EXPECT_EQ(cpu.P.N,0x0);
//     EXPECT_EQ(cpu.cycles, 0);
// }

// test LDX Immediate instruction
TEST(CPU_6502, LDX_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x10);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDX Immediate instruction with Zero flag set
TEST(CPU_6502, LDX_IM_Zero_Flag_Set) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x00;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x00);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDX Immediate instruction with Negative flag set
TEST(CPU_6502, LDX_IM_Negative_Flag_Set) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x81;
    cpu.execute(2);
    EXPECT_EQ(cpu.X,0x81);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// test LDX ZeroPage instruction
TEST(CPU_6502, LDX_ZP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_ZP;
    cpu.mem[0x1001] = 0xCA;
    cpu.mem[0x00CA] = 0x23;
    cpu.execute(3);
    EXPECT_EQ(cpu.X,0x23);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDX ZeroPage,Y instruction
TEST(CPU_6502, LDX_ZP_Y) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_ZP_Y;
    cpu.mem[0x1001] = 0xC0;
    cpu.mem[0x00CF] = 0x23;
    cpu.Y = 0x0F;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x23);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDX Absolute instruction
TEST(CPU_6502, LDX_ABS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_ABS;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x2000] = 0x35;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDX Absolute,Y instruction
TEST(CPU_6502, LDX_ABS_Y) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_ABS_Y;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x20CA] = 0x35;
    cpu.Y = 0xCA;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDX Absolute,Y instruction, page crossing
TEST(CPU_6502, LDX_ABS_Y_PAGE_CROSS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDX_ABS_Y;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x210A] = 0x35;
    cpu.Y = 0xCA;
    cpu.execute(5);
    EXPECT_EQ(cpu.X,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY Immediate instruction
TEST(CPU_6502, LDY_IM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x10;
    cpu.execute(2);
    EXPECT_EQ(cpu.Y,0x10);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY Immediate instruction with Zero flag set
TEST(CPU_6502, LDY_IM_Zero_Flag_Set) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x00;
    cpu.execute(2);
    EXPECT_EQ(cpu.Y,0x00);
    EXPECT_EQ(cpu.P.Z,0x1);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY Immediate instruction with Negative flag set
TEST(CPU_6502, LDY_IM_Negative_Flag_Set) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x81;
    cpu.execute(2);
    EXPECT_EQ(cpu.Y,0x81);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// test LDY ZeroPage instruction
TEST(CPU_6502, LDY_ZP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_ZP;
    cpu.mem[0x1001] = 0xCA;
    cpu.mem[0x00CA] = 0x23;
    cpu.execute(3);
    EXPECT_EQ(cpu.Y,0x23);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY ZeroPage,X instruction
TEST(CPU_6502, LDY_ZP_Y) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_ZP_X;
    cpu.mem[0x1001] = 0xC0;
    cpu.mem[0x00CF] = 0x23;
    cpu.X = 0x0F;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x23);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY Absolute instruction
TEST(CPU_6502, LDY_ABS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_ABS;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x2000] = 0x35;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY Absolute,X instruction
TEST(CPU_6502, LDY_ABS_Y) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_ABS_X;
    cpu.mem[0x1001] = 0x00;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x20CA] = 0x35;
    cpu.X = 0xCA;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

// test LDY Absolute,X instruction, page crossing
TEST(CPU_6502, LDY_ABS_Y_PAGE_CROSS) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDY_ABS_X;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = 0x20;
    cpu.mem[0x210A] = 0x35;
    cpu.X = 0xCA;
    cpu.execute(5);
    EXPECT_EQ(cpu.Y,0x35);
    EXPECT_EQ(cpu.P.Z,0x0);
    EXPECT_EQ(cpu.P.N,0x0);
    checkCyclesAndException();
}

