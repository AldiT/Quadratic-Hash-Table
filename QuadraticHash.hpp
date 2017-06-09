

#ifndef QuadraticHash_hpp
#define QuadraticHash_hpp

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>


using namespace std;

int nextPrime( int n );

template <typename HashedObj>
class QuadraticHash
{
public:
    
    /*
     
     */
    
    
    explicit QuadraticHash( int size = 101 ) : array( nextPrime( size ) )
    {
        
        
        makeEmpty();
    }
    
    bool contains(const HashedObj & x) const
    {
        return isActive( findPos( x ) );
    }
    
    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }
    
    bool insert( const HashedObj & x )
    {
        
        
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;
        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;
        // Rehash
        if( ++currentSize > array.size( ) / 2 )
            rehash( );
        return true;
    }
    bool insert( HashedObj && x ){
        
        
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;
        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;
        // Rehash;
        if( ++currentSize > array.size( ) / 2 )
            rehash( );
        return true;
    }
    
    HashedObj getMin(){
        return min;
    }
    
    HashedObj getMax(){
        return max;
    }
    
    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ))
            return false;
        array[ currentPos ].info = DELETED;
        return true;
    }
    
    HashedObj getIn(int i){
        int in = findPos(i);
        
        if(array[i].info == ACTIVE)
            return array[i].element;
        
        
        return NULL;
    }
    
    int getSize(){
        return currentSize;
    }
    
    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;
        
        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY ): element{ e }, info{ i } { }
        HashEntry( HashedObj && e, EntryType i = EMPTY ) : element{ std::move( e ) }, info{ i}{}
    };
    
    vector<HashEntry> array;
    int currentSize;
    
    bool isActive( int currentPos ) const
    {
        return array[ currentPos ].info == ACTIVE;
    }
    
    int findPos( const HashedObj & x ) const
    {
        int offset = 1;
        int currentPos = (unsigned) myhash( x );
        while( array[ currentPos ].info != EMPTY &&
              array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }
        return currentPos;
    }
    
    void rehash( )
    {
        vector<HashEntry> oldArray = array;
        // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;
        // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }
    
    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
};

#endif /* QuadraticHash_hpp */
