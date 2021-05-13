#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>

int main( int argc, char* argv[] )
{
    if ( argc != 2 ) {
        printf("Usage: nesparser [file]\n");
        return 1;
    }
    std::ifstream ifs(argv[1], std::ios_base::in |std::ios_base::binary);
    // FILE *fp = fopen(argv[1], "rb");
    if ( ifs ) {
        // uint8_t header[16];
        std::vector<uint8_t> header;
        std::vector<uint8_t> prgrom;
        // fread(&header[0],1,16,fp);
        header.resize(0x10);
        if (!ifs.read(reinterpret_cast<char*>(&header[0]), 0x10)) {
            printf("ERROR\n");
        }
        for ( int i = 0; i < 0x10; i++ ) {
            printf("%x: %x\n",i,header[i]);
        }

        uint8_t banks = header[4];
        uint8_t vbanks = header[5];
        uint8_t tablemirror = header[6] & 0xB;
        uint8_t mapper = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);

        int prgromsize = 1024*16*banks;
        prgrom.resize(prgromsize);
        if (!ifs.read(reinterpret_cast<char*>(&prgrom[0]), prgromsize)) {
            printf("Error reading PRG-ROM\n");
        }
        printf("0xFFFE: %.2x %.2x( at %d )\n",prgrom[prgromsize-2],prgrom[prgromsize-1],prgromsize);
        std::cout << "16KB PRG-ROM Banks: " << +banks << std::endl;
        std::cout << "8KB CHR-ROM Banks: " << +vbanks << std::endl;
        std::cout << "Name Table Mirroring: " << +tablemirror << std::endl;
        std::cout << "Mapper #: " << +mapper << std::endl;

        // fclose(fp);
    } else {
        return 1;
    }
    return 0;
}
