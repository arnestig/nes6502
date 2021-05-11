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

    cpu.execute(18);

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

// Test RTS implied instruction
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

// Test RTI implied instruction
TEST(CPU_6502,RTI_IMP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM; // push high byte PC to stack
    cpu.mem[0x1001] = 0x10;
    cpu.mem[0x1002] = INS::PHA_IMP;
    cpu.mem[0x1003] = INS::LDA_IM; // push low byte PC to stack
    cpu.mem[0x1004] = 0x40;
    cpu.mem[0x1005] = INS::PHA_IMP;
    cpu.mem[0x1006] = INS::LDA_IM; // push processor status to stack
    cpu.mem[0x1007] = 0xF;
    cpu.mem[0x1008] = INS::PHA_IMP;
    cpu.mem[0x1009] = INS::RTI_IMP;
    cpu.execute(21);
    EXPECT_EQ(cpu.P.C, 1); // bit 1
    EXPECT_EQ(cpu.P.Z, 1); // bit 2
    EXPECT_EQ(cpu.P.I, 1); // bit 3
    EXPECT_EQ(cpu.P.D, 1); // bit 4
    EXPECT_EQ(cpu.P.B, 0); // bit 5
    EXPECT_EQ(cpu.P.V, 0); // bit 6
    EXPECT_EQ(cpu.P.N, 0); // bit 7
    EXPECT_EQ(cpu.PC, 0x1040);
    checkCyclesAndException();
}


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

