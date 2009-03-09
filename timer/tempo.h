#include <time.h>

class Tempo
{
private:
	clock_t start;
public:
	inline Tempo() { start = clock(); }
	inline void reset() { start = clock(); }
	inline double time() const 
    { return (1000. * (clock() - start)  / CLOCKS_PER_SEC); }
};
