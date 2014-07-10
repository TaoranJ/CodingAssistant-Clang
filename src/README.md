# CodingAssistant-Clang #

***

>[1]Bjarne Stroustrup. **The C++ Programming Language 4th Edition**.
>
>[2]Bjarne Stroustrup. **The C++ Programming Language 3rd Edition**.
>
>[3]Herb Sutter ,Andrei Alexandrescu . **C++ Coding Standards 101 Rules ,GuideLines, and Best Practices**.
>
>[4]Scott Meyers . **Effective C++: 55 Specific Ways to Improve Your Programs and Designs,3rd Edition**.

***

## Rule 001: MatchCString ##

### Description: ###
    * Prefer strings over C-style strings. 4.6[1]
### Implementation: ###
    1. char *; char []; char *array[]; char **.
    2. FieldDecl, VarDecl, ParmVarDecl.
### May be bugs: ###
    * pass the return type of a function.
    * pass the the usage of these variables.

***    

## Rule 002: MatchArray ##

### Description: ###
    * Prefer vector<T>, map<K,T>, and unordered_map<K,T> over T[]. 4.8[1]
### Implementation: ###
    1. find built-in [].
    2. user-defined type.
### May be bugs: ###
    * I don't know if char * should also be checked.

***

## Rule 003: MatchArray ##

### Description: ###
    * Prefer vector<T>, map<K,T>, and unordered_map<K,T> over T[]. 4.8[1]
### Implementation: ###
    1. find user-defined [].
    2. built-in type.
### May be bugs: ###
	* Where are you?

***

## Rule 004: MatchContainer  ##

### Description: ###
    * If in doubt use a range-checked vector. 4.12[1]
### Implementation: ###
    1. find operator[] whose definition is not in the std file.
    2. if the caller inherits from a std container.
### May be bugs: ###
    * pass [](left, right).
    * doesn't check the at().
### Examples: ###
	template<typename T>
	class Vec : public std::vector<T> {
	pulic:
		using vector<T>::vector;
		
		T& operator[](int i) { return vector<T>::at(i); }
	};

***

## Rule 005: MatchContainer ##

### Description: ###
    * If in doubt use a range-checked vector. 4.12[1]
### Implementation: ###
    1. find operator[] whose definition is in the std file.
    2. in a std container.
### May be bugs: ###
	* Where are you?
	
***

## Rule 006: MatchMainCatch ##

### Description: ###
    * Catch common exceptions in main(). 4.15[1]
### Implementation: ###
    1. find function which has try in it.
    2. isMain().
### May be bugs: ###
	* Where are you?
### Examples: ###
	int main() {
		try 
		{
			//code
		}
		catch(out_of_range)
		{
			cerr << "range error\n";
		}
	}

***
    
## Rule 007: MatchRealloc ##

### Description: ###
    * Use push_back() on a vector rather than realloc() on an array 31.20[1]
### Implementation: ###
    1. find realloc
### May be bugs: ###
	* Where are you?
 
***

## Rule 008: MatchNotPlainChar ##

### Description: ###
    * Prefer plain char over signed char and unsigned char. 6.9[1]
### Implementation: ###
    1. find declarator decl.
    2. type
### May be bugs: ###
	* Where are you?
    
***

## Rule 009: MatchSignedCast ##

### Description: ###
    * Beware of conversions between signed and unsigned types. 6.10[1]
### Implementation: ###
    1. ImplicitCastExpr
### May be bugs: ###
	* Where are you?

***

## Rule 010: MatchMultiDeclarations ##

### Description: ###
    * Declare one name per declaration. 6.11[1]
### Implementation: ###
    1. decl stmt
### May be bugs: ###
	* Where are you?

***

## Rule 011: MatchUninitVar ##

### Description: ###
    * Avoid uninitialized variables. 6.21[1]
### Implementation: ###
    1. VarDecl
    2. !hasInit()
### May be bugs: ###
	* Where are you?
    
***

## Rule 012: MatchAuto ##

### Description: ###
    * Prefer the = syntax for the initialization in delcarations using auto. 6.20[1]
### Implementation: ###
    1. VarDecl.
    2. auto type.
    3. without =.
### May be bugs: ###
	* Where are you?
    
***

## Rule 013: MatchInitList ##

### Description: ###
    * Prefer the {}-initializer syntax for declarations with a named type. 6.19[1]
### Implementation: ###
    1. VarDecl
    2. hasInitializer(initListExpr())
### May be bugs: ###
	* Where are you?

***

## Rule 014: MatchInitList ##

### Description: ###
    * Prefer the {}-initializer syntax for declarations with a named type. 6.19[1]
### Implementation: ###
    1. VarDecl.
    2. hasInitializer(constructExpr()).
