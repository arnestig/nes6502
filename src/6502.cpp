#include "6502.h"

void CPU::branchInstruction( bool takeBranch )
{
    uint8_t byte = readByte();
    if ( takeBranch ) {
        cycles--;
        PC += byte;
        if (( PC >> 8) != ( (PC-byte) >> 8 )) { // if to a new page
            cycles--;
        }
    }
};

void CPU::setStatusBits( uint8_t byte ) {
    P.C = (byte & (1 << 0)) != 0;
    P.Z = (byte & (1 << 1)) != 0;
    P.I = (byte & (1 << 2)) != 0;
    P.D = (byte & (1 << 3)) != 0;
    P.B = (byte & (1 << 4)) != 0;
    P.V = (byte & (1 << 5)) != 0;
    P.N = (byte & (1 << 6)) != 0;
};

uint8_t CPU::getStatusByte() {
    return (P.N << 6|P.V << 5|P.B << 4|P.D << 3|P.I << 2|P.Z << 1|P.C << 0);
};

void CPU::reset()
{
    // reset memory
    for ( int i = 0; i < MEM_SIZE; i++ ) {
        mem[i] = 0;
    }

    // registers
    A = 0x0;
    X = 0x0;
    Y = 0x0;

    // NMI vector

    // reset vector
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x10;

    // BRK vector

    // load PC from reset vector
    PC = ( mem[0xFFFC] | (mem[0xFFFD] << 8));

    // stack pointer
    S = 0xFF;

    // cycles, used for testing
    cycles = 0;

    // process status
    P = {0};

    // exception, used for testing
    exception = false;
};

// read one byte from mem and increment program counter
uint8_t CPU::readByte()
{
    cycles--;
    return mem[PC++];
};

// dump memory at address + 20 bytes
void CPU::dumpMemory( uint16_t addr )
{
    for ( int i = addr; i <addr+20; i++) {
        if ( i == PC ) {
            printf("0x%.4x: 0x%.2x <<<\n",i,mem[i]);
        } else {
            printf("0x%.4x: 0x%.2x\n",i,mem[i]);
        }
    }
}

// dump memory at current PC +- 10 bytes
void CPU::dumpRegister()
{
    for ( int i = PC-10; i <PC+10; i++) {
        if ( i == PC ) {
            printf("0x%.4x: 0x%.2x <<<\n",i,mem[i]);
        } else {
            printf("0x%.4x: 0x%.2x\n",i,mem[i]);
        }
    }
}

// dump the stack
void CPU::dumpStack()
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

void CPU::A_status_flags()
{
    if ( A == 0x0 ) {
        P.Z = 1;
    }
    if ( A & 0b10000000 ) {
        P.N = 1;
    }
};

void CPU::X_status_flags()
{
    if ( X == 0x0 ) {
        P.Z = 1;
    }
    if ( X & 0x80 ) {
        P.N = 1;
    }
};
void CPU::Y_status_flags()
{
    if ( Y == 0x0 ) {
        P.Z = 1;
    }
    if ( Y & 0x80 ) {
        P.N = 1;
    }
};

void CPU::M_status_flags( uint8_t M )
{
    if ( M == 0x0 ) {
        P.Z = 1;
    }
    if ( M & 0x80 ) {
        P.N = 1;
    }
};

