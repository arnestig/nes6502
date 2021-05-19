#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

// Test PHA implied instruction
TEST(CPU_6502, PHA_IMP) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFC;
    cpu.mem[0x1002] = INS::PHA_IMP;
    cpu.execute(5);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1],0xFC);
    checkCyclesAndException();
}

// Test PHP implied instruction
TEST(CPU_6502, PHP_IMP) {
    cpu.powerOn( 0x1000 );
    cpu.P.C = 1; // bit 0
    cpu.P.Z = 0; // bit 1
    cpu.P.I = 1; // bit 2
    cpu.P.D = 0; // bit 3
    cpu.P.B = 1; // bit 4
    cpu.P.U = 0; // bit 5
    cpu.P.V = 1; // bit 6
    cpu.P.N = 0; // bit 7
    // bits 4 and 5 will always be sed when pushed to stack
    cpu.mem[0x1000] = INS::PHP_IMP;
    cpu.execute(3);
    EXPECT_EQ(cpu.mem[0x100 + cpu.S + 1],0x75);
    cpu.dumpStack();
    checkCyclesAndException();
}

// Test PLA implied instruction
TEST(CPU_6502, PLA_IMP) {
    cpu.powerOn( 0x1000 );
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
    cpu.powerOn( 0x1000 );
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

