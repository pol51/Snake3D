//
//	texture.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifndef		__TEXTURE_H
#define		__TEXTURE_H


#include	<map>
#include	<string>
#include	<GL/gl.h>

#include	"singleton.h"



// ==============================================
// CTextureManager - OpenGL texture manager.
// ==============================================

class CTextureManager : public CSingleton<CTextureManager>
{
	friend class CSingleton<CTextureManager>;

private:
	// constructeur/destructeur
	CTextureManager( void ) { m_kDataBase.clear(); }
	~CTextureManager( void ) { ReleaseTextures(); }


public:
	// functions publiques
	void	Initialize( void );

	GLuint	LoadTexture( std::string szFilename );

	GLuint	GetTexture( std::string szFilename );
	char	*GetTextureName( GLuint id );

	void	ReleaseTextures( void );

	void	DeleteTexture( std::string szName );
	void	DeleteTexture( GLuint id );


private:
	// texture map<>
	std::map<std::string, GLuint>::iterator	m_kItor;
	std::map<std::string, GLuint>			m_kDataBase;

};


#endif	// __TEXTURE_H