void CPU::execute(int8_t c)
{
    cycles = c;
    while ( cycles > 0 && exception == false ) {
        uint8_t ins = readByte();
        switch ( ins ) {
            case INS::LDA_IM:
                {
                    uint8_t byte = readByte();
                    A = byte;
                    A_status_flags();
                }
                break;
            case INS::LDX_IM:
                {
                    uint8_t byte = readByte();
                    X = byte;
                    X_status_flags();
                }
                break;
            case INS::LDY_IM:
                {
                    uint8_t byte = readByte();
                    Y = byte;
                    Y_status_flags();
                }
                break;
            case INS::LDA_ZP:
                {
                    uint8_t byte = readByte();
                    A = mem[byte];
                    cycles--; // takes one cycle to load accumulator from ZeroPage
                    A_status_flags();
                }
                break;
            case INS::LDX_ZP:
                {
                    uint8_t byte = readByte();
                    X = mem[byte];
                    cycles--; // takes one cycle to load accumulator from ZeroPage
                    X_status_flags();
                }
                break;
            case INS::LDY_ZP:
                {
                    uint8_t byte = readByte();
                    Y = mem[byte];
                    cycles--; // takes one cycle to load accumulator from ZeroPage
                    Y_status_flags();
                }
                break;
            case INS::LDA_ZP_X:
                {
                    uint8_t byte = readByte();
                    A = mem[byte+X];
                    cycles--; // takes one cycle to add X register
                    cycles--; // takes one cycle to load accumulator from ZeroPage
                    A_status_flags();
                }
                break;
            case INS::LDX_ZP_Y:
                {
                    uint8_t byte = readByte();
                    X = mem[byte+Y];
                    cycles--; // takes one cycle to add X register
                    cycles--; // takes one cycle to load accumulator from ZeroPage
                    X_status_flags();
                }
                break;
            case INS::LDY_ZP_X:
                {
                    uint8_t byte = readByte();
                    Y = mem[byte+X];
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
                    uint8_t byte = readByte();
                    cycles--; // read from address
                    // check if we need to handle ZP wrap
                    uint8_t offset = 0;
                    if ( byte+X+1 > 0xFF ) {
                        offset = 0xFF;
                    }
                    uint8_t low = mem[byte+X-offset];
                    cycles--; // one cycle to get low
                    uint8_t high = mem[byte+X+1-offset];
                    cycles--; // one cycle to get high
                    // handling for ZP barrier
                    A = mem[(low | (high << 8))];
                    cycles--; // one cycle to bitshift
                    A_status_flags();
                }
                break;
            case INS::LDA_IND_Y:
                {
                    uint8_t byte = readByte();
                    cycles--; // one cycle to get low
                    uint8_t low = mem[byte];
                    cycles--; // one cycle to get high
                    uint8_t high = mem[byte + 1];
                    cycles--; // read from addr
                    A = mem[(low | (high << 8)) + Y];
                    A_status_flags();
                }
                break;
            case INS::STA_ZP:
                {
                    uint8_t byte = readByte();
                    cycles--; // write to memory
                    mem[byte] = A;
                }
                break;
            case INS::STA_ZP_X:
                {
                    uint8_t byte = readByte();
                    cycles--; // read X
                    cycles--; // write to memory
                    mem[byte + X] = A;
                }
                break;
            case INS::STA_ABS:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    cycles--; // set memory
                    mem[( low | (high << 8))] = A;
                }
                break;
            case INS::STA_ABS_X:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    cycles--; // read X
                    cycles--; // set memory
                    mem[( low | (high << 8)) + X] = A;
                }
                break;
            case INS::STA_ABS_Y:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    cycles--; // read Y
                    cycles--; // set memory
                    mem[( low | (high << 8)) + Y] = A;
                }
                break;
            case INS::STA_IND_X:
                {
                    uint8_t byte = readByte();
                    // check if we need to handle ZP wrap
                    uint8_t offset = 0;
                    if ( byte+X+1 > 0xFF ) {
                        offset = 0xFF;
                    }
                    uint8_t low = mem[byte+X-offset];
                    uint8_t high = mem[byte+X+1-offset];
                    cycles--; // read from address
                    cycles--; // one cycle to get low
                    cycles--; // one cycle to get high
                    cycles--; // one cycle to bitshift
                    mem[(low | (high << 8))] = A;
                }
                break;
            case INS::STA_IND_Y:
                {
                    uint8_t byte = readByte();
                    uint8_t low = mem[byte];
                    uint8_t high = mem[byte + 1];
                    cycles--; // one cycle to get low
                    cycles--; // one cycle to get high
                    cycles--; // read from addr
                    cycles--; // one cycle to bitshift
                    mem[( low | (high << 8)) + Y] = A;
                }
                break;
            case INS::STX_ZP:
                {
                    uint8_t byte = readByte();
                    cycles--; // write to memory
                    mem[byte] = X;
                }
                break;
            case INS::STX_ZP_Y:
                {
                    uint8_t byte = readByte();
                    cycles--; // read Y
                    cycles--; // write to memory
                    mem[byte + Y] = X;
                }
                break;
            case INS::STX_ABS:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    cycles--; // read X
                    mem[( low | (high << 8))] = X;
                }
                break;
            case INS::STY_ZP:
                {
                    uint8_t byte = readByte();
                    cycles--; // write to memory
                    mem[byte] = Y;
                }
                break;
            case INS::STY_ZP_X:
                {
                    uint8_t byte = readByte();
                    cycles--; // read X
                    cycles--; // write to memory
                    mem[byte + X] = Y;
                }
                break;
            case INS::STY_ABS:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    cycles--; // read Y
                    mem[( low | (high << 8))] = Y;
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
                    uint8_t byte = readByte();
                    cycles--; // get value from zero page
                    cycles--; // decrease value from zero page
                    cycles--; // set value to zero page
                    mem[byte]--;
                    M_status_flags(mem[byte]);
                }
                break;
            case INS::DEC_ZP_X:
                {
                    uint8_t byte = readByte();
                    cycles--; // add X to address
                    cycles--; // get value from zero page
                    cycles--; // decrease value from zero page
                    cycles--; // set value to zero page
                    mem[byte+X]--;
                    M_status_flags(mem[byte+X]);
                }
                break;
            case INS::INC_ZP:
                {
                    uint8_t byte = readByte();
                    cycles--; // get value from zero page
                    cycles--; // increase value from zero page
                    cycles--; // set value to zero page
                    mem[byte]++;
                    M_status_flags(mem[byte]);
                }
                break;
            case INS::INC_ZP_X:
                {
                    uint8_t byte = readByte();
                    cycles--; // add X to address
                    cycles--; // get value from zero page
                    cycles--; // increase value from zero page
                    cycles--; // set value to zero page
                    mem[byte+X]++;
                    M_status_flags(mem[byte+X]);
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
            case INS::RTI_IMP:
                {
                    cycles--; // stuff
                    cycles--; // pull processor status from stack
                    cycles--; // pull PC low from stack
                    cycles--; // pull PC high from stack
                    cycles--; // set PC
                    setStatusBits(mem[0x100 + ++S]);
                    uint8_t low = mem[0x100 + ++S];
                    uint8_t high = mem[0x100 + ++S];
                    PC = ( low | (high << 8));
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
            case INS::BCC_REL: // Carry Clear
                {
                    branchInstruction( P.C == 0 );
                }
                break;
            case INS::BCS_REL: // Carry Set
                {
                    branchInstruction( P.C == 1 );
                }
                break;
            case INS::BEQ_REL: // Equal
                {
                    branchInstruction( P.Z == 1 );
                }
                break;
            case INS::BMI_REL: // If minus
                {
                    branchInstruction( P.N == 1 );
                }
                break;
            case INS::BNE_REL: // Not equal
                {
                    branchInstruction( P.Z == 0 );
                }
                break;
            case INS::BPL_REL: // If positive
                {
                    branchInstruction( P.N == 0 );
                }
                break;
            case INS::BVC_REL: // Overflow clear
                {
                    branchInstruction( P.V == 0 );
                }
                break;
            case INS::BVS_REL: // Overflow set
                {
                    branchInstruction( P.V == 1 );
                }
                break;
            case INS::BRK_IMP:
                {
                    mem[0x100 + S--] = (PC >> 8);
                    mem[0x100 + S--] = (PC & 0xFF);
                    mem[0x100 + S--] = getStatusByte();
                    cycles--; // PCH to stack
                    cycles--; // PCL to stack
                    cycles--; // P to stack
                    cycles--; // 0xFFFE to PC
                    cycles--; // 0xFFFF to PC
                    cycles--; // Break flag to 1
                    PC = ( mem[0xFFFE] | (mem[0xFFFF] << 8));
                    P.B = 1;
                }
                break;
            case INS::ASL_ACC:
            case INS::LSR_ACC:
            case INS::ROL_ACC:
            case INS::ROR_ACC:
                {
                    uint8_t oldC = P.C;
                    if ( ins == INS::ASL_ACC ) {
                        P.C = ( A & 0x80 ) != 0;
                        A = A << 1;
                    } else if ( ins == INS::LSR_ACC ) {
                        P.C = ( A & 0x1 ) != 0;
                        A = A >> 1;
                    } else if ( ins == INS::ROL_ACC ) {
                        P.C = ( A & 0x80 ) != 0;
                        A = (A << 1)|oldC;
                    } else if ( ins == INS::ROR_ACC ) {
                        P.C = ( A & 0x1 ) != 0;
                        A = (A >> 1)|(oldC << 7);
                    }
                    cycles--; // bitshift
                    A_status_flags();
                }
                break;
            case INS::ASL_ZP:
            case INS::LSR_ZP:
            case INS::ROL_ZP:
            case INS::ROR_ZP:
                {
                    uint8_t byte = readByte();
                    uint8_t oldC = P.C;
                    cycles--; // get value from zero page
                    cycles--; // bitshift left
                    cycles--; // set value to zero page
                    if ( ins == INS::ASL_ZP ) {
                        P.C = ( mem[byte] & 0x80 ) != 0;
                        mem[byte] = mem[byte] << 1;
                    } else if ( ins == INS::LSR_ZP ) {
                        P.C = ( mem[byte] & 0x1 ) != 0;
                        mem[byte] = mem[byte] >> 1;
                    } else if ( ins == INS::ROL_ZP ) {
                        P.C = ( mem[byte] & 0x80 ) != 0;
                        mem[byte] = (mem[byte] << 1)|oldC;
                    } else if ( ins == INS::ROR_ZP ) {
                        P.C = ( mem[byte] & 0x1 ) != 0;
                        mem[byte] = (mem[byte] >> 1)|(oldC << 7);
                    }
                    M_status_flags(mem[byte]);
                }
                break;
            case INS::ASL_ZP_X:
            case INS::LSR_ZP_X:
            case INS::ROL_ZP_X:
            case INS::ROR_ZP_X:
                {
                    uint8_t byte = readByte()+X;
                    uint8_t oldC = P.C;
                    cycles--; // add X to address
                    cycles--; // get value from memory
                    cycles--; // bitshift left
                    cycles--; // set value to memory
                    if ( ins == INS::ASL_ZP_X ) {
                        P.C = ( mem[byte] & 0x80 ) != 0;
                        mem[byte] = mem[byte] << 1;
                    } else if ( ins == INS::LSR_ZP_X ) {
                        P.C = ( mem[byte] & 0x1 ) != 0;
                        mem[byte] = mem[byte] >> 1;
                    } else if ( ins == INS::ROL_ZP_X ) {
                        P.C = ( mem[byte] & 0x80 ) != 0;
                        mem[byte] = (mem[byte] << 1)|oldC;
                    } else if ( ins == INS::ROR_ZP_X ) {
                        P.C = ( mem[byte] & 0x1 ) != 0;
                        mem[byte] = (mem[byte] >> 1)|(oldC << 7);
                    }
                    M_status_flags(mem[byte]);
                }
                break;
            case INS::ASL_ABS:
            case INS::LSR_ABS:
            case INS::ROL_ABS:
            case INS::ROR_ABS:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    uint16_t addr = (low|(high << 8));
                    uint8_t oldC = P.C;
                    cycles--; // get value from memory
                    cycles--; // bitshift left
                    cycles--; // set value to memory
                    if ( ins == INS::ASL_ABS ) {
                        P.C = ( mem[addr] & 0x80 ) != 0;
                        mem[addr] = mem[addr] << 1;
                    } else if ( ins == INS::LSR_ABS ) {
                        P.C = ( mem[addr] & 0x1 ) != 0;
                        mem[addr] = mem[addr] >> 1;
                    } else if ( ins == INS::ROL_ABS ) {
                        P.C = ( mem[addr] & 0x80 ) != 0;
                        mem[addr] = (mem[addr] << 1)|oldC;
                    } else if ( ins == INS::ROR_ABS ) {
                        P.C = ( mem[addr] & 0x1 ) != 0;
                        mem[addr] = (mem[addr] >> 1)|(oldC << 7);
                    }
                    M_status_flags(mem[addr]);
                }
                break;
            case INS::ASL_ABS_X:
            case INS::LSR_ABS_X:
            case INS::ROL_ABS_X:
            case INS::ROR_ABS_X:
                {
                    uint8_t low = readByte();
                    uint8_t high = readByte();
                    uint16_t addr = (low|(high << 8))+X;
                    uint8_t oldC = P.C;
                    cycles--; // add X to address
                    cycles--; // get value from memory
                    cycles--; // bitshift left
                    cycles--; // set value to memory
                    if ( ins == INS::ASL_ABS_X ) {
                        P.C = ( mem[addr] & 0x80 ) != 0;
                        mem[addr] = mem[addr] << 1;
                    } else if ( ins == INS::LSR_ABS_X ) {
                        P.C = ( mem[addr] & 0x1 ) != 0;
                        mem[addr] = mem[addr] >> 1;
                    } else if ( ins == INS::ROL_ABS_X ) {
                        P.C = ( mem[addr] & 0x80 ) != 0;
                        mem[addr] = (mem[addr] << 1)|oldC;
                    } else if ( ins == INS::ROR_ABS_X ) {
                        P.C = ( mem[addr] & 0x1 ) != 0;
                        mem[addr] = (mem[addr] >> 1)|(oldC << 7);
                    }
                    M_status_flags(mem[addr]);
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

