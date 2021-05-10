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

    //dec instructions
    DEX_IM = 0xCA,
    DEY_IM = 0x88,
    DEC_ZP = 0xC6,
    DEC_ZP_X = 0xD6,
    DEC_ABS = 0xCE,
    DEC_ABS_X = 0xDE,

    //dec instructions
    INX_IM = 0xE8,
    INY_IM = 0xC8,
    INC_ZP = 0xE6,
    INC_ZP_X = 0xF6,
    INC_ABS = 0xEE,
    INC_ABS_X = 0xFE,

    // Jump instructions
    JMP_ABS = 0x4C,
    JMP_IND = 0x6C,
    JSR_ABS = 0x20,
    RTS_IMP = 0x60,

    // Push instructions
    PHA_IMP = 0x48,
    PHP_IMP = 0x08,

    // pull instructions
    PLA_IMP = 0x68,
    PLP_IMP = 0x28,
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

    void setStatusBits( uint8_t byte ) {
        P.C = (byte & (1 << 0)) != 0;
        P.Z = (byte & (1 << 1)) != 0;
        P.I = (byte & (1 << 2)) != 0;
        P.D = (byte & (1 << 3)) != 0;
        P.B = (byte & (1 << 4)) != 0;
        P.V = (byte & (1 << 5)) != 0;
        P.N = (byte & (1 << 6)) != 0;
    };

    uint8_t getStatusByte() {
        return (P.N << 6|P.V << 5|P.B << 4|P.D << 3|P.I << 2|P.Z << 1|P.C << 0);
    };

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

    void dumpRegister()
    {
        for ( int i = PC-10; i <PC+10; i++) {
            if ( i == PC ) {
                printf("0x%.4x: 0x%.2x <<<\n",i,mem[i]);
            } else {
                printf("0x%.4x: 0x%.2x\n",i,mem[i]);
            }

        }
    }

    void dumpStack()
    {
        int rowcount = 0;
        for ( uint16_t i = 0x01FF; i >= 0x100; i-- ) {
            if ( (rowcount % 16) == 0 ) {
                printf("\n0x%.4x",i);
            }
            rowcount++;
            printf(" %.2x",mem[i]);
        }
        printf("\n");
    }

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

    void M_status_flags( uint8_t M )
    {
        if ( M == 0x0 ) {
            P.Z = 1;
        }
        if ( M & 0b10000000 ) {
            P.N = 1;
        }
    };

    void execute(int8_t c)
    {
        cycles = c;
        while ( cycles > 0 && exception == false ) {
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
                case INS::DEX_IM:
                    {
                        cycles--;
                        X--;
                        X_status_flags();
                    }
                    break;
                case INS::DEY_IM:
                    {
                        cycles--;
                        Y--;
                        Y_status_flags();
                    }
                    break;
                case INS::INX_IM:
                    {
                        cycles--;
                        X++;
                        X_status_flags();
                    }
                    break;
                case INS::INY_IM:
                    {
                        cycles--;
                        Y++;
                        Y_status_flags();
                    }
                    break;
                case INS::DEC_ZP:
                    {
                        uint8_t Byte = readByte();
                        cycles--; // get value from zero page
                        cycles--; // decrease value from zero page
                        cycles--; // set value to zero page
                        mem[Byte]--;
                        M_status_flags(mem[Byte]);
                    }
                    break;
                case INS::DEC_ZP_X:
                    {
                        uint8_t Byte = readByte();
                        cycles--; // add X to address
                        cycles--; // get value from zero page
                        cycles--; // decrease value from zero page
                        cycles--; // set value to zero page
                        mem[Byte+X]--;
                        M_status_flags(mem[Byte+X]);
                    }
                    break;
                case INS::INC_ZP:
                    {
                        uint8_t Byte = readByte();
                        cycles--; // get value from zero page
                        cycles--; // increase value from zero page
                        cycles--; // set value to zero page
                        mem[Byte]++;
                        M_status_flags(mem[Byte]);
                    }
                    break;
                case INS::INC_ZP_X:
                    {
                        uint8_t Byte = readByte();
                        cycles--; // add X to address
                        cycles--; // get value from zero page
                        cycles--; // increase value from zero page
                        cycles--; // set value to zero page
                        mem[Byte+X]++;
                        M_status_flags(mem[Byte+X]);
                    }
                    break;
                case INS::DEC_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // read value from low + high
                        cycles--; // increment value
                        cycles--; // set value to memory
                        mem[( low | (high << 8))]--;
                        M_status_flags(mem[( low | (high << 8))]);
                    }
                    break;
                case INS::DEC_ABS_X:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // add X to addr
                        cycles--; // read value from low + high
                        cycles--; // decrease value
                        cycles--; // set value to memory
                        A = mem[(low | (high << 8))+X]--;
                        M_status_flags(mem[(low | (high << 8))+X]);
                    }
                    break;
                case INS::INC_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // read value from low + high
                        cycles--; // increment value
                        cycles--; // set value to memory
                        mem[( low | (high << 8))]++;
                        M_status_flags(mem[(low | (high << 8))]);
                    }
                    break;
                case INS::INC_ABS_X:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        cycles--; // add X to addr
                        cycles--; // read value from low + high
                        cycles--; // increase value
                        cycles--; // set value to memory
                        A = mem[(low | (high << 8))+X]++;
                        M_status_flags(mem[(low | (high << 8))+X]);
                    }
                    break;
                case INS::JMP_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        PC = ( low | (high << 8));
                    }
                    break;
                case INS::JMP_IND:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        uint8_t low_real = mem[(low|(high << 8))];
                        uint8_t high_real = mem[(low|(high << 8))+1];
                        cycles--; // read byte from memory
                        cycles--; // read byte from memory
                        PC = ( low_real | (high_real << 8));
                    }
                    break;
                case INS::JSR_ABS:
                    {
                        uint8_t low = readByte();
                        uint8_t high = readByte();
                        uint16_t last_addr = PC-1;
                        mem[0x100 + S--] = (last_addr >> 8);
                        mem[0x100 + S--] = (last_addr & 0xFF);
                        cycles--; // push high to stack
                        cycles--; // push low to stack
                        cycles--; // set PC
                        PC = ( low | (high << 8));
                    }
                    break;
                case INS::RTS_IMP:
                    {
                        uint8_t low = mem[0x100 + ++S];
                        uint8_t high = mem[0x100 + ++S];
                        cycles--; // read low from stack
                        cycles--; // read high from stack
                        cycles--; // inc S
                        cycles--; // inc S
                        cycles--; // set PC
                        PC = ( low | (high << 8))+1;
                    }
                    break;
                case INS::PHA_IMP:
                    {
                        cycles--; // read accumulator
                        cycles--; // Write to stack and decrement stack
                        mem[0x100 + S--] = A;
                    }
                    break;
                case INS::PHP_IMP:
                    {
                        cycles--; // Read status byte
                        cycles--; // Write to stack and decrement stack
                        mem[0x100 + S--] = getStatusByte();
                    }
                    break;
                case INS::PLA_IMP:
                    {
                        cycles--; // increment stack
                        cycles--; // read from stack
                        cycles--; // set accumulator to value
                        A = mem[0x100 + ++S];
                        A_status_flags();
                    }
                    break;
                case INS::PLP_IMP:
                    {
                        cycles--; // increment stack
                        cycles--; // read from stack
                        cycles--; // set statusregister to value
                        setStatusBits(mem[0x100 + ++S]);
                    }
                    break;
                default:
                    printf("Unhandled instruction: 0x%x\n", ins);
                    exception = true;
                    dumpRegister();
                    break;
            }
        }
    };
};
