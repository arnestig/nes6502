#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

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
    EXPECT_EQ(cpu.P.V, 0);
    checkCyclesAndException();
}


