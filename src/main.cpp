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

