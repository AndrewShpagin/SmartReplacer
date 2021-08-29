#pragma once

//--------------------------------------------------------------------------------------------------------------|
// Speed comparison between "cList<>::Ctor / cList<>" (sec)														|
//--------------------------------------------------------------------------------------------------------------|
// "100" times allocate "500000" elements and free them															|
//            | cStr					| int					| float					| cVec3					|
// Debug 32   | 11.76 / 2.06 (x 5.7)	| 0.18 / 0.18 (x 1.0)	| 0.18 / 0.18 (x 1.0)	| 2.44 / 0.70 (x 3.5)	|
// Release 32 | 2.06 / 0.01 (x 298.3)	| 0.00 / 0.00 (x 1.1)	| 0.00 / 0.00 (x 1.0)	| 0.00 / 0.00 (x 1.0)	|
// Debug 64   | 5.59 / 2.57 (x 2.2)		| 0.19 / 0.19 (x 1.0)	| 0.19 / 0.19 (x 1.0)	| 0.94 / 0.69 (x 1.4)	|
// Release 64 | 2.62 / 0.01 (x 311.0)	| 0.00 / 0.00 (x 1.1)	| 0.00 / 0.00 (x 1.0)	| 0.00 / 0.00 (x 1.0)	|
//--------------------------------------------------------------------------------------------------------------|
// "500000" times allocate "100" elements and free them															|
//            | cStr					| int					| float					| cVec3					|
// Debug 32   | 19.64 / 6.93 (x 2.8)	| 1.70 / 1.12 (x 1.5)	| 1.70 / 1.09 (x 1.6)	| 4.98 / 2.10 (x 2.4)	|
// Release 32 | 3.41 / 2.10 (x 1.6)		| 1.13 / 0.74 (x 1.5)	| 1.84 / 1.45 (x 1.3)	| 3.36 / 2.93 (x 1.1)	|
// Debug 64   | 7.58 / 3.23 (x 2.3)		| 2.23 / 1.70 (x 1.3)	| 2.37 / 1.74 (x 1.4)	| 5.11 / 4.23 (x 1.2)	|
// Release 64 | 5.07 / 4.18 (x 1.2)		| 0.86 / 0.59 (x 1.5)	| 0.86 / 0.59 (x 1.5)	| 3.60 / 3.33 (x 1.1)	|
//--------------------------------------------------------------------------------------------------------------|

//*****************************************************************************
// cList_DefCompare
//*****************************************************************************
template<class TYPE>
inline int cList_DefCompare(const TYPE *l, const TYPE *r) {
	if(*l > *r) {
		return 1;
	} else if(*l < *r) {
		return -1;
	} else {
		return 0;
	}
}

//*****************************************************************************
// cList_DefEquals
//*****************************************************************************
template<class TYPE>
inline bool cList_DefEquals(const TYPE *l, const TYPE *r) {
	return *l == *r;
}

// cList_Debug
struct cList_Debug {
	static int ContainersCount;
	static int CtorContainersCount;
};

#ifdef COMMS_LINUX
#if (__GNUC__ < 5)
// Type properties are missing under GCC 4.8.2:
// https://gcc.gnu.org/onlinedocs/gcc-4.8.2/libstdc++/manual/manual/status.html (20.9.4.3)
namespace std {
template<class T>
bool is_trivially_default_constructible() {
	return false;
}
template<class T>
bool is_trivially_copy_constructible() {
	return false;
}
template<class T>
bool is_compound() {
	return true;
}
} // std
#endif // Before GCC 5
#endif // Linux

//*****************************************************************************
// cList_Container
//*****************************************************************************
template<class TYPE>
class cList_Container { // most common container
public:
	int Count, Capacity;
	TYPE List[1024];
	static cList_Container * New(const int Capacity) {
		cList_Debug::ContainersCount++;
		size_t Size = 8 + Capacity * sizeof(TYPE);
		byte *Ptr = new byte[Size];
		if (!std::is_trivially_default_constructible<TYPE>()){
			for (int k = 0; k < Capacity; k++)new (Ptr + 8 + k*sizeof(TYPE)) TYPE();
		}
		return (cList_Container *)Ptr;
	}
	static void Free(cList_Container *Ptr) {
		cList_Debug::ContainersCount--;
		delete[] ((byte *)Ptr);
	}
	void Copy(cList_Container* to, int Count) const {
		if (std::is_trivially_copy_constructible<TYPE>()){
			memcpy(((byte*)to) + 8, List, Count*sizeof(TYPE));
		}
		else{
			for (int i = 0; i < Count; i++){
				to->List[i] = List[i];
			}
		}
	}
	void DestroyElements(){
		if (std::is_compound<TYPE>()){
			for (int i = 0; i < Capacity; i++){
				List[i].~TYPE();
			}
		}
	}
}; // cList_Container

//*****************************************************************************
// cList_TrivialContainer
//*****************************************************************************
template<class TYPE>
class cList_TrivialContainer { // Doesn't call constructors/destructors in the lists like "cList<cStr>"
public:
	int Count, Capacity;
	TYPE List[1024];
	static cList_TrivialContainer * New(const int Capacity) {
		cList_Debug::ContainersCount++;
		size_t Size = 8 + Capacity * sizeof(TYPE);
		byte *Ptr = new byte[Size];
		return (cList_TrivialContainer *)Ptr;
	}
	static void Free(cList_TrivialContainer *Ptr) {
		cList_Debug::ContainersCount--;
		delete[]((byte *)Ptr);
	}
	void Copy(cList_TrivialContainer* to, int Count) const {
		memcpy(((byte*)to) + 8, List, Count*sizeof(TYPE));
	}
	void DestroyElements(){
	}
}; // cList_Container

