//
//	bmp.cpp - source file / freeware
//
//	David Henry - tfc_duke@hotmail.com
//


#include	<fstream>
#include	"bmp.h"

using namespace std;


// --------------------------------------------------
// LoadFileBMP() - charge une image Windows/OS2
//				   BITMAP [.bmp].
//
// paramètres :
//    - filename [in]  : nom de fichier
//    - pixels	 [out] : données image rgba 32 bits
//    - width    [out] : largeur en pixels
//    - height   [out] : hauteur en pixels
//    - flipvert [in]  : flag retournement vertical
//
// valeur de retour :
//    - -1 : pas de données pixels
//    -  0 : échec
//    -  1 : succès
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// formats acceptés :
//     # RGB 1-4-8-24-32 bits	WINDOWS - OS/2
//     # RLE 4-8 bits			WINDOWS
// --------------------------------------------------

int LoadFileBMP( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert )
{
	ifstream			file;			// fichier
	BITMAPFILEHEADER	*bmfh;			// bitmap file header
	BITMAPINFOHEADER	*bmih;			// bitmap info header (windows)
	BITMAPCOREHEADER	*bmch;			// bitmap core header (os/2)
	RGBTRIPLE			*os2_palette;	// pointeur palette couleurs os/2
	RGBQUAD				*win_palette;	// pointeur palette couleurs windows
	char				*buffer;		// tampon mémoire stockant les données du fichier
	unsigned char		*ptr;			// pointeur données pixels
	int					bitCount;		// nombre de bit par pixel
	int					compression;	// type de compression (rgb/rle)
	int					row, col, i;	// variables de contrôle



	/////////////////////////////////////////////////////
	// lecture de l'intégralité du  fichier

	file.open( filename, ios::in | ios::binary );

	if( file.fail() )
		return 0;

	file.seekg( 0, ios::end );
	long flen = file.tellg();
	file.seekg( 0, ios::beg );

	buffer = new char[ flen + 1 ];
	file.read( buffer, flen );
	char *pBuff = buffer;

	file.close();
	file.clear();

	/////////////////////////////////////////////////////


	// lecture du header
	bmfh = (BITMAPFILEHEADER *)pBuff;
	pBuff += sizeof( BITMAPFILEHEADER );

	// on vérifie qu'il s'agit bien d'un BITMAP
	if( bmfh->bfType != BITMAP_ID )
	{
		delete [] buffer;
		return 0;
	}


	bmch = (BITMAPCOREHEADER *)pBuff;
	bmih = (BITMAPINFOHEADER *)pBuff;


	if( (bmih->biCompression < 0) || (bmih->biCompression > 3) )
	{
		// OS/2 style
		pBuff += sizeof( BITMAPCOREHEADER );

		bitCount	= bmch->bcBitCount;
		compression	= BI_OS2;

		if( width )
			*width	= bmch->bcWidth;

		if( height )
			*height	= bmch->bcHeight;
	}
	else
	{
		// WINDOWS style
		pBuff += sizeof( BITMAPINFOHEADER );

		bitCount	= bmih->biBitCount;
		compression	= bmih->biCompression;

		if( width )
			*width	= bmih->biWidth;

		if( height )
			*height	= bmih->biHeight;
	}


	if( !pixels )
	{
		delete [] buffer;
		return (-1);
	}


	/////////////////////////////////////////////////////
	// lecture de la palette

	if( bitCount <= 8 )
	{
		// les images 24 et 32 bits ne possèdent pas de palettes.

		// on ajuste le pointeur de la palette sur la zone mémoire
		// correspondante dans buffer
		os2_palette = (RGBTRIPLE *)pBuff;
		win_palette = (RGBQUAD *)pBuff;

		//	[nombre de couleurs palette] * [taille d'un pixel]
		pBuff += (1 << bitCount) * (bitCount >> 3) * sizeof( unsigned char );
	}

	/////////////////////////////////////////////////////


	// allocation de mémoire pour les données pixels de l'image
	*pixels = new unsigned char[ (*width) * (*height) * 4 ];
	ptr		= &(*pixels)[0];


	// on déplace le pointeur fichier au début des données pixels
	pBuff = buffer + (bmfh->bfOffBits * sizeof( char ));


	/////////////////////////////////////////////////////
	// lecture des données suivant le type de
	// compression et le nombre de bits par pixel
	/////////////////////////////////////////////////////

	switch( compression )
	{
		case BI_OS2:
		case BI_RGB:
		{
			for( row = (*height) - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * (*width) * 4 ];

				switch( bitCount )
				{
					case 1:
					{
// RGB 1 BITS
						for( col = 0; col < (int)((*width) / 8); col++ )
						{
							// lecture du pixel en cours
							unsigned char color = *((unsigned char *)(pBuff++));

							for( i = 7; i >= 0; i--, ptr += 4 )
							{
								// conversion pixel index couleur bgr 1 bits
								// en pixel rgba 32 bits
								int clrIdx = ((color & (1<<i)) > 0);

								if( compression == BI_OS2 )
								{
									ptr[0] = os2_palette[ clrIdx ].rgbtRed;
									ptr[1] = os2_palette[ clrIdx ].rgbtGreen;
									ptr[2] = os2_palette[ clrIdx ].rgbtBlue;
									ptr[3] = 255;
								}
								else
								{
									ptr[0] = win_palette[ clrIdx ].rgbRed;
									ptr[1] = win_palette[ clrIdx ].rgbGreen;
									ptr[2] = win_palette[ clrIdx ].rgbBlue;
									ptr[3] = 255;
								}
							}
						}

						break;
					}

					case 4:
					{
// RGB 4 BITS
						for( col = 0; col < (int)((*width) / 2); col++, ptr += 8 )
						{
							// lecture du pixel en cours
							unsigned char color = *((unsigned char *)(pBuff++));

							// conversion pixel index couleur bgr 4 bits
							// en pixel rgba 32 bits
							int clrIdx;

							if( compression == BI_OS2 )
							{
								clrIdx = (color >> 4);
								ptr[0] = os2_palette[ clrIdx ].rgbtRed;
								ptr[1] = os2_palette[ clrIdx ].rgbtGreen;
								ptr[2] = os2_palette[ clrIdx ].rgbtBlue;
								ptr[3] = 255;

								clrIdx = (color & 0x0F);
								ptr[4] = os2_palette[ clrIdx ].rgbtRed;
								ptr[5] = os2_palette[ clrIdx ].rgbtGreen;
								ptr[6] = os2_palette[ clrIdx ].rgbtBlue;
								ptr[7] = 255;
							}
							else
							{
								clrIdx = (color >> 4);
								ptr[0] = win_palette[ clrIdx ].rgbRed;
								ptr[1] = win_palette[ clrIdx ].rgbGreen;
								ptr[2] = win_palette[ clrIdx ].rgbBlue;
								ptr[3] = 255;

								clrIdx = (color & 0x0F);
								ptr[4] = win_palette[ clrIdx ].rgbRed;
								ptr[5] = win_palette[ clrIdx ].rgbGreen;
								ptr[6] = win_palette[ clrIdx ].rgbBlue;
								ptr[7] = 255;
							}
						}

						break;
					}

					case 8:
					{
// RGB 8 BITS
						for( col = 0; col < (*width); col++, ptr += 4 )
						{
							// lecture du pixel en cours
							unsigned char color = *((unsigned char *)(pBuff++));

							// conversion pixel index couleur bgr 8 bits
							// en pixel rgba 32 bits
							if( compression == BI_OS2 )
							{
								ptr[0] = os2_palette[ color ].rgbtRed;
								ptr[1] = os2_palette[ color ].rgbtGreen;
								ptr[2] = os2_palette[ color ].rgbtBlue;
								ptr[3] = 255;
							}
							else
							{
								ptr[0] = win_palette[ color ].rgbRed;
								ptr[1] = win_palette[ color ].rgbGreen;
								ptr[2] = win_palette[ color ].rgbBlue;
								ptr[3] = 255;
							}
						}

						break;
					}

					case 24:
					{
// RGB 24 BITS
						for( col = 0; col < (*width); col++, ptr += 4 )
						{
							// conversion pixel bgr 24 bits en pixel rgba 32 bits
							RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
							pBuff += sizeof( RGBTRIPLE );

							ptr[0] = pix->rgbtRed;
							ptr[1] = pix->rgbtGreen;
							ptr[2] = pix->rgbtBlue;
							ptr[3] = 255;
						}

  						break;
					}

					case 32:
					{
// RGB 32 BITS
						for( col = 0; col < (*width); col++, ptr += 4 )
						{
							// conversion pixel bgr 32 bits en pixel rgba 32 bits
							RGBQUAD *pix = (RGBQUAD *)(pBuff += sizeof( RGBQUAD ));
							pBuff += sizeof( RGBQUAD );

							ptr[0] = pix->rgbRed;
							ptr[1] = pix->rgbGreen;
							ptr[2] = pix->rgbBlue;
							ptr[3] = 255;
						}

						break;
					}
				}
			}

			break;
		}

		case BI_RLE8:
		{
// RLE 8 BITS
			for( row = (*height) - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * (*width) * 4 ];

				for( col = 0; col < (*width); /* rien */ )
				{
					// on récupère une unité de code (sur 2 octets)
					unsigned char byte1 = *((unsigned char *)(pBuff++));
					unsigned char byte2 = *((unsigned char *)(pBuff++));


					if( byte1 == RLE_COMMAND )
					{
						// encodage absolu
						for( i = 0; i < byte2; i++, ptr += 4, col++ )
						{
							// lecture du pixel en cours
							unsigned char color = *((unsigned char *)(pBuff++));

							// conversion pixel index couleur bgr 8 bits
							// en pixel rgba 32 bits
							ptr[0] = win_palette[ color ].rgbRed;
							ptr[1] = win_palette[ color ].rgbGreen;
							ptr[2] = win_palette[ color ].rgbBlue;
							ptr[3] = 255;
						}

						// on avance d'un octet si l'on s'est arrêté
						// sur un nombre impair d'octets décodés
						if( (byte2 % 2) == 1 )
							pBuff++;
					}
					else
					{
						// on decode pour les byte1 pixels suivants
						for( i = 0; i < byte1; i++, ptr += 4, col++ )
						{
							// conversion pixel index couleur bgr 8 bits
							// en pixel rgba 32 bits
							ptr[0] = win_palette[ byte2 ].rgbRed;
							ptr[1] = win_palette[ byte2 ].rgbGreen;
							ptr[2] = win_palette[ byte2 ].rgbBlue;
							ptr[3] = 255;
						}        
					}
				}
			}

			break;
		}

		case BI_RLE4:
		{
// RLE 4 BITS
			unsigned char color;
			int	bytesRead = 0;		// nombre d'octets lus du fichier

			for( row = (*height) - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * (*width) * 4 ];

				for( col = 0; col < (*width); /* rien */ )
				{
					// on récupère une unité de code (sur 2 octets)
					unsigned char byte1 = *((unsigned char *)(pBuff++));
					unsigned char byte2 = *((unsigned char *)(pBuff++));

					bytesRead += 2;
      

					if( byte1 == RLE_COMMAND )
					{
						// encodage absolu
						unsigned char databyte;

						for( i = 0; i < byte2; i++, ptr += 4, col++ )
						{
							if( (i % 2) == 0 )
							{
								// lecture du pixel en cours
								databyte = *((unsigned char *)(pBuff++));
								bytesRead++;

								color = (databyte >> 4);	// 4 premiers bits
							}
							else
							{
								color = (databyte & 0x0F);	// 4 derniers bits
							}

							// conversion pixel index couleur bgr 4 bits
							// en pixel rgba 32 bits
							ptr[0] = win_palette[ color ].rgbRed;
							ptr[1] = win_palette[ color ].rgbGreen;
							ptr[2] = win_palette[ color ].rgbBlue;
							ptr[3] = 255;
						}
          
						// on avance d'un octet si l'on s'est arrêté
						// sur un nombre impair d'octets décodés
						while( (bytesRead % 2) != 0 )
						{
							pBuff++;
							bytesRead++;
						}
					}
					else
					{
						// on decode pour les byte1 pixels suivants
						for( i = 0; i < byte1; i++, ptr += 4, col++ )
						{
							if( (i % 2) == 0 )
								color = (byte2 >> 4);	// 4 premiers bits
							else
								color = (byte2 & 0x0F);	// 4 derniers bits

							// conversion pixel index couleur bgr 4 bits
							// en pixel rgba 32 bits
							ptr[0] = win_palette[ color ].rgbRed;
							ptr[1] = win_palette[ color ].rgbGreen;
							ptr[2] = win_palette[ color ].rgbBlue;
							ptr[3] = 255;
						}        
					}
				}
			}

			break;
		}
	}


	// désallocation mémoire tampon
	delete [] buffer;

	// retour avec succès
	return 1;
}
