#pragma once
#include "Common.h"
#include <map>
#include <stdlib.h>

namespace Utils{
    template<class KeyType>
    inline unsigned int hashFunction(const KeyType& key){
        return 0u;
    }

    template <class KeyVlaue,class ValueType>
    class Hashmap{
    public: 
        typename std::map<unsigned int,ValueType> KeyVlaueMap;
    private:
        KeyVlaueMap **m_hashMap;
        unsigned int m_bucketCount;
        unsigned int m_moduloValue;

    protected:
        FORCE_INLINE void init(){
            m_hashMap = new KeyVlaueMap*[m_bucketCount];
            for (unsigned int i = 0; i < m_bucketCount; ++i)
            {
                m_hashMap[i] = NULL;
            }
            
            
        }
    };
}