//*****************************************************************************
// cList
//*****************************************************************************
template <class TYPE, class CONTAINER = cList_Container<TYPE> >
class cList {
public:
    // you can use it for agile grab the types in the code
    // # By analogy with the STL-containers.
    typedef TYPE       value_type;
    typedef CONTAINER  container_type;

	typedef cList<TYPE, cList_Container<TYPE> > Ctor; // C++ doesn't support template typedefs. Use for type "cList<cStr>::Ctor".

	typedef int Compare(const TYPE *, const TYPE *);

	struct CompareFunctor {
		virtual ~CompareFunctor() {}
		virtual bool operator()( const TYPE& a, const TYPE& b ) const = 0;
	};

	typedef bool Equals(const TYPE *, const TYPE *);
	
	//-------------------------------------------------------------------------
	// .ctor
	//-------------------------------------------------------------------------
	cList();
	cList(const cList<TYPE, CONTAINER> &Src);
	cList(const TYPE *Src, const int Count);		// cList<int> U(I, 100);
	explicit cList(const int Count);				// cList<int> T(20);
	cList(const int Count, const TYPE &Fill);		// cList<int> Z(20, 0);
	
	// .dtor
	~cList();
	
	size_t Size() const;			// Total size of used for elements memory (bytes)
	size_t SizeCapacity() const;	// Total size of allocated memory (bytes)

	// Assignment operators
	void Copy(const cList<TYPE, CONTAINER> &Src);
	void Copy(const TYPE *Src, const int Count);
    // @copy DynArray::MoveArrayTo()
    void Move( cList<TYPE, CONTAINER>& Dest );
    // @copy DynArray::Move()
    bool Move( int from, int to );
    void Set(cList<TYPE, CONTAINER> *Src);
	void operator = (const cList<TYPE, CONTAINER> &Src);
	void Swap(cList<TYPE, CONTAINER> *With);

	// Access operators
	const TYPE & operator [] (const int Index) const;
	TYPE & operator [] (const int Index);
	const TYPE & GetAt(const int Index) const;
	TYPE & GetAt(const int Index);
	const TYPE & GetFirst() const;
	TYPE & GetFirst();
	const TYPE & GetLast() const;
	TYPE & GetLast();
	void SetAt(const int Index, const TYPE &Value);
	const TYPE * ToPtr() const; // Returns a const pointer to the list
	TYPE * ToPtr(); // Returns a mutable pointer to the list
	void GetRange(const int Index, const int Count, cList<TYPE, CONTAINER> *To) const;
	///Unlimited get - get value at index Index, if beyoud range - return defvalue
	TYPE uGet(const int Index, const TYPE& defvalue);
	///Unlimited set - set value at index Index, if beyoud range - add correcsponding count of defvalue-s
	void uSet(const int Index, const TYPE& value, const TYPE& defvalue);

    // @copy DynArray::operator+( int )
    // @todo Remove it?
    TYPE* operator + ( int Index ) {
        cAssert( Index >= 0 && Index < Count() && !IsEmpty() );
        return m_Container->List + Index;
    }

	void Free(); // Removes all items from the list and frees all the space
	void FreeUnused(); // Resizes the list to exactly the number of elements it contains
	void FreeContents(); // Calls the destructor of all elements in the list

    // Fast version of Resize().
    // @copy "SetCapacityMod" == DynArray::CleanupAllocation()
    // @see pool.cpp
    template< int mod = 8 >
	void SetCapacityMod();

	int Count() const; // Gets the number of elements actually contained in the "cList"

    // @todo fine  Remove after change DynArray<> to cList<>.
    int GetAmount() const { return Count(); }

	void SetCount(const int Count); // Sets the number of elements actually contained in the "cList"
	// Sets the number of elements actually contained in the "cList" and
	// initializes every element with specified value "Fill"
	void SetCount(int Count, const TYPE &Value);

	// Gets or sets the number of elements that the "cList" can contain
	int Capacity() const;
	void SetCapacity(const int Capacity);

	bool IsEmpty() const;
	void Clear();
	void Fill(const TYPE &Value);

	// Adds "Count" elements to the end of the list. Returns the index of the last added element.
	int Add(const TYPE &Value, const int Count = 1);

    // @todo fine  Copied from class DynArray<>. Remove or rename it.
    void AddValues( const TYPE* Values, int N ) {
        AddRange( Values, N );
    }

	void AddRange(const cList<TYPE, CONTAINER> &Src); // Adds the elements of another "cList" object to the end of current
	void AddRange(const TYPE *Src, const int Count); // Adds "Count" elements of another list to the end of current

	// Inserts element to the list at the specified position with specified number of times
	void Insert(const int Index, const TYPE &Value, const int Count = 1);

	// Inserts the elements of another "cList" object into current at the specified position
	void InsertRange(const int Index, const cList<TYPE, CONTAINER> &Src);

	// Insert the elements of another list of specified length at the specified position
	void InsertRange(const int Index, const TYPE *Src, const int Count);

    TYPE& ExpandTo( int Index, const TYPE& );

    // @copy DynArray::Putp()
    // @return true when inserted.
    bool InsertFirstOrRemove( const TYPE& );

	// Removes the first occurence of a specific object from the "cList" object.
	// Returns "true" if value was found; otherwise, false.
	bool Remove(const TYPE &Value, Equals *E = (Equals *)&cList_DefEquals);

	// cList<int> L;
	// ...
	// while(L.Remove(10, ZeroResidue)) {
	// }
	
	// Removes a range of elements from the "cList" object with
    // specified number of elements to remove
	void RemoveAt(const int Index, const int Count = 1);
	
