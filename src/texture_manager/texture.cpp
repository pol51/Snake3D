//
//	texture.cpp - source file
//
//	David Henry - tfc_duke@club-internet.fr
//


#include	<iostream>
#include	<math.h>

#include	"texture.h"
#include	"bmp.h"



/////////////////////////////////////////////////
//				CTextureManager				   //
/////////////////////////////////////////////////


// ----------------------------------------------
// Initialize() - crée une texture d'échiquier
// pour la texture par défaut.
// ----------------------------------------------

void CTextureManager::Initialize( void )
{
	// vide la base de données
	ReleaseTextures();

	// c'est la première texture chargée en mémoire. Si
	// une texture ne peut être lue ou chargée, on utilise
	// celle si à la place.

#ifdef DEBUG
	std::cout << "CTextureManager : Texture list is empty, creating default texture..." << std::endl;
#endif
	GLuint id;

	// création et initialisation de la texture
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// ajout à la base de donnée
	m_kDataBase[ "default" ] = id;

#ifdef DEBUG
	std::cout << "CTextureManager : creating default texture, id [" << id << "]" << std::endl;
#endif

	// création d'une texture échiquier pour la texture par défaut
	int i, j, c;			// variables de contrôle
	GLubyte *checker;		// données texels

	checker = new GLubyte[ 64 * 64 * 4 ];

	// construction de l'échiquier
	for( i = 0; i < 64; i++ )
	{
		for( j = 0; j < 64; j++ )
		{
			c = ( !(i & 8) ^ !(j & 8)) * 255;

			checker[ (i * 256) + (j * 4) + 0 ] = (GLubyte)c;
			checker[ (i * 256) + (j * 4) + 1 ] = (GLubyte)c;
			checker[ (i * 256) + (j * 4) + 2 ] = (GLubyte)c;
			checker[ (i * 256) + (j * 4) + 3 ] = (GLubyte)255;
		}
	}

	glTexImage2D( GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker );
	delete [] checker;
}



// ----------------------------------------------
// LoadTexture() - charge une texture. Vérifie
// si elle n'est pas déjà stockée en mémoire. Si
// c'est le cas, la fonction retourne l'id de la
// texture déjà stockée. Sinon, elle retourne
// l'id de la nouvelle texture.
// ----------------------------------------------

GLuint CTextureManager::LoadTexture( std::string szFilename )
{
	GLuint		id = 0;			// id de la texture à retourner
	GLubyte		*pixels = 0;	// tableau de pixels de la texture
	int			width, height;	// dimensions de la texture


	// on tente d'obtenir la texture depuis la base de données.
	// Si elle a déjà été chargée au préalable, on retourne son
	// id, sinon l'id retournée est celle de la texture par défaut.
	// On tente alors de la charger dans la base.
	id = GetTexture( szFilename );

	if( id != GetTexture( "default" ) )
	{
		// la texture a déjà été chargée
		std::cout << "CTextureManager : " << szFilename << " is already stored, id [" << id << "]" << std::endl;
	}
	else
	{
		// on tente de charger la texture
		if( LoadFileBMP( szFilename.c_str(), &pixels, &width, &height, false ) > 0 )
		{
			// création et initialisation de la texture opengl
			glGenTextures( 1, &id );
			glBindTexture( GL_TEXTURE_2D, id );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

			// création d'une nouvelle texture et ajout à la base de données
			m_kDataBase[ szFilename ] = id;

#ifdef DEBUG
			std::cout << "CTextureManager : " << szFilename << " successfully loaded!" << std::endl;
			std::cout << " # creating new texture, id [" << id << "], (" << width<< " * " << height<< ")" << std::endl;
#endif
		}
		else
		{
			// impossible de charger la texture, texture par défault utilisée
#ifdef DEBUG
			std::cout << "CTextureManager : failed to load " << szFilename << "! giving default texture id [" << m_kDataBase[ "default" ] << "]" << std::endl;
#endif
		}

		// désallocation de mémoire - OpenGL possède sa propre copie de l'image
		if( pixels )
			delete [] pixels;
	}

	return id;
}



// ----------------------------------------------
// GetTexture() - retourne l'id de la texture
// filename si déjà chargée.
// ----------------------------------------------

GLuint CTextureManager::GetTexture( std::string szFilename )
{
	if( (m_kItor = m_kDataBase.find( szFilename )) != m_kDataBase.end() )
	{
		return (*m_kItor).second;
	}
	else
	{
		return m_kDataBase[ "default" ];
	}
}



// ----------------------------------------------
// GetTextureName() - retourne un pointeur sur le
// nom de l'image d'id spécifiée.
// ----------------------------------------------

char *CTextureManager::GetTextureName( GLuint id )
{
	if( glIsTexture( id ) )
	{
		for( m_kItor = m_kDataBase.begin(); m_kItor != m_kDataBase.end(); ++m_kItor )
		{
			if( (*m_kItor).second == id )
			{
				return (char *)((*m_kItor).first.c_str());
			}
		}
	}

	// la texture n'a pas été trouvée
	return (char *)"failed";
}



// ----------------------------------------------
// ReleaseTextures() - détruit toutes les textures
// stockées dans la base de données.
// ----------------------------------------------

void CTextureManager::ReleaseTextures( void )
{
#ifdef DEBUG
	std::cout << "CTextureManager : cleaning all textures!!! size = " << m_kDataBase.size();
#endif

	// on détruit chaque texture de la base de données du manager
	for( m_kItor = m_kDataBase.begin(); m_kItor != m_kDataBase.end(); )
	{
		glDeleteTextures( 1, &(*m_kItor).second );
		m_kDataBase.erase( m_kItor++ );
	}

#ifdef DEBUG
	std::cout << " // cleaned! size = " << m_kDataBase.size() << std::endl;
#endif
}



// ----------------------------------------------
// DeleteTexture() - détruit la texture szName.
// ----------------------------------------------

void CTextureManager::DeleteTexture( std::string szName )
{
	// on recherche l'id de la texture à détruire
	if( (m_kItor = m_kDataBase.find( szName )) != m_kDataBase.end() )
	{
#ifdef DEBUG
		std::cout << "CTextureManager : erasing " << szName << std::endl;
#endif
		
		// destruction de la texture et retrait de l'id
		// de la base de données du manager
		glDeleteTextures( 1, &(*m_kItor).second );
		m_kDataBase.erase( m_kItor );
	}
	else
	{
#ifdef DEBUG
		std::cout << "CTextureManager : couldn't erase " << szName << std::endl;
#endif
	}
}



// ----------------------------------------------
// DeleteTexture() - détruit la texture de l'id
// spécifiée.
// ----------------------------------------------

void CTextureManager::DeleteTexture( GLuint id )
{
	if( glIsTexture( id ) )
	{
		for( m_kItor = m_kDataBase.begin(); m_kItor != m_kDataBase.end(); ++m_kItor )
		{
			if( (*m_kItor).second == id )
			{
#ifdef DEBUG
				std::cout << "CTextureManager : erasing " << (*m_kItor).first<< " [" << (*m_kItor).second << "], size = " << m_kDataBase.size() << std::endl;
#endif

				// destruction de la texture et retrait de l'id
				// de la base de données du manager
				glDeleteTextures( 1, &(*m_kItor).second );
				m_kDataBase.erase( m_kItor );

				return;
			}
		}
	}
}
