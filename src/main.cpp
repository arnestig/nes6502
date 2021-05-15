#include "6502.h"
#include "gtest/gtest.h"
#include <fstream>
#include <string.h>

struct CPU cpu;

void loadNESFile()
{
    std::ifstream ifs("registers.nes", std::ios_base::in | std::ios_base::binary);
    if ( ifs ) {
        uint8_t header[16];
        if (!ifs.read(reinterpret_cast<char*>(&header[0]),0x10)) {
            printf("Error reading NES file\n");
            exit(1);
        }
        uint8_t prgbanks = header[4];
        uint8_t chrbanks = header[5];
        uint8_t mapper = ((header[6] >> 4) & 0xF) | (header[7] & 0xF0);
        if ( mapper != 0 ) {
            printf("Mapper %d not supported\n",mapper);
            exit(1);
        }
        int prgsize = 1024*16*prgbanks;
        int chrsize = 1024*8*chrbanks;
        uint8_t prgrom[prgsize];
        uint8_t chrrom[chrsize];
        if (!ifs.read(reinterpret_cast<char*>(&prgrom[0]),prgsize)) {
            printf("Error reading NES file\n");
            exit(1);
        }
        if ( prgbanks == 1 ) {
            memcpy(&cpu.mem[0x8000],&prgrom[0],prgsize);
            memcpy(&cpu.mem[0xC000],&prgrom[0],prgsize);
        } else if ( prgbanks == 2 ) {
            memcpy(&cpu.mem[0x8000],&prgrom[0],prgsize);
        }

    }
}

