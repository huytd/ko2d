
#include "TGA.h"
#include <stdio.h>

#pragma pack(push,x1)					// Byte alignment (8-bit)
#pragma pack(1)

typedef struct
{
    unsigned char  identsize;			// size of ID field that follows 18 byte header (0 usually)
    unsigned char  colourmaptype;		// type of colour map 0=none, 1=has palette
    unsigned char  imagetype;			// type of image 2=rgb uncompressed, 10 - rgb rle compressed

    short colourmapstart;				// first colour map entry in palette
    short colourmaplength;				// number of colours in palette
    unsigned char  colourmapbits;		// number of bits per palette entry 15,16,24,32

    short xstart;						// image x origin
    short ystart;						// image y origin
    short width;						// image width in pixels
    short height;						// image height in pixels
    unsigned char  bits;				// image bits per pixel 24,32
    unsigned char  descriptor;			// image descriptor bits (vh flip bits)

    // pixel data follows header

} TGA_HEADER;

#pragma pack(pop,x1)

const int IT_COMPRESSED = 10;
const int IT_UNCOMPRESSED = 2;

void LoadCompressedImage( char* pDest, char * pSrc, TGA_HEADER * pHeader )
{
    int w = pHeader->width;
    int h = pHeader->height;
    int rowSize = w * pHeader->bits / 8;
    bool bInverted = ( (pHeader->descriptor & (1 << 5)) == 0 );
    char * pDestPtr = bInverted ? pDest + (h + 1) * rowSize : pDest;
    int countPixels = 0;
    int nPixels = w * h;

    while( nPixels > countPixels )
    {
        unsigned char chunk = *pSrc ++;
        if ( chunk < 128 )
        {
            int chunkSize = chunk + 1;
            for ( int i = 0; i < chunkSize; i ++ )
            {
                if ( bInverted && (countPixels % w) == 0 )
                    pDestPtr -= 2 * rowSize;
                *pDestPtr ++ = pSrc[2];
                *pDestPtr ++ = pSrc[1];
                *pDestPtr ++ = pSrc[0];
                pSrc += 3;
                if ( pHeader->bits != 24 )
                    *pDestPtr ++ = *pSrc ++;
                countPixels ++;
            }
        }
        else
        {
            int chunkSize = chunk - 127;
            for ( int i = 0; i < chunkSize; i ++ )
            {
                if ( bInverted && (countPixels % w) == 0 )
                    pDestPtr -= 2 * rowSize;
                *pDestPtr ++ = pSrc[2];
                *pDestPtr ++ = pSrc[1];
                *pDestPtr ++ = pSrc[0];
                if ( pHeader->bits != 24 )
                    *pDestPtr ++ = pSrc[3];
                countPixels ++;
            }
            pSrc += (pHeader->bits >> 3);
        }
    }
}

void LoadUncompressedImage( char* pDest, char * pSrc, TGA_HEADER * pHeader )
{
    int w = pHeader->width;
    int h = pHeader->height;
    int rowSize = w * pHeader->bits / 8;
    bool bInverted = ( (pHeader->descriptor & (1 << 5)) == 0 );
    for ( int i = 0; i < h; i ++ )
    {
        char * pSrcRow = pSrc + 
            ( bInverted ? ( h - i - 1 ) * rowSize : i * rowSize );
        if ( pHeader->bits == 24 )
        {
            for ( int j = 0; j < w; j ++ )
            {
                *pDest ++ = pSrcRow[2];
                *pDest ++ = pSrcRow[1];
                *pDest ++ = pSrcRow[0];
                pSrcRow += 3;
            }
        }
        else
        {
            for ( int j = 0; j < w; j ++ )
            {
                *pDest ++ = pSrcRow[2];
                *pDest ++ = pSrcRow[1];
                *pDest ++ = pSrcRow[0];
                *pDest ++ = pSrcRow[3];
                pSrcRow += 4;
            }
        }
    }
}


char * LoadTGA( const char * szFileName, int * width, int * height, int * bpp )
{
    
    FILE* f = fopen(szFileName, "rb" );
	
	if (f == 0)
        return NULL;

    TGA_HEADER header;
    fread( &header, sizeof(header), 1, f );

    fseek( f, 0, SEEK_END );
    int fileLen = ftell( f );
    fseek( f, sizeof( header ) + header.identsize, SEEK_SET );

    if ( header.imagetype != IT_COMPRESSED && header.imagetype != IT_UNCOMPRESSED )
    {
        fclose( f );
        return NULL;
    }

    if ( header.bits != 24 && header.bits != 32 )
    {
        fclose( f );
        return NULL;
    }

    int bufferSize = fileLen - sizeof( header ) - header.identsize;
    char * pBuffer = new char[bufferSize];
    fread( pBuffer, 1, bufferSize, f );
    fclose( f );

    *width = header.width;
    *height = header.height;
    *bpp = header.bits;
    char * pOutBuffer = new char[ header.width * header.height * header.bits / 8 ];

    switch( header.imagetype )
    {
    case IT_UNCOMPRESSED:
        LoadUncompressedImage( pOutBuffer, pBuffer, &header );
        break;
    case IT_COMPRESSED:
        LoadCompressedImage( pOutBuffer, pBuffer, &header );
        break;
    }

    delete[] pBuffer;

    return pOutBuffer;
}
