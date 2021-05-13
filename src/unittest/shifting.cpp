#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

// Test ASL accumulator instruction
TEST(CPU_6502,ASL_ACC) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x5;
    cpu.mem[0x1002] = INS::ASL_ACC;
    cpu.execute(4);
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
    cpu.execute(4);
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
    cpu.execute(4);
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
    cpu.execute(8);
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
    cpu.execute(8);
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
    cpu.execute(8);
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
    cpu.execute(4);
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
    cpu.execute(4);
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
    cpu.execute(8);
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
    cpu.execute(8);
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
    cpu.execute(4);
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
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0xB);
    checkCyclesAndException();
}

// Test ROL accumulator instruction with negative flag set
TEST(CPU_6502,ROL_ACC_NEGATIVE) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x40;
    cpu.mem[0x1002] = INS::ROL_ACC;
    cpu.execute(4);
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
    cpu.execute(8);
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
    cpu.execute(8);
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
    cpu.execute(8);
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
    cpu.execute(4);
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
    cpu.execute(4);
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
    cpu.execute(8);
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
    cpu.execute(8);
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