void checkCyclesAndException()
{
    EXPECT_EQ(cpu.cycles, 0);
    EXPECT_FALSE(cpu.exception);
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

// Test NOP instruction
TEST(CPU_6502, NOP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::NOP_IM;
    cpu.execute(2);
    checkCyclesAndException();
}

// Test ADC immediate instruction
TEST(CPU_6502, ADC_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::ADC_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC immediate instruction with overflow
TEST(CPU_6502, ADC_IM_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_IM;
    cpu.mem[0x1003] = 0xFF;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC immediate instruction with overflow resulting in 0
TEST(CPU_6502, ADC_IM_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_IM;
    cpu.mem[0x1003] = 0x1;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC ZeroPage
TEST(CPU_6502, ADC_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::ADC_ZP;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x00AC] = 0x4;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC ZeroPage with overflow
TEST(CPU_6502, ADC_ZP_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ZP;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x00AC] = 0xFF;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC ZeroPage with overflow resulting in 0
TEST(CPU_6502, ADC_ZP_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ZP;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x00AC] = 0x1;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test ADC ZeroPage X
TEST(CPU_6502, ADC_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ZP_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x4;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC ZeroPage X with overflow
TEST(CPU_6502, ADC_ZP_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ZP_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC ZeroPage X with overflow resulting in 0
TEST(CPU_6502, ADC_ZP_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ZP_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC Absolute
TEST(CPU_6502, ADC_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::ADC_ABS;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x1004] = 0x10;
    cpu.mem[0x10AC] = 0x4;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute with overflow
TEST(CPU_6502, ADC_ABS_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ABS;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x1004] = 0x10;
    cpu.mem[0x10AC] = 0xFF;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Absolute with overflow resulting in 0
TEST(CPU_6502, ADC_ABS_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::ADC_ABS;
    cpu.mem[0x1003] = 0xAC;
    cpu.mem[0x1004] = 0x10;
    cpu.mem[0x10AC] = 0x1;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC Absolute X
TEST(CPU_6502, ADC_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x4;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute X, page crossed
TEST(CPU_6502, ADC_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0x27;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xFE;
    cpu.mem[0x1006] = 0x00;
    cpu.mem[0x0125] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute X with overflow
TEST(CPU_6502, ADC_ABS_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Absolute X with overflow resulting in 0
TEST(CPU_6502, ADC_ABS_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test ADC Absolute Y
TEST(CPU_6502, ADC_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x4;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute Y, page crossed
TEST(CPU_6502, ADC_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0x27;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xFE;
    cpu.mem[0x1006] = 0x00;
    cpu.mem[0x0125] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Absolute Y with overflow
TEST(CPU_6502, ADC_ABS_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0xFF;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Absolute Y with overflow resulting in 0
TEST(CPU_6502, ADC_ABS_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_ABS_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x1006] = 0x10;
    cpu.mem[0x10BB] = 0x1;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test ADC Indirect X
TEST(CPU_6502, ADC_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x001B] = 0x25;
    cpu.mem[0x001C] = 0x40;
    cpu.mem[0x4025] = 0x4;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect X, ZP wrap
TEST(CPU_6502, ADC_IND_X_ZP_WRAP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xCD;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0x32;
    cpu.mem[0x00FF] = 0x25;
    cpu.mem[0x0000] = 0x40;
    cpu.mem[0x4025] = 0x4;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect X with overflow
TEST(CPU_6502, ADC_IND_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x25;
    cpu.mem[0x00BC] = 0x40;
    cpu.mem[0x4025] = 0xFF;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Indirect X with overflow resulting in 0
TEST(CPU_6502, ADC_IND_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDX_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_X;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00BB] = 0x25;
    cpu.mem[0x00BC] = 0x40;
    cpu.mem[0x4025] = 0x1;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test ADC Indirect Y
TEST(CPU_6502, ADC_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4034] = 0x4;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect Y, ZP wrap
TEST(CPU_6502, ADC_IND_Y_ZP_WRAP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x2;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xCD;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0x32;
    cpu.mem[0x0032] = 0x80;
    cpu.mem[0x0033] = 0x40;
    cpu.mem[0x414D] = 0x4;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x6);
    checkCyclesAndException();
}

// Test ADC Indirect Y with overflow
TEST(CPU_6502, ADC_IND_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00AC] = 0x25;
    cpu.mem[0x00AD] = 0x40;
    cpu.mem[0x4034] = 0xFF;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0xFE);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test ADC Indirect Y with overflow resulting in 0
TEST(CPU_6502, ADC_IND_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xFF;
    cpu.mem[0x1002] = INS::LDY_IM;
    cpu.mem[0x1003] = 0xF;
    cpu.mem[0x1004] = INS::ADC_IND_Y;
    cpu.mem[0x1005] = 0xAC;
    cpu.mem[0x00AC] = 0x25;
    cpu.mem[0x00AD] = 0x40;
    cpu.mem[0x4034] = 0x1;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test SBC immediate instruction
TEST(CPU_6502, SBC_IM) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::SBC_IM;
    cpu.mem[0x1003] = 0x2;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC immediate instruction with overflow
TEST(CPU_6502, SBC_IM_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x02;
    cpu.mem[0x1002] = INS::SEC_IM;
    cpu.mem[0x1003] = INS::SBC_IM;
    cpu.mem[0x1004] = 0x10;
    cpu.execute(6);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC immediate instruction with margin
TEST(CPU_6502, SBC_IM_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC immediate instruction with margin
TEST(CPU_6502, SBC_IM_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_IM;
    cpu.mem[0x1003] = 0xAC;
    cpu.execute(4);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC ZeroPage instruction
TEST(CPU_6502, SBC_ZP) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::SBC_ZP;
    cpu.mem[0x1003] = 0x8B;
    cpu.mem[0x008B] = 0x2;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC ZeroPage instruction with overflow
TEST(CPU_6502, SBC_ZP_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0x02;
    cpu.mem[0x1002] = INS::SEC_IM;
    cpu.mem[0x1003] = INS::SBC_ZP;
    cpu.mem[0x1004] = 0x25;
    cpu.mem[0x0025] = 0x10;
    cpu.execute(7);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage instruction with margin
TEST(CPU_6502, SBC_ZP_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_ZP;
    cpu.mem[0x1003] = 0x25;
    cpu.mem[0x0025] = 0xAD;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage instruction overflow, result 0
TEST(CPU_6502, SBC_ZP_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDA_IM;
    cpu.mem[0x1001] = 0xAD;
    cpu.mem[0x1002] = INS::SBC_ZP;
    cpu.mem[0x1003] = 0xF0;
    cpu.mem[0x00F0] = 0xAC;
    cpu.execute(5);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC ZeroPage X instruction
TEST(CPU_6502, SBC_ZP_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ZP_X;
    cpu.mem[0x1005] = 0x8B;
    cpu.mem[0x008F] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC ZeroPage X instruction with overflow
TEST(CPU_6502, SBC_ZP_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ZP_X;
    cpu.mem[0x1006] = 0x25;
    cpu.mem[0x0028] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage X instruction with margin
TEST(CPU_6502, SBC_ZP_X_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ZP_X;
    cpu.mem[0x1005] = 0x25;
    cpu.mem[0x0029] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC ZeroPage X instruction overflow, result 0
TEST(CPU_6502, SBC_ZP_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ZP_X;
    cpu.mem[0x1005] = 0xF0;
    cpu.mem[0x00F4] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC Absolute instruction
TEST(CPU_6502, SBC_ABS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2019] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Absolute instruction with overflow
TEST(CPU_6502, SBC_ABS_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ABS;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1007] = 0x20;
    cpu.mem[0x2019] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute instruction with margin
TEST(CPU_6502, SBC_ABS_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2019] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute instruction overflow, result 0
TEST(CPU_6502, SBC_ABS_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2019] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC Absolute X instruction
TEST(CPU_6502, SBC_ABS_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Absolute X instruction with page cross
TEST(CPU_6502, SBC_ABS_X_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0xF7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2110] = 0x2;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}


// Test SBC Absolute X instruction with overflow
TEST(CPU_6502, SBC_ABS_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ABS_X;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1007] = 0x20;
    cpu.mem[0x201C] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute X instruction with margin
TEST(CPU_6502, SBC_ABS_X_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute X instruction overflow, result 0
TEST(CPU_6502, SBC_ABS_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_X;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}



// Test SBC Absolute Y instruction
TEST(CPU_6502, SBC_ABS_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0x2;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}


// Test SBC Absolute Y instruction with page cross
TEST(CPU_6502, SBC_ABS_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xF7;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x2110] = 0x2;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Absolute Y instruction with overflow
TEST(CPU_6502, SBC_ABS_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_ABS_Y;
    cpu.mem[0x1006] = 0x19;
    cpu.mem[0x1007] = 0x20;
    cpu.mem[0x201C] = 0x10;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute Y instruction with margin
TEST(CPU_6502, SBC_ABS_Y_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAD;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Absolute Y instruction overflow, result 0
TEST(CPU_6502, SBC_ABS_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_ABS_Y;
    cpu.mem[0x1005] = 0x19;
    cpu.mem[0x1006] = 0x20;
    cpu.mem[0x201D] = 0xAC;
    cpu.execute(8);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction
TEST(CPU_6502, SBC_IND_X) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x0010] = 0x25;
    cpu.mem[0x0011] = 0x40;
    cpu.mem[0x4025] = 0x2;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction with overflow
TEST(CPU_6502, SBC_IND_X_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_IND_X;
    cpu.mem[0x1006] = 0x0C;
    cpu.mem[0x000F] = 0x25;
    cpu.mem[0x0010] = 0x40;
    cpu.mem[0x4025] = 0x10;
    cpu.execute(12);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction with margin
TEST(CPU_6502, SBC_IND_X_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x0010] = 0x25;
    cpu.mem[0x0011] = 0x40;
    cpu.mem[0x4025] = 0xAD;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect X instruction overflow, result 0
TEST(CPU_6502, SBC_IND_X_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDX_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_X;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x0010] = 0x25;
    cpu.mem[0x0011] = 0x40;
    cpu.mem[0x4025] = 0xAC;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}


// Test SBC Indirect Y instruction
TEST(CPU_6502, SBC_IND_Y) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4029] = 0x2;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction with page cross
TEST(CPU_6502, SBC_IND_Y_PAGE_CROSS) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0xED;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x4;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0xBC;
    cpu.mem[0x00BC] = 0x25;
    cpu.mem[0x00BD] = 0x40;
    cpu.mem[0x4112] = 0x2;
    cpu.execute(10);
    EXPECT_EQ(cpu.A,0x1);
    EXPECT_EQ(cpu.P.C,0x1);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction with overflow
TEST(CPU_6502, SBC_IND_Y_OVERFLOW) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x3;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0x02;
    cpu.mem[0x1004] = INS::SEC_IM;
    cpu.mem[0x1005] = INS::SBC_IND_Y;
    cpu.mem[0x1006] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4028] = 0x10;
    cpu.execute(11);
    EXPECT_EQ(cpu.A,0xF2);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction with margin
TEST(CPU_6502, SBC_IND_Y_OVERFLOW_MARGIN) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4029] = 0xAD;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_EQ(cpu.P.C,0);
    EXPECT_EQ(cpu.P.N,1);
    EXPECT_EQ(cpu.P.Z,0);
    checkCyclesAndException();
}

// Test SBC Indirect Y instruction overflow, result 0
TEST(CPU_6502, SBC_IND_Y_OVERFLOW_ZERO) {
    cpu.reset();
    cpu.mem[0x1000] = INS::LDY_IM;
    cpu.mem[0x1001] = 0x4;
    cpu.mem[0x1002] = INS::LDA_IM;
    cpu.mem[0x1003] = 0xAD;
    cpu.mem[0x1004] = INS::SBC_IND_Y;
    cpu.mem[0x1005] = 0x0C;
    cpu.mem[0x000C] = 0x25;
    cpu.mem[0x000D] = 0x40;
    cpu.mem[0x4029] = 0xAC;
    cpu.execute(9);
    EXPECT_EQ(cpu.A,0x0);
    EXPECT_EQ(cpu.P.C,1);
    EXPECT_EQ(cpu.P.N,0);
    EXPECT_EQ(cpu.P.Z,1);
    checkCyclesAndException();
}

int main( int argc, char* argv[])
{
    // cpu.reset();
    // loadNESFile();
    // cpu.PC = ( cpu.mem[0xFFFC] | (cpu.mem[0xFFFD] << 8));
    // cpu.execute(100);
    // cpu.dumpRegister();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