	void RemoveLast();
	
	// Searches for the specified object and returns the zero - based index of the first occurrence
	// within the entire "cList" object using the specified equals function
	int IndexOf(const TYPE &Value, Equals *E = (Equals *)&cList_DefEquals) const;
	
	// cList<cStr> L;
	// ...
	// int i = L.IndexOf("Tilda", cStr::EqualsNoCase);

	// Using the specified equals function searches for the specified object and returns the zero - based
	// index of the first occurrence within the range of elements in the "cList" object that
	// extends from the specified index to the last element
	int IndexOf(const TYPE &Value, const int StartIndex, Equals *E = (Equals *)&cList_DefEquals) const;

	// Searches for the specified "Value" and returns the zero - based index of the first occurrence
	// within the range of elements in the "cList" object that starts at the specified index and
	// contains the specified number of elements
	int IndexOf(const TYPE &Value, const int StartIndex, const int Count, Equals *E = (Equals *)&cList_DefEquals) const;

	// Searches for the specified "Value" and returns the zero - based index of the last occurrence
	// within the entire "cList" object
	int LastIndexOf(const TYPE &Value, Equals *E = (Equals *)&cList_DefEquals) const;

	// Searches for the specified "Value" and returns the zero - based index of the last occurrence
	// within the range of elements in the "cList" object that extends from the first element
	// to the specified index
	int LastIndexOf(const TYPE &Value, const int StartIndex, Equals *E = (Equals *)&cList_DefEquals) const;

	// Searches for the specified "Value" and returns the zero - based index of the last occurrence
	// within the range of elements in the "cList" object the contains the specified number of elements
	// and ends at the specified index
	int LastIndexOf(const TYPE &Value, const int StartIndex, const int Count, Equals *E = (Equals *)&cList_DefEquals) const;
	
	// Searches the entire sorted "cList" object for an element using the specified compare function and returns
	// the zero - based index, if "Value" is found; otherwise, -1
	int BinarySearch(const TYPE &Value, Compare *C = (Compare *)&cList_DefCompare) const;

	// cList<cStr> L;
	// ...
	// int i = L.BinarySearch("Over", cStr::CompareNoCase);

	// Searches the range of elements in the sorted "cList" object for an element using the specified
	// compare function and returns the zero - based index, if "Value" is found; otherwise, -1
	int BinarySearch(const int Index, const int Count, const TYPE &Value, Compare *C = (Compare *)&cList_DefCompare) const;

	// Determines whether an element is in the "cList" object using the specified equals function
	bool Contains(const TYPE &Value, Equals *E = (Equals *)&cList_DefEquals) const;

    // @todo fine  Copied from class DynArray<>. Change to cList::Contains().
    int find( const TYPE& Value ) const {
        for ( int i = 0; i < Count(); i++ ) {
            if ( !memcmp( &Value, &m_Container->List[ i ], sizeof( TYPE ) ) ) {
                return i;
            }
        }
        return -1;
    }

    // @todo fine  Copied from the class DynArray<>. Remove or rename it.
    TYPE pop_front() {
        const TYPE  e = GetFirst();
        RemoveAt( 0, 1 );
        return e;
    }

    // @todo fine  Copied from the class DynArray<>. Remove or rename it.
    TYPE pop_back() {
        const TYPE  e = GetLast();
        RemoveLast();
        return e;
    }

	// Determines using "IndexOf" function whether an element is in the "cList" object, and only if
	// it is not in the list, adds it. Returns the index of founded or added element.
	int AddUnique(const TYPE &Value, Equals *E = (Equals *)&cList_DefEquals);
	
    // @return true when added
    bool AddOnce( const TYPE&, Equals *E = ( Equals * )&cList_DefEquals );

	// cList<cStr> L;
	// ...
	// bool s = L.Contains("Over", cStr::EqualsNoCase);
	
	void Reverse();									// Reverses the order of the elements in the entire "cList" object
	void Reverse(const int Index, const int Count);	// Reverses the order of the elements in the specified range
	
	// Sorts the elements in the entire "cList" object using the specified compare function
	void Sort(Compare *C = (Compare *)&cList_DefCompare);
	void Sort(const CompareFunctor&);

	// Sorts the elements in a range of elements in "cList" object using the specified compare function
	void Sort(const int Index, const int Count, Compare *C = (Compare *)&cList_DefCompare);
	void Sort(int Index, int Count, const CompareFunctor& );
	void EnsureCapacity(int size);

	void operator += (const TYPE &);
	void operator -= (const TYPE &);
	void operator *= (const TYPE &);
	void operator /= (const TYPE &);

	bool operator == (const cList<TYPE, CONTAINER> &) const;
	bool operator != (const cList<TYPE, CONTAINER> &) const;

	// cList<cVec3> L;
	// ...
	// L += cVec3(10, 20, 0);
	// L *= cVec3(20.0f);

protected:
	friend class cXml;
	void Null();
	CONTAINER *m_Container;

	CONTAINER * EnsureCapacity(const int Capacity, const bool SkipDelete);
	CONTAINER * Resize(const int Capacity, const bool SkipDelete);
};

//the difference from cList is that destuctors are not called during deallocation of the list
//this array is most efficient of all in terms of memory consumption and speed of allocation
template <class TYPE>
class cArray : public cList<TYPE, cList_TrivialContainer<TYPE> >{

};

// cList<TYPE, CONTAINER>::Null
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Null() {
	m_Container = NULL;
}


// cList<TYPE, CONTAINER>.ctor : ()
template<class TYPE, class CONTAINER>
inline cList<TYPE, CONTAINER>::cList() {
	Null();
}

