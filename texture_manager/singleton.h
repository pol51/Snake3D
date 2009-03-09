//
//	singleton.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifndef		__SINGLETON_H
#define		__SINGLETON_H



// ==============================================
// CSingleton - singleton.
// ==============================================

template <typename T> class CSingleton
{
protected:
	// constructeur/destructeur
	CSingleton( void ) { }
	~CSingleton( void ) { }


public:
	// fonctions publiques
	static T *GetInstance( void )
	{
		if( m_Singleton == 0 )
			m_Singleton = new T;

		return ((T *)m_Singleton);
	}

	static void Kill( void )
	{
		if( m_Singleton != 0 )
		{
			delete m_Singleton;
			m_Singleton = 0;
		}
	}


protected:
	// variables membres privées
	static T	*m_Singleton;

};


template <typename T> T *CSingleton<T>::m_Singleton = 0;


#endif	// __SINGLETON_H
