#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 0xFFFF

enum INS
{
    // LDA
    LDA_IM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZP_X = 0xB5,
    LDA_ABS = 0xAD,
    LDA_ABS_X = 0xBD,
    LDA_ABS_Y = 0xB9,
    LDA_IND_X = 0xA1,
    LDA_IND_Y = 0xB1,

    // LDX
    LDX_IM = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZP_Y = 0xB6,
    LDX_ABS = 0xAE,
    LDX_ABS_Y = 0xBE,

    // LDY
    LDY_IM = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZP_X = 0xB4,
    LDY_ABS = 0xAC,
    LDY_ABS_X = 0xBC,

    // STA
    STA_ABS = 0x8D,
    // more here...

    // Clear/set flags
    CLC_IM = 0x18,
    CLD_IM = 0xD8,
    CLI_IM = 0x58,
    CLV_IM = 0xB8,
    SEC_IM = 0x38,
    SED_IM = 0xF8,
    SEI_IM = 0x78,

    // NOP
    NOP_IM = 0xEA,

    // Transfer instructions
    TAX_IM = 0xAA,
    TAY_IM = 0xA8,
    TSX_IM = 0xBA,
    TXA_IM = 0x8A,
    TXS_IM = 0x9A,
    TYA_IM = 0x98,

    //inc/dec instructions
    DEX_IM = 0xCA,
};

struct CPU
{
    uint8_t mem[MEM_SIZE];
    uint16_t PC; // program counter
    uint8_t S; // stack pointer

    uint8_t cycles;

    bool exception; // flag only used for unit tests

    // Processor status bits
    struct {
        uint8_t N : 1; // negative
        uint8_t V : 1; // overflow
        uint8_t B : 1; // break
        uint8_t D : 1; // deccimal mode
        uint8_t I : 1; // interrupt disable mode
        uint8_t Z : 1; // zero flag
        uint8_t C : 1; // carry flag
    } P;

    uint8_t A; // accumulator
    uint8_t X; // X register
    uint8_t Y; // Y register

    void reset()
    {
        A = 0x0;
        X = 0x0;
        Y = 0x0;
        PC = 0x1000;
        S = 0xFF;
        cycles = 0;
        P = {0};
        exception = false;
        for ( int i = 0; i < MEM_SIZE; i++ ) {
            mem[i] = 0;
        }
    };

    uint8_t readByte()
    {
        cycles--;
        return mem[PC++];
    };

    void A_status_flags()
    {
        if ( A == 0x0 ) {
            P.Z = 1;
        }
        if ( A & 0b10000000 ) {
            P.N = 1;
        }
    };

    void X_status_flags()
    {
        if ( X == 0x0 ) {
            P.Z = 1;
        }
        if ( X & 0b10000000 ) {
            P.N = 1;
        }
    };
    void Y_status_flags()
    {
        if ( Y == 0x0 ) {
            P.Z = 1;
        }
        if ( Y & 0b10000000 ) {
            P.N = 1;
        }
    };

