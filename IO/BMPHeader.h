//
// Created by phoenixflower on 9/26/20.
//

#ifndef LANDESSDEVCORE_BMPHEADER_H
#define LANDESSDEVCORE_BMPHEADER_H
#include <stdint.h>
#include <array>
namespace LD
{
    struct BMPHeader
    {
        uint16_t signature;
        uint32_t size_bytes;
        uint32_t reserved;
        uint32_t data_offset;
    };

    enum class BMPCompression : uint32_t {
        None      = 0,
        RLE8      = 1,
        RLE4      = 2,
        Bitfields = 3,
        JPEG      = 4,
        PNG       = 5
    };

    // For simplicity, we only support v4 headers
    struct BMPSecondaryHeaderV4 {
        uint32_t header_size;

        uint32_t width, height;

        uint16_t num_planes;

        uint16_t bits_per_pixel;

        BMPCompression compression;

        // there's more data in this header, but we're not interested in it
        std::array<uint8_t, 0x58> unused;
    };

    namespace Detail
    {
        const int infoHeaderSize = 40;
        const int bytesPerPixel = 3; /// red, green, blue
        const int fileHeaderSize = 14;
    }
    unsigned char* createBitmapInfoHeader(int height, int width){
        static unsigned char infoHeader[] = {
                0,0,0,0, /// header size
                0,0,0,0, /// image width
                0,0,0,0, /// image height
                0,0, /// number of color planes
                0,0, /// bits per pixel
                0,0,0,0, /// compression
                0,0,0,0, /// image size
                0,0,0,0, /// horizontal resolution
                0,0,0,0, /// vertical resolutionReflectionExample
                0,0,0,0, /// colors in color table
                0,0,0,0, /// important color count
        };

        infoHeader[ 0] = (unsigned char)(LD::Detail::infoHeaderSize);
        infoHeader[ 4] = (unsigned char)(width    );
        infoHeader[ 5] = (unsigned char)(width>> 8);
        infoHeader[ 6] = (unsigned char)(width>>16);
        infoHeader[ 7] = (unsigned char)(width>>24);
        infoHeader[ 8] = (unsigned char)(height    );
        infoHeader[ 9] = (unsigned char)(height>> 8);
        infoHeader[10] = (unsigned char)(height>>16);
        infoHeader[11] = (unsigned char)(height>>24);
        infoHeader[12] = (unsigned char)(1);
        infoHeader[14] = (unsigned char)(LD::Detail::bytesPerPixel*8);

        return infoHeader;
    }

    unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
        int fileSize = LD::Detail::fileHeaderSize + LD::Detail::infoHeaderSize + (LD::Detail::bytesPerPixel*width+paddingSize) * height;

        static unsigned char fileHeader[] = {
                0,0, /// signature
                0,0,0,0, /// image file size in bytes
                0,0,0,0, /// reserved
                0,0,0,0, /// start of pixel array
        };

        fileHeader[ 0] = (unsigned char)('B');
        fileHeader[ 1] = (unsigned char)('M');
        fileHeader[ 2] = (unsigned char)(fileSize    );
        fileHeader[ 3] = (unsigned char)(fileSize>> 8);
        fileHeader[ 4] = (unsigned char)(fileSize>>16);
        fileHeader[ 5] = (unsigned char)(fileSize>>24);
        fileHeader[10] = (unsigned char)(LD::Detail::fileHeaderSize + LD::Detail::infoHeaderSize);

        return fileHeader;
    }

    void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName){

        unsigned char padding[3] = {0, 0, 0};
        int paddingSize = (4 - (width*LD::Detail::bytesPerPixel) % 4) % 4;

        unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
        unsigned char* infoHeader = createBitmapInfoHeader(height, width);

        FILE* imageFile = fopen(imageFileName, "wb");

        //fwrite(fileHeader, 1, fileHeaderSize, imageFile);
        //fwrite(infoHeader, 1, infoHeaderSize, imageFile);

        int i;
        for(i=0; i<height; i++){
            //fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
            //fwrite(padding, 1, paddingSize, imageFile);
        }

        fclose(imageFile);
    }

}
#endif //LANDESSDEVCORE_BMPHEADER_H