### May be bugs: ###
	* Where are you?
    
***

## Rule 015: MatchIntegerToChar ##

### Description: ###
    * Avoid unnecessary assumptions about the numeric value of characters. 6.4[1]
### Implementation: ###
    1. int x = 'c';
    2. char c = 12;
    3. int x = 1; char c = x;
    4. i = c;
    5. c = i;
### May be bugs: ###
	* Where are you?

***

## Rule 016: MatchMultidimArrays ##

### Description: ###
    * Avoid multidimentsional arrays; define suitable containers instead. 7.4[1]
### Implementation: ###
    1. find multidimentional arrays. 
### May be bugs: ###
	* Where are you?
    
***

## Rule 017: MatchNull ##

### Description: ###
    * Use nullptr rather than 0 or NULL. 7.5[1]
### Implementation: ###
    1. ==
    2. !=
    3. VarDecl
### May be bugs: ###
	* Where are you?

***

## Rule 018: MatchRefParm ##

### Description: ###
    * Prefer const reference arguments to plain reference arguments. 7.9[1]
### Implementation: ###
    1. parm 
    2. is reference type
### May be bugs: ###
    * pass override method
   
***

##  Rule 019: MatchHandleClass  ##

### Description:  ###
    * Keep pointers that represent ownership inside handle classes. 7.11[1]
    * Use resource handles and RAII to manage resources. 3.5[1] 5.1[1]
    * Use resource handles to avoid leaks, premature deletion, and double deletion. 11.2[1]
    * Use RAII 11.5[1]
### Implementation: ###
    1. CXXConstructExpr
    2. CXXNewExpr
    3. std(garbage collection interface)
### May be bugs: ###
	* where are you?
### Examples: ###
	class RAIIExample {
	public:
		RAIIExample(Resource* s){}
		~RAIIExample(){delete resourceHandle;}
	private:
		Resource resourceHandle;
	};
	void f()
	{
		RAIIExample resource(new Resource);
	}

***

##  Rule 020: MatchHandleClass  ##

###  Description:  ###
   	* Keep pointers that represent ownership inside handle classes. 7.11[1]
    * Use resource handles and RAII to manage resources. 3.5[1] 5.1[1]
    * Use resource handles to avoid leaks, premature deletion, and double deletion. 11.2[1]
    * Use RAII 11.5[1]
###  Implementation:  ###
    1. CXXConstructExpr
    2. CXXNewExpr
    3. non-std(user-defined RAII)
### May be bugs: ###
	* where are you?
### Examples: ###
	* see Rule019

***

## Rule 021: MatchNakedNew  ##

### Description:  ###
	* Avoid "naked" new and delete operations. 3.4[1] 11.4[1]
    * Keep pointers that represent ownership inside handle classes. 7.11[1]
### Implementation: ###
    1. varDecl with newExpr
    2. has local storage
### May be bugs: ###
	* where are you?
### Examples: ###
	void f(int n, int *m)
	{
		int* p1 = new int[n]; //potential trouble
		if (n % 2) throw runtime_error("odd");
		if (nullptr == m) return; // something is wrong
		delete[] p1; //we may never get here
	}
	
***

## Rule 022: MatchVoidPointer ##

### Description: ###
    * Avoid void* except in low-level code. 1-7[12]
### Implementation: ###
    1. void*
    2. void**
    3. void*[]
### May be bugs: ###
	* where are you?
 
***
   
## Rule 023: MatchStruct ##

### Description: ###
    * When compactness of data is important, lay out structure data memebers with larger numbers before smaller ones. 8.1[1]
### Implementation: ###
    1. only compare builtin type size
### May be bugs: ###
	* ugly code

***

## Rule 024: MatchBitField  ##

### Description: ###
    * Use bit-fields to represent hardware-imposed data layouts. 8.2[1]
    * Don't natively try to optimize memory construction by packing several values * into a single byte. 8.3[1]
### Implementation: ###
    1. RecordDecl;
    2. FieldDecl;
    2. Bitfield;
### May be bugs: ###
	* Where are you?

***

## Rule 025: MatchEnum  ##

### Description: ###
    * Prefer class enums over "plain" enums to minimize surprise. 8.6[1]
### Implementation: ###
    1. plaim enum
### May be bugs: ###
	* Where are you?

***

## Rule 026: MatchForStmt  ##

### Description: ###
    * Prefer a range-for-statement to a for-statement when there is a choice. 9.3[1]
### Implementation: ###
    1. for range
    2. for
### May be bugs: ###
	* Where are you?

***

## Rule 027: MatchDoStmt ##

### Description: ###
    * Avoid do-statements. 9.6[1]
### Implementation: ###
    1. do statement.
