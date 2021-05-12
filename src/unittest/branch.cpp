#include "../6502.h"
#include "gtest/gtest.h"

extern struct CPU cpu;

extern void checkCyclesAndException();

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