// cList<TYPE, CONTAINER>.ctor : (const cList<TYPE, CONTAINER> &)
template<class TYPE, class CONTAINER>
inline cList<TYPE, CONTAINER>::cList(const cList<TYPE, CONTAINER> &Src) {
	Null();
	Copy(Src);
}

// cList<TYPE, CONTAINER>.ctor : (const TYPE *, const int)
template<class TYPE, class CONTAINER>
inline cList<TYPE, CONTAINER>::cList(const TYPE *Src, const int Count) {
	Null();
	Copy(Src, Count);
}

// cList<TYPE, CONTAINER>.ctor : (const int)
template<class TYPE, class CONTAINER>
inline cList<TYPE, CONTAINER>::cList(const int Count) {
	Null();
	SetCount(Count);
}

// cList<TYPE, CONTAINER>.ctor : (const int, const TYPE &)
template<class TYPE, class CONTAINER>
inline cList<TYPE, CONTAINER>::cList(const int Count, const TYPE &Fill) {
	Null();
	SetCount(Count, Fill);
}

// cList<TYPE, CONTAINER>.dtor
template<class TYPE, class CONTAINER>
inline cList<TYPE, CONTAINER>::~cList() {
	Free();
}

// cList<TYPE, CONTAINER>::Size
template<class TYPE, class CONTAINER>
inline size_t cList<TYPE, CONTAINER>::Size() const {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	return CurCount * sizeof(TYPE);
}

// cList<TYPE, CONTAINER>::SizeCapacity
template<class TYPE, class CONTAINER>
inline size_t cList<TYPE, CONTAINER>::SizeCapacity() const {
	int CurCapacity = (NULL == m_Container) ? 0 : m_Container->Capacity;
	return CurCapacity * sizeof(TYPE);
}

// cList<TYPE, CONTAINER>::Copy : (const cList<TYPE, CONTAINER> &)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Copy(const cList<TYPE, CONTAINER> &Src) {
	int i;
	int SrcCount = (NULL == Src.m_Container) ? 0 : Src.m_Container->Count;
	EnsureCapacity(SrcCount, false);
	if(m_Container != NULL) {
		m_Container->Count = SrcCount;
		for(i = 0; i < m_Container->Count; i++) {
			m_Container->List[i] = Src.m_Container->List[i];
		}
	}
}

// cList<TYPE, CONTAINER>::Copy : (const TYPE *, const int)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Copy(const TYPE *Src, const int Count) {
	int i;

	CONTAINER *OldContainer = EnsureCapacity(Count, true);
	if(m_Container != NULL) {
		m_Container->Count = Count;
		for(i = 0; i < m_Container->Count; i++) {
			m_Container->List[i] = Src[i];
		}
	}
	if(OldContainer != NULL) {
		if (!std::is_trivially_copy_constructible<TYPE>()){
			OldContainer->DestroyElements();
		}
		CONTAINER::Free(OldContainer);
	}
}

// cList<TYPE, CONTAINER>::Move : (cList<TYPE, CONTAINER> &)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Move( cList<TYPE, CONTAINER>& Dest ) {
    Dest.Clear();
    Dest.m_Container = m_Container;
    m_Container = nullptr;
}

// cList<TYPE, CONTAINER>::Move : (int, int)
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::Move( int from, int to ){
    const int  n = Count();
    if ( from >= n ) from = n - 1; else if ( from < 0 ) from = 0;
    if ( to >= n ) to = n - 1; else if ( to < 0 ) to = 0;
    if ( from == to ) return false;
    const TYPE  t = m_Container->List[ from ];
    if ( from < to ) {
        memcpy(
            m_Container->List + from,
            m_Container->List + from + 1,
            (to - from) * sizeof( TYPE ) );
    } else {
        memmove(
            m_Container->List + to + 1,
            m_Container->List + to,
            (from - to) * sizeof( TYPE ) );
    }
    m_Container->List[ to ] = t;
    return true;
}

// cList<TYPE, CONTAINER>::Set : (cList<TYPE, CONTAINER> *)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Set(cList<TYPE, CONTAINER> *Src) {
	cAssert((NULL == m_Container) || (m_Container != Src->m_Container));
	Free();

	m_Container = Src->m_Container;
	Src->m_Container = NULL;
}

// cList<TYPE, CONTAINER>::operator =
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::operator = (const cList<TYPE, CONTAINER> &Src) {
	Copy(Src);
}

// cList<TYPE, CONTAINER>::Swap : (cList<TYPE, CONTAINER> *)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Swap(cList<TYPE, CONTAINER> *With) {
	cMath::Swap(m_Container, With->m_Container);
}

// cList<TYPE, CONTAINER>::operator [] const
template<class TYPE, class CONTAINER>
inline const TYPE & cList<TYPE, CONTAINER>::operator [] (const int Index) const {
	cAssert(Index >= 0);
	cAssert(m_Container != NULL);
	cAssert(Index < m_Container->Count);
	return m_Container->List[Index];
}

// cList<TYPE, CONTAINER>::operator []
template<class TYPE, class CONTAINER>
inline TYPE & cList<TYPE, CONTAINER>::operator [] (const int Index) {
	cAssert(Index >= 0);
	cAssert(m_Container != NULL);
	cAssert(Index < m_Container->Count);
	return m_Container->List[Index];
}

// cList<TYPE, CONTAINER>::GetAt const
template<class TYPE, class CONTAINER>
inline const TYPE & cList<TYPE, CONTAINER>::GetAt(const int Index) const {
	cAssert(Index >= 0);
	cAssert(m_Container != NULL);
	cAssert(Index < m_Container->Count);
	return m_Container->List[Index];
}