### May be bugs: ###
	* Where are you?

***

## Rule 028: MatchGotoStmt ##

### Description: ###
    * Avoid goto. 9.7[1]
### Implementation: ###
    1. goto
### May be bugs: ###
	* Where are you?

***

## Rule 029: MatchSTD ##

### Description: ###
    * Prefer the standard library to other libraries and to "handcrafted code". 10.1[1]
### Implementation: ###
    1. std var
    2. std function
### May be bugs: ###
	* Where are you?

***

## Rule 030: MatchNarrowingConversion ##

### Description: ###
    * Avoid narrowing conversions. 10.8[1]
### Implementation: ###
	1. check some special cast(FloatingToInteger ..)
### May be bugs: ###
	* pass the situation in which long int turn to small int

***

## Rule 031: MatchNewExpr ##

### Description: ###
    * Don't put objects on the free store if you don't have to; prefer scoped variables. 11.3[1]
### Implementation: ###
    1. newExpr
### May be bugs: ###
	* Where are you?
    
***

## Rule 032: MatchLambda ##

### Description: ###
    * Keep lambdas short. 11.8[1]
### Implementation: ###
    1. lambda
### May be bugs: ###
	* Where are you?

***

## Rule 033: MatchLambda ##

### Description: ###
    * Let the compiler deduce the return type of a lambda. 11.10[1]
### Implementation: ###
    1. lambda
### May be bugs: ###
	* Where are you?

***

## Rule 034: MatchFunCast ##

### Description: ###
    * Avoid explicit type conversion. 11.12[1]
### Implementation: ###
    1. functional cast
### May be bugs: ###
	* Where are you?

***

## Rule 035: MatchDynamicCast ##

### Description: ###
    * Avoid explicit type conversion. 11.12[1]
### Implementation: ###
    1. dynamic_cast
### May be bugs: ###
	* Where are you?

***

## Rule 036: MatchFuncConstexpr ##

### Description: ###
    * If a function may have to be evaluated at compiletime, declare it constexpr. 12.5[1] 
### Implementation: ###
    1. func
    2. non constexpr
### May be bugs: ###
	* Where are you?

***

## Rule 037: MatchShouldBeConstRef ##

### Description: ###
    * Use pass-by-const-reference to pass large values that you don't need to modify. 12.8[1]
### Implementation: ###
	1. RefDeclExpr
### May be bugs: ###
	* Where are you?

***

## Rule 038: MatchPassByValue ##

### Description: ###
    * Use pass-by-value for small objects. 12.7[1]
### Implementation: ###
    1. pass-by-value
    2. built-in
### May be bugs: ###
	* Where are you?

***

## Rule 039: MatchReturnWarning ##

### Description: ###
    * Return a result as a return value rather than modifying an object through an argument. 12.9[1]
### Implementation: ###
    1. ReturnStmt
    2. pointer or reference
### May be bugs: ###
	* Where are you?

***

## Rule 040: MatchAryAsPtr ##

### Description: ###
    * Avoid passing arrays as pointers. 12.15[1]
### Implementation: ###
	1. ArrayToPointerDecay
### May be bugs: ###
	* Where are you?

***

## Rule 041: MatchVarParm ##

### Description: ###
    * Pass a homogeneous list of unknown length as an initialzier_list<T>. 1-12[16]
    * Avoid unspecified numbers of argument. 12.16[1]
### Implementation: ###
    1. isVariadic
### May be bugs: ###
	* Where are you?

***

## Rule 042: MatchOverloading ##

### Description: ###
    * When overloading on integers, provide functions to eliminate common ambiguities. 12.19[1]
### Implementation: ###
	1. count the overloading times
### May be bugs: ###
	* ugly code

***

## Rule 043 MatchNamespace ##

### Description: ###
    * Avoid very short names for namespaces. If necessary, use namespace aliases to abbreviate long namespace names. 8.4[2]
### Implementation: ###
	1. NamespaceDecl
### May be bugs: ###
	* Where are you？

***

## Rule 044: -Wunused-parameter ##

***

## Rule 045: MatchCast ##

### Description: ###
    * Avoid using reinterpret_cast. 92[3],27[4]
    * Avoid explicit type conversion. 11.2[1]
### Implementation: ###
    1. reinterpret_cast
### May be bugs: ###
	* Where are you?

***

## Rule 046: MatchCast ##

### Description: ###
    * Avoid using static_cast on pointers. 92[1],27[2]
    * Avoid explicit type conversion. 11.2[1]
### Implementation: ###
    1. static_cast
### May be bugs: ###
	* Where are you?
***

## Rule 047: MatchCast ##

### Description: ###
    * Avoid casting away const. 92[1],27[2]
    * Avoid explicit type conversion. 11.2[1]
