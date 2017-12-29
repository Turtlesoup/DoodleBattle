#ifndef ID_TO_ELELEMT_H
#define ID_TO_ELELEMT_H

template <class T>
class IdToElement
{
	public:
		IdToElement(unsigned int id, T element);
		~IdToElement();
		unsigned int getID();
		T& getElement();
	private:
		unsigned int _id;
		T _element;
};



template <class T>
IdToElement<T>::IdToElement(unsigned int id, T element)
{
	_id = id;
	_element = element;
}

template <class T>
IdToElement<T>::~IdToElement()
{

}

template <class T>
unsigned int IdToElement<T>::getID()
{
	return _id;
}

template <class T>
T& IdToElement<T>::getElement()
{
	return _element;
}


#endif