// cList<TYPE, CONTAINER>::GetAt
template<class TYPE, class CONTAINER>
inline TYPE & cList<TYPE, CONTAINER>::GetAt(const int Index) {
	cAssert(Index >= 0);
	cAssert(m_Container != NULL);
	cAssert(Index < m_Container->Count);
	return m_Container->List[Index];
}

// cList<TYPE, CONTAINER>::GetFirst const
template<class TYPE, class CONTAINER>
inline const TYPE & cList<TYPE, CONTAINER>::GetFirst() const {
	cAssert(m_Container != NULL);
	cAssert(m_Container->Count >= 1);
	return m_Container->List[0];
}

// cList<TYPE, CONTAINER>::GetFirst
template<class TYPE, class CONTAINER>
inline TYPE & cList<TYPE, CONTAINER>::GetFirst() {
	cAssert(m_Container != NULL);
	cAssert(m_Container->Count >= 1);
	return m_Container->List[0];
}

// cList<TYPE, CONTAINER>::GetLast const
template<class TYPE, class CONTAINER>
inline const TYPE & cList<TYPE, CONTAINER>::GetLast() const {
	cAssert(m_Container != NULL);
	cAssert(m_Container->Count >= 1);
	return m_Container->List[m_Container->Count - 1];
}

// cList<TYPE, CONTAINER>::GetLast
template<class TYPE, class CONTAINER>
inline TYPE & cList<TYPE, CONTAINER>::GetLast() {
	cAssert(m_Container != NULL);
	cAssert(m_Container->Count >= 1);
	return m_Container->List[m_Container->Count - 1];
}

// cList<TYPE, CONTAINER>::SetAt
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::SetAt(const int Index, const TYPE &Value) {
	cAssert(Index >= 0);
	cAssert(m_Container != NULL);
	cAssert(Index < m_Container->Count);
	m_Container->List[Index] = Value;
}

// cList<TYPE, CONTAINER>::ToPtr() const
template<class TYPE, class CONTAINER>
inline const TYPE * cList<TYPE, CONTAINER>::ToPtr() const {
	return (NULL == m_Container) ? NULL : m_Container->List;
}

// cList<TYPE, CONTAINER>::ToPtr
template<class TYPE, class CONTAINER>
inline TYPE * cList<TYPE, CONTAINER>::ToPtr() {
	return (NULL == m_Container) ? NULL : m_Container->List;
}

//------------------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::GetRange
//------------------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::GetRange(const int Index, const int Count, cList<TYPE, CONTAINER> *To) const {
	cAssert(Index >= 0);
	cAssert(Count >= 0);
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	cAssert(Index + Count <= CurCount);
	
	int i;

	To->Clear();

	for(i = Index; i < Index + Count; i++) {
		To->Add(m_Container->List[i]);
	}
} // cList<TYPE, CONTAINER>::GetRange

// cList<TYPE, CONTAINER>::Free
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Free() {
	if(m_Container != NULL) {
		m_Container->DestroyElements();
		CONTAINER::Free(m_Container);
		m_Container = NULL;
	}
}

template<class TYPE, class CONTAINER>
TYPE cList<TYPE, CONTAINER>::uGet(const int Index, const TYPE& defvalue){
	if (Index >= 0 && Index < Count())return (*this)[Index];
	return defvalue;
}

template<class TYPE, class CONTAINER>
void cList<TYPE, CONTAINER>::uSet(const int Index, const TYPE& value, const TYPE& defvalue){
	if (Index >= 0){
		int n = Count();
		if (Index >= n)Add(defvalue, Index - n + 1);
		(*this)[Index] = value;
	}
}

// cList<TYPE, CONTAINER>::FreeContents
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::FreeContents() {
	if(m_Container != NULL) {
		int i;
		for(i = 0; i < m_Container->Count; i++) {
			delete m_Container->List[i];
			m_Container->List[i] = NULL;
		}
	}
}

// cList<TYPE, CONTAINER>::FreeUnused
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::FreeUnused() {
	if(m_Container != NULL) {
		Resize(m_Container->Count, false);
	}
}

// cList<TYPE, CONTAINER>::Clear
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Clear() {
	if(m_Container != NULL) {
		m_Container->Count = 0;
	}
}

// cList<TYPE, CONTAINER>::SetCapacityMod
template<class TYPE, class CONTAINER>
template< int mod >
inline void cList<TYPE, CONTAINER>::SetCapacityMod() {
    static_assert( mod % 8 == 0,
        "`mod` must be an integral of multiple of 8" );
    const int ncap = Count() * mod / 8;
    SetCapacity( ncap );
}

// cList<TYPE, CONTAINER>::Count
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::Count() const {
	return (NULL == m_Container) ? 0 : m_Container->Count;
}

// cList<TYPE, CONTAINER>::IsEmpty
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::IsEmpty() const {
	return (NULL == m_Container) || (m_Container->Count < 1);
}

// cList<TYPE, CONTAINER>::Capacity
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::Capacity() const {
	return (NULL == m_Container) ? 0 : m_Container->Capacity;
}

