#include "6502.h"
#include "gtest/gtest.h"
#include <string.h>

struct CPU cpu;

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
    cpu.powerOn( 0x1000 );
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
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = INS::NOP_EA;
    cpu.execute(2);
    checkCyclesAndException();
}

// Test BNE
// 0600: a2 08 ca 8e 00 02 e0 03 d0 f8 8e 01 02 00
// Load X to 8, decrease X, compare X with 3, jump back to decrease if not equal
TEST(CPU_6502, BNE_PROGRAM) {
    cpu.powerOn( 0x1000 );
    cpu.mem[0x1000] = 0xA2;
    cpu.mem[0x1001] = 0x08;
    cpu.mem[0x1002] = 0xCA;
    cpu.mem[0x1003] = 0x8E;
    cpu.mem[0x1004] = 0x00;
    cpu.mem[0x1005] = 0x02;
    cpu.mem[0x1006] = 0xE0;
    cpu.mem[0x1007] = 0x03;
    cpu.mem[0x1008] = 0xD0;
    cpu.mem[0x1009] = 0xF8;
    cpu.mem[0x100A] = 0x8E;
    cpu.mem[0x100B] = 0x01;
    cpu.mem[0x100C] = 0x02;
    cpu.mem[0x100D] = 0x00;
    cpu.execute(100);
    EXPECT_EQ(cpu.X,0x3);
}

// Run Blargg CPU_reset/registers.nes test rom
TEST(CPU_6502, BLARGG_registers) {
    cpu.loadNESFile( "test-roms/blargg/cpu_reset/registers.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg CPU_reset/ram_after_reset.nes test rom
TEST(CPU_6502, BLARGG_ram_after_reset) {
    cpu.loadNESFile( "test-roms/blargg/cpu_reset/ram_after_reset.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run nestest.nes test rom
// TEST(CPU_6502, NESTEST_ROM) {
//     cpu.loadNESFile( "test-roms/nestest/nestest.nes" );
//     cpu.powerOn( 0xC000 );
//     printf("Will start with PC: %x\n",cpu.PC);
//     bool done = false;
//     while ( done == false ) {
//         cpu.execute(100000);
        // if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
        //     if ( cpu.mem[0x6000] == 0x81 ) {
        //         cpu.reset();
        //     }
        //     if ( cpu.mem[0x6000] < 0x80 ) {
        //         done = true;
        //     }
        // }
    //     done = true;
    //     cpu.dumpMemory(0x0000);
    // }
    // cpu.dumpMemory(0x6000);
    // EXPECT_EQ(cpu.mem[0x6000], 0x0);
// }

TEST(CPU_6502, TEST_STATUS_BITS) {
    cpu.powerOn( 0x1000 );
    cpu.P = {0};
    cpu.P.C = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x1);
    cpu.P = {0};
    cpu.P.Z = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x2);
    cpu.P = {0};
    cpu.P.I = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x4);
    cpu.P = {0};
    cpu.P.D = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x8);
    cpu.P = {0};
    cpu.P.B = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x10);
    cpu.P = {0};
    cpu.P.U = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x20);
    cpu.P = {0};
    cpu.P.V = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x40);
    cpu.P = {0};
    cpu.P.N = 1;
    EXPECT_EQ(cpu.getStatusByte(), 0x80);
    cpu.P = {1,1,1,1,1,1,1,1};
    EXPECT_EQ(cpu.getStatusByte(), 0xFF);
    checkCyclesAndException();
}

// Run Blargg cpu/01-basics.nes test rom
TEST(CPU_6502, BLARGG_01_BASICS_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/01-basics.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/02-implied.nes test rom
TEST(CPU_6502, BLARGG_02_IMPLIED_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/02-implied.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/03-immediate.nes test rom
TEST(CPU_6502, BLARGG_03_IMMEDIATE_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/03-immediate.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0xFF);
    }
}

// Run Blargg cpu/04-zero_page.nes test rom
TEST(CPU_6502, BLARGG_04_ZERO_PAGE_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/04-zero_page.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/05-zp_xy.nes test rom
TEST(CPU_6502, BLARGG_05_ZP_XY_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/05-zp_xy.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/06-absolute.nes test rom
TEST(CPU_6502, BLARGG_06_ABSOLUTE_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/06-absolute.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/07-abs_xy.nes test rom
TEST(CPU_6502, BLARGG_07_ABS_XY_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/07-abs_xy.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/08-ind_x.nes test rom
TEST(CPU_6502, BLARGG_08_IND_X_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/08-ind_x.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/09-ind_y.nes test rom
TEST(CPU_6502, BLARGG_09_IND_Y_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/09-ind_y.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/10-branches.nes test rom
TEST(CPU_6502, BLARGG_10_BRANCHES_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/10-branches.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/11-stack.nes test rom
TEST(CPU_6502, BLARGG_11_STACK_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/11-stack.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/12-jmp_jsr.nes test rom
TEST(CPU_6502, BLARGG_12_JMP_JSR_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/12-jmp_jsr.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/13-rts.nes test rom
TEST(CPU_6502, BLARGG_13_RTS_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/13-rts.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/14-rti.nes test rom
TEST(CPU_6502, BLARGG_14_RTI_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/14-rti.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/15-brk.nes test rom
TEST(CPU_6502, BLARGG_15_BRK_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/15-brk.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}



// Run Blargg cpu/16-special.nes test rom
TEST(CPU_6502, BLARGG_16_SPECIAL_NES) {
    cpu.loadNESFile( "test-roms/blargg/cpu/16-special.nes" );
    cpu.powerOn();
    bool done = false;
    while ( done == false && cpu.exception == false ) {
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

// Run Blargg cpu/official_only.nes test rom
TEST(CPU_6502, BLARGG_OFFICIAL_ONLY_NES) {
    if ( cpu.loadNESFile( "test-roms/blargg/cpu/official_only.nes" ) == false ) {
        FAIL();
    }
    cpu.powerOn();
    bool done = false;
    printf("...\n");
    while ( done == false && cpu.exception == false ) {
        printf("...\n");
        cpu.execute(4000000);
        if ( cpu.mem[0x6001] == 0xDE && cpu.mem[0x6002] == 0xB0 && cpu.mem[0x6003] == 0x61 ) {
            if ( cpu.mem[0x6000] == 0x81 ) {
                cpu.reset();
            }
            if ( cpu.mem[0x6000] < 0x80 ) {
                done = true;
            }
        }
    }
    EXPECT_EQ(cpu.mem[0x6000], 0x0);
    if ( cpu.mem[0x6000] != 0x0 && cpu.exception == false ) {
        cpu.dumpMemory(0x6000, 0x40);
    }
}

int main( int argc, char* argv[])
{

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