    void execute(int8_t c)
    {
        cycles = c;
        while ( cycles > 0 ) {
            uint8_t ins = readByte();
            switch ( ins ) {
                case INS::LDA_IM:
                    {
                        uint8_t Byte = readByte();
                        A = Byte;
                        A_status_flags();
                    }
                    break;
                case INS::LDX_IM:
                    {
                        uint8_t Byte = readByte();
                        X = Byte;
                        X_status_flags();
                    }
                    break;
                case INS::LDY_IM:
                    {
                        uint8_t Byte = readByte();
                        Y = Byte;
                        Y_status_flags();
                    }
                    break;
                case INS::LDA_ZP:
                    {
                        uint8_t Byte = readByte();
                        A = mem[Byte];
                        cycles--; // takes one cycle to load accumulator from ZeroPage
                        A_status_flags();
                    }
                    break;
                case INS::LDX_ZP:
                    {
                        uint8_t Byte = readByte();
                        X = mem[Byte];
                        cycles--; // takes one cycle to load accumulator from ZeroPage
                        X_status_flags();
                    }
                    break;
                case INS::LDY_ZP:
                    {
                        uint8_t Byte = readByte();
                        Y = mem[Byte];
                        cycles--; // takes one cycle to load accumulator from ZeroPage
                        Y_status_flags();
                    }
                    break;
                case INS::LDA_ZP_X:
                    {
                        uint8_t Byte = readByte();
                        A = mem[Byte+X];
                        cycles--; // takes one cycle to add X register
                        cycles--; // takes one cycle to load accumulator from ZeroPage
                        A_status_flags();
                    }
                    break;
                case INS::LDX_ZP_Y:
                    {
                        uint8_t Byte = readByte();
                        X = mem[Byte+Y];
                        cycles--; // takes one cycle to add X register
                        cycles--; // takes one cycle to load accumulator from ZeroPage
                        X_status_flags();
                    }
                    break;
                case INS::LDY_ZP_X:
                    {
                        uint8_t Byte = readByte();
                        Y = mem[Byte+X];
                        cycles--; // takes one cycle to add X register
                        cycles--; // takes one cycle to load accumulator from ZeroPage
                        Y_status_flags();
                    }
                    break;
                case INS::LDA_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        A = mem[low | (high << 8)];
                        A_status_flags();
                    }
                    break;
                case INS::LDX_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        X = mem[low | (high << 8)];
                        X_status_flags();
                    }
                    break;
                case INS::LDY_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        Y = mem[low | (high << 8)];
                        Y_status_flags();
                    }
                    break;
                case INS::LDA_ABS_X:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        A = mem[(low | (high << 8))+X];
                        if ( low + X > 0xFF ) {
                            cycles--;
                        }
                        A_status_flags();
                    }
                    break;
                case INS::LDX_ABS_Y:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        X = mem[(low | (high << 8))+Y];
                        if ( low + Y > 0xFF ) {
                            cycles--;
                        }
                        X_status_flags();
                    }
                    break;
                case INS::LDY_ABS_X:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        Y = mem[(low | (high << 8))+X];
                        if ( low + X > 0xFF ) {
                            cycles--;
                        }
                        Y_status_flags();
                    }
                    break;
                case INS::LDA_ABS_Y:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // one cycle to bitshift
                        A = mem[(low | (high << 8))+Y];
                        if ( low + Y > 0xFF ) {
                            cycles--;
                        }
                        A_status_flags();
                    }
                    break;
                case INS::LDA_IND_X:
                    {
                        uint8_t Byte = readByte();
                        cycles--; // read from address
                        // check if we need to handle ZP wrap
                        uint8_t offset = 0;
                        if ( Byte+X+1 > 0xFF ) {
                            offset = 0xFF;
                        }
                        uint8_t low = mem[Byte+X-offset];
                        cycles--; // one cycle to get low
                        uint8_t high = mem[Byte+X+1-offset];
                        cycles--; // one cycle to get high
                        // handling for ZP barrier
                        A = mem[(low | (high << 8))];
                        cycles--; // one cycle to bitshift
                        A_status_flags();
                    }
                    break;
                case INS::LDA_IND_Y:
                    {
                        uint8_t Byte = readByte();
                        cycles--; // one cycle to get low
                        uint8_t low = mem[Byte];
                        cycles--; // one cycle to get high
                        uint8_t high = mem[Byte+1];
                        cycles--; // read from addr
                        A = mem[( (low + Y) | (high << 8))];
                        A_status_flags();
                    }
                    break;
                case INS::STA_ABS:
                    {
                        cycles--; // one cycle to get low
                        uint8_t low = readByte();
                        cycles--; // one cycle to get high
                        uint8_t high = readByte();
                        cycles--; // set memory
                        mem[( low | (high << 8))] = A;
                    }
                    break;
                case INS::CLC_IM:
                    {
                        cycles--;
                        P.C = 0;
                    }
                    break;
                case INS::CLD_IM:
                    {
                        cycles--;
                        P.D = 0;
                    }
                    break;
                case INS::CLI_IM:
                    {
                        cycles--;
                        P.I = 0;
                    }
                    break;
                case INS::CLV_IM:
                    {
                        cycles--;
                        P.V = 0;
                    }
                    break;
                case INS::SEC_IM:
                    {
                        cycles--;
                        P.C = 1;
                    }
                    break;
                case INS::SED_IM:
                    {
                        cycles--;
                        P.D = 1;
                    }
                    break;
                case INS::SEI_IM:
                    {
                        cycles--;
                        P.I = 1;
                    }
                    break;
                case INS::NOP_IM:
                    {
                        cycles--;
                    }
                    break;
                case INS::TAX_IM:
                    {
                        cycles--;
                        X = A;
                        X_status_flags();
                    }
                    break;
                case INS::TAY_IM:
                    {
                        cycles--;
                        Y = A;
                        Y_status_flags();
                    }
                    break;
                case INS::TSX_IM:
                    {
                        cycles--;
                        X = S;
                        X_status_flags();
                    }
                    break;
                case INS::TXA_IM:
                    {
                        cycles--;
                        A = X;
                        A_status_flags();
                    }
                    break;
                case INS::TXS_IM:
                    {
                        cycles--;
                        S = X;
                    }
                    break;
                case INS::TYA_IM:
                    {
                        cycles--;
                        A = Y;
                        A_status_flags();
                    }
                    break;
                default:
                    printf("Unhandled instruction: 0x%x\n", ins);
                    exception = true;
                    break;
            }
        }
    };
};
