const int Size = 100;

template <class T>
class Array
{ 
    T *elements;
    int Asize;
public:
    Array(int size = Size);
    ~Array(){ delete []elements; }
    T& operator[] (int i);
};

template<class T>
Array<T> :: Array(int size)
{
    elements = new T(size);
    for(int i = 0; i < size; i++){
        elements[i] = i;
    }
    Asize = size;
}

template<class T>
T& Array<T>::operator[](int i)
{
    return elements[i];
}