#include "6502.h"
#include "gtest/gtest.h"

struct CPU cpu;

void checkCyclesAndException()
{
    EXPECT_EQ(cpu.cycles, 0);
    EXPECT_FALSE(cpu.exception);
}

// test LDA Immediate instruction
TEST(CPU_6502, LDA_IM) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
//     cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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


TEST(CPU_6502, RAW_PROGRAM_1) {
    // Address  Hexdump   Dissassembly
    // -------------------------------
    // $0600    a9 01     LDA #$01
    // $0602    8d 00 02  STA $0200
    // $0605    a9 05     LDA #$05
    // $0607    8d 01 02  STA $0201
    // $060a    a9 08     LDA #$08
    // $060c    8d 02 02  STA $0202
    cpu.reset();
    cpu.mem[0x1000] = 0xa9;
    cpu.mem[0x1001] = 0x01;
    cpu.mem[0x1002] = 0x8d;
    cpu.mem[0x1003] = 0x00;
    cpu.mem[0x1004] = 0x02;
    cpu.mem[0x1005] = 0xa9;
    cpu.mem[0x1006] = 0x05;
    cpu.mem[0x1007] = 0x8d;
    cpu.mem[0x1008] = 0x01;
    cpu.mem[0x1009] = 0x02;
    cpu.mem[0x100A] = 0xa9;
    cpu.mem[0x100B] = 0x08;
    cpu.mem[0x100C] = 0x8d;
    cpu.mem[0x100D] = 0x02;
    cpu.mem[0x100E] = 0x02;

    cpu.execute(24);

    EXPECT_EQ(cpu.mem[0x0200], 0x01);
    EXPECT_EQ(cpu.mem[0x0201], 0x05);
    EXPECT_EQ(cpu.mem[0x0202], 0x08);
    checkCyclesAndException();
}

// Test carry flag clear/set
TEST(CPU_6502, CLC_SEC) {
    cpu.reset();
    cpu.P.C = 1;
    cpu.mem[0x1000] = INS::CLC_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.C, 0);
    cpu.mem[0x1001] = INS::SEC_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.C, 1);
    checkCyclesAndException();
}

// Test decimal flag clear/set
TEST(CPU_6502, CLD_SED) {
    cpu.reset();
    cpu.P.D = 1;
    cpu.mem[0x1000] = INS::CLD_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.D, 0);
    cpu.mem[0x1001] = INS::SED_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.D, 1);
    checkCyclesAndException();
}

// Test interrupt flag clear/set
TEST(CPU_6502, CLI_SEI) {
    cpu.reset();
    cpu.P.I = 1;
    cpu.mem[0x1000] = INS::CLI_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.I, 0);
    cpu.mem[0x1001] = INS::SEI_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.I, 1);
    checkCyclesAndException();
}

// Test overflow flag clear
TEST(CPU_6502, CLV) {
    cpu.reset();
    cpu.P.V = 1;
    cpu.mem[0x1000] = INS::CLV_IM;
    cpu.execute(2);
    EXPECT_EQ(cpu.P.I, 0);
    checkCyclesAndException();
}


// Test NOP instruction
TEST(CPU_6502, NOP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::NOP_IM;
    cpu.execute(2);
    checkCyclesAndException();
}

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