// cList<TYPE, CONTAINER>::SetCapacity
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::SetCapacity(const int Capacity) {
	Resize(Capacity, false);
}

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Add
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::Add(const TYPE &Value, const int Count) {
	cAssert(Count >= 0);
	int i;
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	CONTAINER *OldContainer = EnsureCapacity(CurCount + Count, true);
	for(i = 0; i < Count; i++) {
		m_Container->List[m_Container->Count] = Value;
		m_Container->Count++;
	}
	if(OldContainer != NULL) {
		if (!std::is_trivially_copy_constructible<TYPE>()){
			OldContainer->DestroyElements();
		}
		CONTAINER::Free(OldContainer);
	}
	return ((NULL == m_Container) ? 0 : m_Container->Count) - 1;
} // cList<TYPE, CONTAINER>::Add

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::AddRange : (const cList<TYPE, CONTAINER> &)
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::AddRange(const cList<TYPE, CONTAINER> &Src) {
	int i;
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	int SrcCount = (NULL == Src.m_Container) ? 0 : Src.m_Container->Count;
	EnsureCapacity(CurCount + SrcCount, false);
	for(i = 0; i < SrcCount; i++) {
		m_Container->List[m_Container->Count] = Src.m_Container->List[i];
		m_Container->Count++;
	}
} // cList<TYPE, CONTAINER>::AddRange

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::AddRange : (const TYPE *, const int)
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::AddRange(const TYPE *Src, const int Count) {
	cAssert(Count >= 0);
	int i;
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	CONTAINER *OldContainer = EnsureCapacity(CurCount + Count, true);
	for(i = 0; i < Count; i++) {
		m_Container->List[m_Container->Count] = Src[i];
		m_Container->Count++;
	}
	if(OldContainer != NULL) {
		if (!std::is_trivially_copy_constructible<TYPE>()){
			OldContainer->DestroyElements();
		}
		CONTAINER::Free(OldContainer);
	}
} // cList<TYPE, CONTAINER>::AddRange

//-------------------------------------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::IndexOf
//-------------------------------------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::IndexOf(const TYPE &Value, const int StartIndex, const int Count, Equals *E) const {
	cAssert(StartIndex >= 0);
	cAssert(Count >= 0);
	cAssert(StartIndex + Count <= ((NULL == m_Container) ? 0 : m_Container->Count));
	
	int i;
	
	for(i = StartIndex; i < StartIndex + Count; i++) {
		if(E(&m_Container->List[i], &Value)) {
			return i;
		}
	}
	
	return -1;
}

template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::IndexOf(const TYPE &Value, const int StartIndex, Equals *E) const {
	return IndexOf(Value, StartIndex, Count() - StartIndex, E);
}

template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::IndexOf(const TYPE &Value, Equals *E) const {
	return IndexOf(Value, 0, Count(), E);
}
// cList<TYPE, CONTAINER>::IndexOf

//--------------------------------------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::LastIndexOf
//--------------------------------------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::LastIndexOf(const TYPE &Value, Equals *E) const {
	return LastIndexOf(Value, Count() - 1, Count(), E);
}

template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::LastIndexOf(const TYPE &Value, const int StartIndex, Equals *E) const {
	return LastIndexOf(Value, StartIndex, StartIndex + 1, E);
}

template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::LastIndexOf(const TYPE &Value, const int StartIndex, const int Count, Equals *E) const {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	if(0 == CurCount) {
		cAssert(StartIndex >= -1);
		cAssert(StartIndex <= 0);
		cAssert(Count >= 0);
		return -1;
	}
	
	cAssert(StartIndex >= 0);
	cAssert(StartIndex < CurCount);
	cAssert(Count >= 0);
	cAssert(Count <= StartIndex + 1);
	
	int i, i0;
	
	i0 = StartIndex - Count + 1;
	for(i = StartIndex; i >= i0; i--) {
		if(E(&m_Container->List[i], &Value)) {
			return i;
		}
	}

	return -1;
} // cList<TYPE, CONTAINER>::LastIndexOf

//------------------------------------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::BinarySearch
//------------------------------------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::BinarySearch(const int Index, const int Count, const TYPE &Value, Compare *C) const {
	cAssert(Index >= 0);
	cAssert(Count >= 0);
	cAssert(Index + Count <= ((NULL == m_Container) ? 0 : m_Container->Count));

	int l, r, m;
	
	if(Count > 0) {
		l = 0;
		r = Index + Count - 1;
		while(l < r) {
			m = (l + r) / 2;
			if(C(&Value, &GetAt(m)) <= 0) {
				r = m;
			} else {
				l = m + 1;
			}
		}
		if(C(&Value, &GetAt(l)) == 0) {
			return l;
		}
	}
	
	return -1;
}

template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::BinarySearch(const TYPE &Value, Compare *C) const {
	return BinarySearch(0, Count(), Value, C);
}
// cList<TYPE, CONTAINER>::BinarySearch

// cList<TYPE, CONTAINER>::Contains
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::Contains(const TYPE &Value, Equals *E) const {
	int i;
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	for(i = 0; i < CurCount; i++) {
		if(E(&m_Container->List[i], &Value)) {
			return true;
		}
	}
	return false;
}

// cList<TYPE, CONTAINER>::AddUnique
template<class TYPE, class CONTAINER>
inline int cList<TYPE, CONTAINER>::AddUnique(const TYPE &Value, Equals *E) {
	int i = IndexOf(Value, E);
	if(-1 == i) {
		i = Add(Value);
	}
	return i;
}

