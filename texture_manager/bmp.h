//
//	bmp.h - header file / freeware
//
//	David Henry - tfc_duke@hotmail.com
//


#ifndef		__BMP_H_
#define		__BMP_H_

// identifiant "BM"
#define			BITMAP_ID			('B' + ('M'<<8))

// status premier/second octet pour compression RLE
#define			RLE_COMMAND			0
#define			RLE_ENDOFLINE		0
#define			RLE_ENDOFBITMAP		1
#define			RLE_DELTA			2

#define			BI_OS2				-1


#ifdef WIN32
#include	<windows.h>
#else


// type de compression
#define			BI_RGB				0
#define			BI_RLE8				1
#define			BI_RLE4				2
#define			BI_BITFIELDS		3


// --------------------------------------------
// BITMAPFILEHEADER - bitmap file header.
// --------------------------------------------

#pragma pack(2)

typedef struct tagBITMAPFILEHEADER		// bmfh
{
	unsigned short	bfType;				// numéro magique "BM"
	unsigned int	bfSize;				// taille du fichier
	unsigned short	bfReserved1;		// réservé
	unsigned short	bfReserved2;		// réservé
	unsigned int	bfOffBits;			// offset pour les données bitmap

} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

#pragma pack(4)



// --------------------------------------------
// BITMAPINFOHEADER - bitmap info header.
// --------------------------------------------

typedef struct
{
	unsigned int	biSize;				// taille de la structure
	int				biWidth;			// largeur de l'image
	int				biHeight;			// hauteur de l'mage
	unsigned short	biPlanes;			// doit être égal à 1
	unsigned short	biBitCount;			// nombre de bits par pixels
	unsigned int	biCompression;		// type de compression
	unsigned int	biSizeImage;		// taille des données pixels
	int				biXPelsPerMeter;	// nombre de pixels par mètre sur l'axe des x
	int				biYPelsPerMeter;	// nombre de pixels par mètre sur l'axe des y
	unsigned int	biClrUsed;			// nombre de couleur utilisées par le bitmap
	unsigned int	biClrImportant;		// nombre de couleurs importantes

} BITMAPINFOHEADER;



// --------------------------------------------
// BITMAPCOREHEADER - bitmap core header.
// --------------------------------------------

typedef struct tagBITMAPCOREHEADER		// bmch
{
	unsigned int	bcSize;				// taille de la structure
	unsigned short	bcWidth;			// doit être égal à 1
	unsigned short	bcHeight;			// nombre de bits par pixels
	unsigned short	bcPlanes;			// doit être égal à 1
	unsigned short	bcBitCount;			// nombre de bits par pixels

} BITMAPCOREHEADER, *PBITMAPCOREHEADER;



// --------------------------------------------
// RGBTRIPLE - pixel 24 bits.
// --------------------------------------------

typedef struct tagRGBTRIPLE			// rgbt
{
	unsigned char	rgbtBlue;
	unsigned char	rgbtGreen;
	unsigned char	rgbtRed;

} RGBTRIPLE, *PRGBTRIPLE;



// --------------------------------------------
// RGBQUAD - pixel 32 bits.
// --------------------------------------------

typedef struct tagRGBQUAD			// rgbt
{
	unsigned char	rgbBlue;
	unsigned char	rgbGreen;
	unsigned char	rgbRed;
	unsigned char	rgbReserved;

} RGBQUAD, *PRGBQUAD;



// --------------------------------------------
// BITMAPCOREINFO - bitmap core info.
// --------------------------------------------

typedef struct tagBITMAPCOREINFO		// bmci
{
	BITMAPCOREHEADER	bmciHeader;		// taille de la structure
	RGBTRIPLE			bcmiColors[1];	// doit être égal à 1

} BITMAPCOREINFO, *PBITMAPCOREINFO;


#endif	// WIN32



// chargement fichier bmp
int LoadFileBMP( const char *filename, unsigned char **texels, int *width, int *height, bool flipvert );


#endif // __BMP_H_