### Implementation: ###
    1. const_cast
### May be bugs: ###
	* Where are you?

***

## Rule 048: MatchCast ##

### Description: ###
    * Don't use C-style casts. 92[1],27[2]
    * Avoid explicit type conversion. 11.2[1]
### Implementation: ###
    1. cstyle-cast
### May be bugs: ###
	* Where are you?

***

## Rule 049: MatchMagicNumber  ##

### Description: ###
    * Avoid magic numbers.
    * Avoid "magic constants". 6[6]
### Implementation: ###
    1. magic numbers.
### May be bugs: ###
	* Where are you?

***

## Rule 050: MatchFuncLines ##

### Description: ###
    * Avoid long function. 20[3]
### Implementation: ###
    1. long function
### May be bugs: ###
	* Where are you?

***

## Rule 052: MatchGlobalVar ##

### Description: ###
    * Minimize global and shared data. 4.4[2]
### Implementation: ###
    1. global var name which is too long
### May be bugs: ###
	* Where are you?

***

## Rule 053: MatchGlobalVar ##

### Description: ###
    * Minimize global and shared data. 4.4[2]
### Implementation: ###
    1. global
### May be bugs: ###
	* Where are you?

***

## Rule 053: MatchSimilarName  ##

### Description: ###
    * Avoid similar names. 4.5[2]
### Implementation: ###
    1. named decl
    2. getNameAsString()
### May be bugs: ###
	* Where are you?

***

## Rule 054: MatchUninline ##

### Description: ###
    * Avoid inline function defined in non header files. 9.4[2]
### Implementation: ###
    1. isInlineSpecified
### May be bugs: ###
	* Where are you?

***

## Rule 055: MatchVirtualCalee ##

### Description: ###
    * Never call virtual functions during construction or destruction. 9[4]
### Implementation: ###
    1. CXXConstructorDecl 
    2. CXXDestructorDecl 
### May be bugs: ###
	* Where are you?

***

## Rule 056: MatchVarDefinition ##

Description:
    * Postpone variable definitons as long as possible. 26[4]
Implementation:
    1. DeclRefExpr
    2. getDecl
    3. diffNo
### May be bugs: ###
	* Where are you?

***

## Rule 057: MatchCMemFunc ##

### Description: ###
    * Avoid malloc(), free(), and realloc(). 19.11[2]
### Implementation: ###
    1. call expr
    2. DeclRefExpr
    3. has name 
### May be bugs: ###
	* Where are you?

***

## Rule 058: MatchCMemFunc ##

### Description: ###
    * Don't memcpy or memcmp non-pods. 96[2]
### Implementation: ###
    1. Rule061.1
    2. Rule061.2
    3. POD
### May be bugs: ###
	* Where are you?

***

## Rule 059: MatchPrivate ##

### Description: ###
    * Make data member private, except in behaviorless aggregates(C-style). 41[2] 22[3]
### Implementation: ###
    1. protected
    2. public
    3. isAggregate()
### May be bugs: ###
	* Where are you?

***

## Rule 060: MatchPointerMath ##

### Description: ###
    * Avoid the calculate between a pointer and a literal. 5.1[2]
### Implementation: ###
    1. LHS ignoringImpCasts DeclRefExpr pointerType
    2. RHS ... ingeterLiteral
    3. op type
### May be bugs: ###
	* Where are you?

***

## Rule 061: MatchReturn ##

### Description: ###
    * Don't return local var as pointer or reference. 7.6[2]
### Implementation: ###
    1. -Wreturn-statck-address solve reference problem.
### May be bugs: ###
	* pointer problem unresloved

***


## Rule 062: MatchVirtualFunc ##

### Description: ###
    * Consider make virtual functions nonpublic, and public functions nonvirtual. 39[3]
### Implementation: ###
    1. method decl;
    2. is public;
    3. is virtual
### May be bugs: ###
	* Where are you?

***

## Rule 063: MatchScope ##

### Description: ###
    * Keep scopes small.
### Implementation: ###
    1. leaf scope in which there is no other compound stmt. 4.1[2]
### May be bugs: ###
	* Where are you?

***

## Rule 064: MatchUsing ##

### Description: ###
    * Don't write namespace usings in a header file. 9.4[2]
### Implementation: ###
    1. using
### May be bugs: ###
	* only match using-declarations - using somenamespace::identifier

***

## Rule 065: MatchOnlyDefaultConstructor ##

### Description: ###
    * Define a constructor to handle initialization of objects. 16.4[1]
### Implementation: ###
    1. RecordDecl
    2. ConstructDecl
    3. unless

### May be bugs: ###
	* Where are you?

***

# *to be continued...* #