// cList<TYPE, CONTAINER>::AddOnce
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::AddOnce( const TYPE& Value, Equals* E ){
    const int i = IndexOf( Value, E );
    if ( i == -1 ) {
        Add( Value );
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::RemoveAt
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::RemoveAt(const int Index, const int Count) {
	cAssert(Index >= 0);
	cAssert(Count >= 0);
	cAssert(Index + Count <= ((NULL == m_Container) ? 0 : m_Container->Count));
	int i;
	if(m_Container != NULL) {
		int lcount = Count;
		if (Index + lcount > m_Container->Count){
			lcount = m_Container->Count - Index;
		}
		m_Container->Count -= lcount;
		for(i = Index; i < m_Container->Count; i++) {
			m_Container->List[i] = m_Container->List[i + lcount];
		}
	}
} // cList<TYPE, CONTAINER>::RemoveAt

// cList<TYPE, CONTAINER>::RemoveLast
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::RemoveLast() {
	cAssert(m_Container != NULL);
	cAssert(m_Container->Count >= 1);
	m_Container->Count--;
}

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Remove
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::Remove(const TYPE &Value, Equals *E) {
	int Index = IndexOf(Value, E);
	if(Index >= 0) {
		RemoveAt(Index);
		return true;
	}
	return false;
} // cList<TYPE, CONTAINER>::Remove

//-----------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Insert
//-----------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Insert(const int Index, const TYPE &Value, const int Count) {
	cAssert(Index >= 0);
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	cAssert(Index <= CurCount);
	cAssert(Count >= 0);
	
	int i;

	CONTAINER *OldContainer = EnsureCapacity(CurCount + Count, true);
	for(i = CurCount - 1 + Count; i >= Index + Count; i--) {
		m_Container->List[i] = m_Container->List[i - Count];
	}
	if(m_Container != NULL) {
		m_Container->Count += Count;
		cAssert(Index + Count <= m_Container->Count);
	}
	for(i = 0; i < Count; i++) {
		m_Container->List[Index + i] = Value;
	}
	if(OldContainer != NULL) {
		if (!std::is_trivially_copy_constructible<TYPE>()){
			OldContainer->DestroyElements();
		}
		CONTAINER::Free(OldContainer);
	}
} // cList<TYPE, CONTAINER>::Insert

//---------------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::InsertRange : void (const int, const cList<TYPE, CONTAINER> &)
//---------------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::InsertRange(const int Index, const cList<TYPE, CONTAINER> &Src) {
	cAssert(Index >= 0);
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	cAssert(Index <= CurCount);
	int SrcCount = (NULL == Src.m_Container) ? 0 : Src.m_Container->Count;
	
	int i;

	EnsureCapacity(CurCount + SrcCount, false);
	for(i = CurCount - 1 + SrcCount; i >= Index + SrcCount; i--) {
		m_Container->List[i] = m_Container->List[i - SrcCount];
	}
	if(m_Container != NULL) {
		m_Container->Count += SrcCount;
		cAssert(Index + SrcCount <= m_Container->Count);
	}
	for(i = 0; i < SrcCount; i++) {
		m_Container->List[Index + i] = Src.m_Container->List[i];
	}
} // cList<TYPE, CONTAINER>::InsertRange

//--------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::InsertRange : void (const int, const TYPE *, const int)
//--------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::InsertRange(const int Index, const TYPE *Src, const int Count) {
	cAssert(Index >= 0);
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	cAssert(Index <= CurCount);
	cAssert(Count >= 0);

	int i;
	
	CONTAINER *OldContainer = EnsureCapacity(CurCount + Count, true);
	for(i = CurCount - 1 + Count; i >= Index + Count; i--) {
		m_Container->List[i] = m_Container->List[i - Count];
	}
	if(m_Container != NULL) {
		m_Container->Count += Count;
		cAssert(Index + Count <= m_Container->Count);
	}
	for(i = 0; i < Count; i++) {
		m_Container->List[Index + i] = Src[i];
	}
	if(OldContainer != NULL) {
		if (!std::is_trivially_copy_constructible<TYPE>()){
			OldContainer->DestroyElements();
		}
		CONTAINER::Free(OldContainer);
	}
} // cList<TYPE, CONTAINER>::InsertRange


//--------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::ExpandTo
//--------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline TYPE& cList<TYPE, CONTAINER>::ExpandTo( int Index, const TYPE& Value ) {
    if ( Index >= Count() ) {
        Add( Value, Index - Count() + 1 );
    }
    return (*this)[ Index ];
}

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Putp
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::InsertFirstOrRemove( const TYPE& Value ) {
    const int  i = find( Value );
    (i == -1) ? Insert( 0, Value ) : RemoveAt( i, 1 );
    return (i == -1);
}

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Reverse
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Reverse(const int Index, const int Count) {
	cAssert(Index >= 0);
	cAssert(Count >= 0);
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	cAssert(Index + Count <= CurCount);

	int i, h;

	if(Count > 1) {
		h = Count / 2;
		for(i = 0; i < h; i++) {
			cMath::Swap(m_Container->List[Index + i], m_Container->List[Index + Count - 1 - i]);
		}
	}
}

template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Reverse() {
	if(m_Container != NULL) {
		Reverse(0, m_Container->Count);
	}
}
// cList<TYPE, CONTAINER>::Reverse

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Resize
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline CONTAINER * cList<TYPE, CONTAINER>::Resize(const int Capacity, const bool SkipDelete) {
	cAssert(Capacity >= 0);
		
	if(Capacity <= 0) {
		Free();
		return NULL;
	}

	int OldCapacity = (NULL == m_Container) ? 0 : m_Container->Capacity;
	if(Capacity == OldCapacity) {
		return NULL;
	}

	CONTAINER *OldContainer = m_Container;
	m_Container = CONTAINER::New(Capacity);
	m_Container->Count = (NULL == OldContainer) ? 0 : OldContainer->Count;
	m_Container->Capacity = Capacity;

	if(Capacity < m_Container->Count) {
		m_Container->Count = Capacity;
	}
	
	if(OldContainer != NULL) {
		OldContainer->Copy(m_Container, m_Container->Count);
		if(SkipDelete) {
			return OldContainer;
		} else {
			if (!std::is_trivially_copy_constructible<TYPE>()){
				OldContainer->DestroyElements();
			}
			CONTAINER::Free(OldContainer);
		}
	}
	return NULL;
} // cList<TYPE, CONTAINER>::Resize