// Test BCC relative instruction
TEST(CPU_6502,BCC_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BCC_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.C = 1;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BCC relative instruction with carry clear
TEST(CPU_6502,BCC_REL_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BCC_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.C = 0;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BCC relative instruction with carry clear
TEST(CPU_6502,BCC_REL_CARRY_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BCC_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.C = 0;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BCS relative instruction with carry clear
TEST(CPU_6502,BCS_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BCS_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.C = 0;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BCS relative instruction with carry set
TEST(CPU_6502,BCS_REL_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BCS_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.C = 1;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BCS relative instruction with carry set
TEST(CPU_6502,BCS_REL_CARRY_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BCS_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.C = 1;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BEQ relative instruction with zero clear
TEST(CPU_6502,BEQ_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BEQ_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.Z = 0;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BEQ relative instruction with zero set
TEST(CPU_6502,BEQ_REL_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BEQ_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.Z = 1;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BEQ relative instruction with zero set
TEST(CPU_6502,BEQ_REL_ZERO_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BEQ_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.Z = 1;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BNE relative instruction with zero set
TEST(CPU_6502,BNE_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BNE_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.Z = 1;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BNE relative instruction with zero clear
TEST(CPU_6502,BNE_REL_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BNE_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.Z = 0;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BNE relative instruction with zero clear
TEST(CPU_6502,BNE_REL_ZERO_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BNE_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.Z = 0;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BMI relative instruction with negative clear
TEST(CPU_6502,BMI_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BMI_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.N = 0;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BMI relative instruction with negative set
TEST(CPU_6502,BMI_REL_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BMI_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.N = 1;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BMI relative instruction with negative set
TEST(CPU_6502,BMI_REL_NEGATIVE_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BMI_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.N = 1;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BPL relative instruction with negative set
TEST(CPU_6502,BPL_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BPL_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.N = 1;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BPL relative instruction with negative clear
TEST(CPU_6502,BPL_REL_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BPL_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.N = 0;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BPL relative instruction with negative clear
TEST(CPU_6502,BPL_REL_NEGATIVE_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BPL_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.V = 0;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BVS relative instruction with overflow clear
TEST(CPU_6502,BVS_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BVS_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.V = 0;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BVS relative instruction with overflow set
TEST(CPU_6502,BVS_REL_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BVS_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.V = 1;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BVS relative instruction with overflow set
TEST(CPU_6502,BVS_REL_OVERFLOW_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BVS_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.V = 1;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BVC relative instruction with overflow set
TEST(CPU_6502,BVC_REL) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BVC_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.V = 1;
    cpu.execute(2);
    EXPECT_EQ(cpu.PC,0x1002);
    checkCyclesAndException();
}

// Test BVC relative instruction with overflow clear
TEST(CPU_6502,BVC_REL_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BVC_REL;
    cpu.mem[0x1001] = 0x20;
    cpu.P.V = 0;
    cpu.execute(3);
    EXPECT_EQ(cpu.PC,0x1022);
    checkCyclesAndException();
}

// Test BVC relative instruction with overflow clear
TEST(CPU_6502,BVC_REL_OVERFLOW_PAGE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BVC_REL;
    cpu.mem[0x1001] = 0xFF;
    cpu.P.N = 0;
    cpu.execute(4);
    EXPECT_EQ(cpu.PC,0x1101);
    checkCyclesAndException();
}

// Test BRK implied instruction
TEST(CPU_6502,BRK_IMP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::BRK_IMP;
    cpu.P.C = 1; // bit 1
    cpu.P.Z = 0; // bit 2
    cpu.P.I = 1; // bit 3
    cpu.P.D = 0; // bit 4
    cpu.P.B = 1; // bit 5
    cpu.P.V = 0; // bit 6
    cpu.P.N = 1; // bit 7
    cpu.mem[0xFFFE] = 0x34;
    cpu.mem[0xFFFF] = 0x12;
    cpu.execute(7);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1],0x55);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 2],0x01);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 3],0x10);
    EXPECT_EQ(cpu.P.B,0x1);
    EXPECT_EQ(cpu.PC,0x1234);
    checkCyclesAndException();
}

// Test ASL accumulator instruction
TEST(CPU_6502,ASL_ACC) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ASL_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0xA);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL accumulator instruction with carry flag set
TEST(CPU_6502,ASL_ACC_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x92;
    cpu.mem[0x1002] = INS::ASL_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x24);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test ASL accumulator instruction with negative flag set
TEST(CPU_6502,ASL_ACC_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x50;
    cpu.mem[0x1002] = INS::ASL_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL zeropage instruction
TEST(CPU_6502,ASL_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ASL_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0xA;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x14);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL zeropage instruction with carry flag set
TEST(CPU_6502,ASL_ZP_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ASL_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x92;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x24);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test ASL zeropage instruction with negative flag set
TEST(CPU_6502,ASL_ZP_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ASL_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x50;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL zeropage X instruction
TEST(CPU_6502,ASL_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ASL_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0xA;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x14);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL zeropage X instruction with carry flag set
TEST(CPU_6502,ASL_ZP_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ASL_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0x92;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x24);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test ASL zeropage X instruction with negative flag set
TEST(CPU_6502,ASL_ZP_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ASL_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0x50;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}


// Test ASL absolute instruction
TEST(CPU_6502,ASL_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ASL_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0xA;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x14);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL absolute instruction with carry flag set
TEST(CPU_6502,ASL_ABS_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ASL_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0x92;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x24);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test ASL absolute instruction with negative flag set
TEST(CPU_6502,ASL_ABS_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ASL_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0x50;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL absolute X instruction
TEST(CPU_6502,ASL_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ASL_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0xA;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x14);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ASL absolute X instruction with carry flag set
TEST(CPU_6502,ASL_ABS_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ASL_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0x92;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x24);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test ASL absolute X instruction with negative flag set
TEST(CPU_6502,ASL_ABS_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ASL_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0x50;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}


// Test LSR accumulator instruction
TEST(CPU_6502,LSR_ACC) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LSR_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test LSR accumulator instruction with carry flag set
TEST(CPU_6502,LSR_ACC_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x9;
    cpu.mem[0x1002] = INS::LSR_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x4);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test LSR zeropage instruction
TEST(CPU_6502,LSR_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LSR_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x4;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test LSR zeropage instruction with carry flag set
TEST(CPU_6502,LSR_ZP_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LSR_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x9;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x4);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test LSR zeropage X instruction
TEST(CPU_6502,LSR_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LSR_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0xC;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x6);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test LSR zeropage X instruction with carry flag set
TEST(CPU_6502,LSR_ZP_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::LSR_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0xD;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x6);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test LSR absolute instruction
TEST(CPU_6502,LSR_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LSR_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0x4;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test LSR absolute instruction with carry flag set
TEST(CPU_6502,LSR_ABS_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LSR_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0xD;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x6);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test LSR absolute X instruction
TEST(CPU_6502,LSR_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::LSR_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test LSR absolute X instruction with carry flag set
TEST(CPU_6502,LSR_ABS_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::LSR_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0xF;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x7);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test ROL accumulator instruction
TEST(CPU_6502,ROL_ACC) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROL_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0xA);
    checkCyclesAndException();
}

// Test ROL accumulator instruction with carry flag pre-set
TEST(CPU_6502,ROL_ACC_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROL_ACC;
    cpu.P.C = 0x1;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0xB);
    checkCyclesAndException();
}

// Test ROL accumulator instruction with negative flag set
TEST(CPU_6502,ROL_ACC_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = INS::ROL_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x80);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROL zeropage instruction
TEST(CPU_6502,ROL_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROL_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0xA;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x14);
    checkCyclesAndException();
}

// Test ROL zeropage instruction with carry flag pre-set
TEST(CPU_6502,ROL_ZP_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROL_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x20;
    cpu.P.C = 0x1;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x41);
    checkCyclesAndException();
}

// Test ROL zeropage instruction with negative flag set
TEST(CPU_6502,ROL_ZP_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROL_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x50;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROL zeropage X instruction
TEST(CPU_6502,ROL_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROL_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0xA;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x14);
    checkCyclesAndException();
}

// Test ROL zeropage X instruction with carry flag pre-set
TEST(CPU_6502,ROL_ZP_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROL_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0x20;
    cpu.P.C = 1;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x41);
    checkCyclesAndException();
}

// Test ROL zeropage X instruction with negative flag set
TEST(CPU_6502,ROL_ZP_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROL_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0x50;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}


// Test ROL absolute instruction
TEST(CPU_6502,ROL_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROL_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0xA;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x14);
    checkCyclesAndException();
}

// Test ROL absolute instruction with carry flag pre-set
TEST(CPU_6502,ROL_ABS_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROL_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0x40;
    cpu.P.C = 1;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x81);
    checkCyclesAndException();
}

// Test ROL absolute instruction with negative flag set
TEST(CPU_6502,ROL_ABS_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROL_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0x50;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROL absolute X instruction
TEST(CPU_6502,ROL_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ROL_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0xA;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x14);
    checkCyclesAndException();
}

// Test ROL absolute X instruction with carry flag pre-set
TEST(CPU_6502,ROL_ABS_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ROL_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0x15;
    cpu.P.C = 1;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x2B);
    checkCyclesAndException();
}

// Test ROL absolute X instruction with negative flag set
TEST(CPU_6502,ROL_ABS_X_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ROL_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0x50;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0xA0);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROR accumulator instruction
TEST(CPU_6502,ROR_ACC) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ROR_ACC;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x7);
    checkCyclesAndException();
}

// Test ROR accumulator instruction with carry flag pre-set
TEST(CPU_6502,ROR_ACC_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xA;
    cpu.mem[0x1002] = INS::ROR_ACC;
    cpu.P.C = 0x1;
    cpu.execute(3);
    EXPECT_EQ(cpu.A,0x85);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROR zeropage instruction
TEST(CPU_6502,ROR_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROR_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x4;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ROR zeropage instruction with carry flag pre-set
TEST(CPU_6502,ROR_ZP_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROR_ZP;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x0020] = 0x9;
    cpu.P.C = 0x1;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x20],0x84);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROR zeropage X instruction
TEST(CPU_6502,ROR_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROR_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0xC;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x6);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ROR zeropage X instruction with carry flag pre-set
TEST(CPU_6502,ROR_ZP_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ROR_ZP_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x0025] = 0xD;
    cpu.P.C = 0x1;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x25],0x86);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROR absolute instruction
TEST(CPU_6502,ROR_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROR_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0x4;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ROR absolute instruction with carry flag pre-set
TEST(CPU_6502,ROR_ABS_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::ROR_ABS;
    cpu.mem[0x1001] = 0x20;
    cpu.mem[0x1002] = 0x25;
    cpu.mem[0x2520] = 0xD;
    cpu.P.C = 0x1;
    cpu.execute(6);
    EXPECT_EQ(cpu.mem[0x2520],0x86);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

// Test ROR absolute X instruction
TEST(CPU_6502,ROR_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ROR_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x2);
    EXPECT_EQ(cpu.P.C,0x0);
    checkCyclesAndException();
}

// Test ROR absolute X instruction with carry flag pre-set
TEST(CPU_6502,ROR_ABS_X_CARRY) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF;
    cpu.mem[0x1002] = INS::ROR_ABS_X;
    cpu.mem[0x1003] = 0x20;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x252F] = 0xF;
    cpu.P.C = 0x1;
    cpu.execute(9);
    EXPECT_EQ(cpu.mem[0x252F],0x87);
    EXPECT_EQ(cpu.P.N,0x1);
    checkCyclesAndException();
}

int main( int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