// Test DEC ZeroPage instruction
TEST(CPU_6502, DEC_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::DEC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0x23;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x00BF],0x22);
    checkCyclesAndException();
}
// Test DEC ZeroPage instruction, zero flag
TEST(CPU_6502, DEC_ZP_0) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
    cpu.mem[0x1000] = INS::DEX_IM;
    cpu.mem[0x1001] = INS::DEX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0xFE);
    checkCyclesAndException();
}
// Test DEX immediate instruction, X = 0
TEST(CPU_6502, DEX_IM_X_0) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
    cpu.mem[0x1000] = INS::DEY_IM;
    cpu.mem[0x1001] = INS::DEY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0xFE);
    checkCyclesAndException();
}
// Test DEY immediate instruction, Y = 0
TEST(CPU_6502, DEY_IM_Y_0) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
    cpu.mem[0x1000] = INS::INC_ZP;
    cpu.mem[0x1001] = 0xBF;
    cpu.mem[0x00BF] = 0x23;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x00BF],0x24);
    checkCyclesAndException();
}
// Test INC ZeroPage instruction, zero flag
TEST(CPU_6502, INC_ZP_0) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
    cpu.mem[0x1000] = INS::INX_IM;
    cpu.mem[0x1001] = INS::INX_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.X,0x02);
    checkCyclesAndException();
}
// Test INX immediate instruction, X = 0
TEST(CPU_6502, INX_IM_X_0) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
    cpu.mem[0x1000] = INS::INY_IM;
    cpu.mem[0x1001] = INS::INY_IM;
    cpu.execute(4);
    EXPECT_EQ(cpu.Y,0x02);
    checkCyclesAndException();
}
// Test INY immediate instruction, Y = 0
TEST(CPU_6502, INY_IM_Y_0) {
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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
    cpu.reset();
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

// Test PHA implied instruction
TEST(CPU_6502, PHA_IMP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::PHA_IMP;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1],0xFC);
    checkCyclesAndException();
}

// Test PHP implied instruction
TEST(CPU_6502, PHP_IMP) {
    cpu.reset();
    cpu.P.C = 1; // bit 1
    cpu.P.Z = 0; // bit 2
    cpu.P.I = 1; // bit 3
    cpu.P.D = 0; // bit 4
    cpu.P.B = 1; // bit 5
    cpu.P.V = 0; // bit 6
    cpu.P.N = 1; // bit 7
    cpu.mem[0x1000] = INS::PHP_IMP;
    cpu.execute(3);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1],0x55);
    checkCyclesAndException();
}

// Test PLA implied instruction
TEST(CPU_6502, PLA_IMP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x13;
    cpu.execute(2);
    EXPECT_EQ(cpu.A,0x13);
    cpu.mem[0x1002] = INS::PHA_IMP;
    cpu.execute(3);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1],0x13);
    cpu.mem[0x1003] = INS::LDA_IM;
    cpu.mem[0x1004] = 0x19;
    cpu.execute(2);
    EXPECT_EQ(cpu.A,0x19);
    cpu.mem[0x1005] = INS::PLA_IMP;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x13);
    checkCyclesAndException();
}

// Test PLP implied instruction
TEST(CPU_6502, PLP_IMP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xf;
    cpu.mem[0x1002] = INS::PHA_IMP;
    cpu.mem[0x1003] = INS::PLP_IMP;
    cpu.execute(9);
    EXPECT_EQ(cpu.P.C, 1); // bit 1
    EXPECT_EQ(cpu.P.Z, 1); // bit 2
    EXPECT_EQ(cpu.P.I, 1); // bit 3
    EXPECT_EQ(cpu.P.D, 1); // bit 4
    EXPECT_EQ(cpu.P.B, 0); // bit 5
    EXPECT_EQ(cpu.P.V, 0); // bit 6
    EXPECT_EQ(cpu.P.N, 0); // bit 7
    checkCyclesAndException();
}

// Test JSR absolute instruction
TEST(CPU_6502,JSR_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::JSR_ABS;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = 0x10;
    cpu.execute(6);
    EXPECT_EQ(cpu.PC,0x1025);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1], 0x02);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 2], 0x10);
    EXPECT_EQ(cpu.S, 0xfd);
    checkCyclesAndException();
}

// Test JSR absolute instruction
TEST(CPU_6502,RTS_IMP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::JSR_ABS;
    cpu.mem[0x1001] = 0x25;
    cpu.mem[0x1002] = 0x10;
    cpu.mem[0x1025] = INS::RTS_IMP;
    cpu.execute(12);
    EXPECT_EQ(cpu.PC, 0x1003);
    checkCyclesAndException();
}

int main( int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