//------------------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::EnsureCapacity
//------------------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline CONTAINER * cList<TYPE, CONTAINER>::EnsureCapacity(const int Capacity, const bool SkipDelete) {
	int OldCapacity = (NULL == m_Container) ? 0 : m_Container->Capacity;
	if(Capacity > OldCapacity) {
		int S = Capacity + OldCapacity / 2;
		if (S < 8)S = 8;
		return Resize(S, SkipDelete);
	}
	return NULL;
} // cList<TYPE, CONTAINER>::EnsureCapacity

template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::EnsureCapacity(int size){
	EnsureCapacity(size, false);

}
// cList<TYPE, CONTAINER>::SetCount : void (const int)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::SetCount(const int Count) {
	cAssert(Count >= 0);
	EnsureCapacity(Count, false);
	if(m_Container != NULL) {
		m_Container->Count = Count;
	}
}

// cList<TYPE, CONTAINER>::Fill
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Fill(const TYPE &Value) {
	int i;
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	for(i = 0; i < CurCount; i++) {
		m_Container->List[i] = Value;
	}
}

// cList<TYPE, CONTAINER>::SetCount : void (const int, const TYPE &)
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::SetCount(const int Count, const TYPE &Value) {
	SetCount(Count);
	Fill(Value);
}

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Sort : (..., Compare *)
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Sort(const int Index, const int Count, Compare *C) {
	cAssert(Index >= 0);
	cAssert(Count >= 0);
	cAssert(Index + Count <= ((NULL == m_Container) ? 0 : m_Container->Count));
	
	int i = 0, j = 0, M = 0;
	bool f = false;
	TYPE K;
	
	M = Count / 2;
	while(M >= 1) {
		for(i = M; i < Count; i++) {
			K = GetAt(i + Index);
			j = i - M;
			f = false;
			while(j >= 0 && !f) {
				if(C(&K, &GetAt(j + Index)) < 0) {
					SetAt(j + M + Index, GetAt(j + Index));
					j -= M;
				} else {
					f = true;
				}
			}
			SetAt(j + M + Index, K);
		}
		M /= 2;
	}
} // cList<TYPE, CONTAINER>::Sort : (..., Compare *)

//-----------------------------------------------------------------------------
// cList<TYPE, CONTAINER>::Sort : (..., const CompareFunctor &)
//-----------------------------------------------------------------------------
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::Sort(const int Index, const int Count, const CompareFunctor &C) {
	cAssert(Index >= 0);
	cAssert(Count >= 0);
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	cAssert(Index + Count <= CurCount);
	
	int i = 0, j = 0, M = 0;
	bool f = false;
	TYPE K;
	
	M = Count / 2;
	while(M >= 1) {
		for(i = M; i < Count; i++) {
			K = GetAt(i + Index);
			j = i - M;
			f = false;
			while(j >= 0 && !f) {
				if(C(K, GetAt(j + Index))) {
					f = true;
				} else {
					SetAt(j + M + Index, GetAt(j + Index));
					j -= M;
				}
			}
			SetAt(j + M + Index, K);
		}
		M /= 2;
	}
} // cList<TYPE, CONTAINER>::Sort : (..., const CompareFunctor &)

// cList<TYPE, CONTAINER>::Sort
template<class TYPE, class CONTAINER>
void cList<TYPE, CONTAINER>::Sort(Compare *C) {
	Sort(0, Count(), C);
}
// cList<TYPE, CONTAINER>::Sort
template<class TYPE, class CONTAINER>
void cList<TYPE, CONTAINER>::Sort(const CompareFunctor &C) {
	Sort(0, Count(), C);
}

// cList<TYPE, CONTAINER>::operator +=
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::operator += (const TYPE &u) {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	for(int i = 0; i < CurCount; i++) {
		m_Container->List[i] += u;
	}
}

// cList<TYPE, CONTAINER>::operator -=
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::operator -= (const TYPE &u) {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	for(int i = 0; i < CurCount; i++) {
		m_Container->List[i] -= u;
	}
}

// cList<TYPE, CONTAINER>::operator *=
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::operator *= (const TYPE &u) {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	for(int i = 0; i < CurCount; i++) {
		m_Container->List[i] *= u;
	}
}

// cList<TYPE, CONTAINER>::operator /=
template<class TYPE, class CONTAINER>
inline void cList<TYPE, CONTAINER>::operator /= (const TYPE &u) {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	for(int i = 0; i < CurCount; i++) {
		m_Container->List[i] /= u;
	}
}

// cList<TYPE, CONTAINER>::operator ==
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::operator == (const cList<TYPE, CONTAINER> &Other) const {
	int CurCount = (NULL == m_Container) ? 0 : m_Container->Count;
	int OtherCount = (NULL == Other.m_Container) ? 0 : Other.m_Container->Count;
	if(CurCount != OtherCount) {
		return false;
	}
	for(int i = 0; i < CurCount; i++) {
		if(m_Container->List[i] != Other.m_Container->List[i]) {
			return false;
		}
	}
	return true;
}

// cList<TYPE, CONTAINER>::operator !=
template<class TYPE, class CONTAINER>
inline bool cList<TYPE, CONTAINER>::operator != (const cList<TYPE, CONTAINER> &Other) const {
	return !(*this == Other);
}

template<class TYPE, class CONTAINER>
::std::ostream & operator << (::std::ostream &out, const cList<TYPE, CONTAINER> &l) {
	for(int i = 0; i < l.Count(); ++i) {
		const bool needSeparator = ((i + 1) != l.Count());
		out << l[i] << (needSeparator ? " " : "");
	}
	return out;
}
