#include <cstdint>
#include <iostream>

class Cache
{
public:
	Cache(const uint32_t bytesByline, const uint32_t nlines)
		:  _BytesBylines(bytesByline),       // bytes por linea
		   _nLines(nlines),					 // numero de lines
		   _limitCache(nlines * bytesByline) // limite de cache antes de reemplazo LRU
	{}

	~Cache() = default;

	void setVector(const size_t num) 
	{
		_iterations = num; 
	}

	void simulateLoop(const uint32_t init, const uint32_t incrm)
	{
		_increment = incrm; // guardamos el incremento para calculos		

		/* cache Loop simulation */		

		for (uint32_t i = init; i < _iterations; i += incrm)
		{
			if (i == 0) _bytesCount += sizeof(i);
			else _bytesCount += sizeof(i) * incrm;	

			if (_bytesCount >= 128 || i == 0) // pasamos de los 128 B o iniciamos primera carga en cache
			{
				++_miss;					  // tenemos un miss

				if (i > 0) _bytesCount = 0;	  // reiniciamos contador de Bytes

				_limitCache -= 128;			  // cada 128 B nos vamos acercando al limite
				_elements   += 32;			  // 32 elementos del vector cacheados

				if (_limitCache == 0)		  // se ha llenado la cache
				{
					_LRUcount++;	  // llamada algoritmo LRU, debe reemplazar una linea
					_limitCache += 128;		  // liberamos una linea
											  // tenemos nuevo limite en la cache
				}
			}
			else ++_hit;						
		}
	}

	void showResults()
	{
		const float tasaAciertos = static_cast<float>(_hit) / (static_cast<float>(_iterations) / static_cast<float>(_increment));
		const float tasaFallos = 1.0f - tasaAciertos;

		std::cout << "\n ------------  Results  ------------ \n\n";
		std::cout << "   Tasa Aciertos       : " << tasaAciertos << "\n";
		std::cout << "   Tasa Fallos         : " << tasaFallos << "\n\n";
		std::cout << "   Algoritmo LRU       : " << _LRUcount << " reemplazos" << "\n\n";
		std::cout << "   Elementos cacheados : " << _elements << "\n";
		std::cout << "   Bytes cacheados     : " << _elements * sizeof(uint32_t) << "\n\n";
		std::cout << "   Hit                 : " << _hit << "\n";
		std::cout << "   Miss                : " << _miss << "\n";
	}

private:
	uint32_t _bytesCount{ 0 }, _BytesBylines{ 0 }, _nLines{ 0 }, _limitCache{ 0 };
	uint32_t _iterations{ 0 }, _increment{ 0 }, _elements{ 0 }, _LRUcount{ 0 };
	uint32_t _miss{ 0 }, _hit{ 0 };
};

int main()
{
	Cache cache(128, 512);	  // tamaño y numero de lineas

	cache.setVector(524288);  // numero elementos del vector

	cache.simulateLoop(0, 5); // inicia en 0 con incrementos de 2

	cache.showResults();	  // mostrar resultados

	return 0;
}
