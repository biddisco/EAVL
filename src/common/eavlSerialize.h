// Copyright 2010-2013 UT-Battelle, LLC.  See LICENSE.txt for more information.
#ifndef EAVL_SERIALIZE_H
#define EAVL_SERIALIZE_H

#include "STL.h"
#include <string.h>

using namespace std;
class eavlStream : public basic_iostream<char,char_traits<char> >
{
public:
    eavlStream(ostream &os) : basic_iostream<char,char_traits<char> >(os.rdbuf()) {}
    eavlStream(istream &is) : basic_iostream<char,char_traits<char> >(is.rdbuf()) {}
};

template <class T>
inline eavlStream& operator<<(eavlStream &s, const T &v)
{
    s.write((const char*)&v, sizeof(T));
    cout<<"write ("<<sizeof(T)<<"): "<<v<<endl;
    return s;
}

template <class T>
inline eavlStream& operator<<(eavlStream &s, const vector<T> &a)
{
    size_t sz = a.size();
    s.write((const char*)&sz, sizeof(sz));
    if (sz > 0)
	s.write((const char*)&(a[0]), sz*sizeof(T));
    
    cout<<"write ("<<sz<<"*"<<sizeof(T)<<"): ";
    if (sz > 0) cout <<a[0]<<endl;
    else cout << "<null>"<<endl;
    return s;
}

inline eavlStream& operator<<(eavlStream &s, const char *str)
{
    size_t sz = strlen(str);
    s.write((const char *)&sz, sizeof(sz));
    s.write(str, sz);
    cout<<"write string: {"<<sz<<"} "<<str<<endl;
    return s;
}

inline eavlStream& operator<<(eavlStream &s, const string &str)
{
    size_t sz = str.length();
    s.write((const char *)&sz, sizeof(sz));
    s.write(&str[0], sz);
    cout<<"write string: ["<<sz<<"] "<<str<<endl;
    return s;
}

template <class T>
inline eavlStream& operator>>(eavlStream &s, T &v)
{
    s.read((char*)&v, sizeof(T));
    cout<<"read ("<<sizeof(T)<<"): "<<v<<endl;
    return s;
}

inline eavlStream& operator>>(eavlStream &s, string &str)
{
    size_t sz;
    s.read((char*)&sz, sizeof(sz));
    str.resize(sz);
    s.read(&str[0], sz);
    cout<<"read string: ["<<sz<<"] "<<str<<endl;
    return s;
}

template <class T>
inline eavlStream& operator>>(eavlStream &s, vector<T> &a)
{
    size_t sz;
    s.read((char*)&sz, sizeof(sz));
    a.resize(sz);
    if (sz > 0)
	s.read((char*)&(a[0]), sz*sizeof(T));
    
    cout<<"read ("<<sz<<"*"<<sizeof(T)<<"): ";
    if (sz > 0) cout <<a[0]<<endl;
    else cout << "<null>"<<endl;

    return s;
}



#if 0

template<class T>
inline ostream& operator<<(ostream &ar, const vector<int> &v)
{
    size_t sz = v.size();
    ar << sz;
    for (int i = 0; i < sz; i++)
	ar << v[i];
    return ar;
}

template<class T>
inline ostream& operator<<(ostream &ar, const vector<T> &v)
{
    typename vector<T>::const_iterator b = v.begin();
    typename vector<T>::const_iterator e = v.end();
    ar << v.size();
    while (b != e)
    {
	(*b).serialize(ar);
	std::advance(b,1);
    }
    return ar;
}

template<class T>
inline ostream& operator<<(ostream &ar, const vector<T*> &v)
{
    typename vector<T*>::const_iterator b = v.begin();
    typename vector<T*>::const_iterator e = v.end();
    ar << v.size();
    while (b != e)
    {
	(*b)->serialize(ar);
	std::advance(b,1);
    }
    return ar;
}
#endif

#endif
