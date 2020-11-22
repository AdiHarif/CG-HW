#pragma once
#include "vec.h"





//----------------------------------------------------------------------------
//
//  mat2 - 2D square matrix
//

class mat2 {

    vec2  _m[2];

   public:
    //
    //  --- Constructors and Destructors ---
    //

    mat2( const GLfloat d = GLfloat(1.0) )  // Create a diagional matrix
	{ _m[0].x = d;  _m[1].y = d;   }

    mat2( const vec2& a, const vec2& b )
	{ _m[0] = a;  _m[1] = b;  }

	/*BUG Fixed*/
    mat2( GLfloat m00, GLfloat m10, GLfloat m01, GLfloat m11 )
	{ _m[0] = vec2( m00,  m01); _m[1] = vec2( m10, m11); }

    mat2( const mat2& m ) {
	if ( *this != m ) {
	    _m[0] = m._m[0];
	    _m[1] = m._m[1];
	} 
    }

    //
    //  --- Indexing Operator ---
    //

    vec2& operator [] ( int i ) { return _m[i]; }
    const vec2& operator [] ( int i ) const { return _m[i]; }

    //
    //  --- (non-modifying) Arithmatic Operators ---
    //

    mat2 operator + ( const mat2& m ) const
	{ return mat2( _m[0]+m[0], _m[1]+m[1] ); }

	
    mat2 operator - ( const mat2& m ) const
	{ return mat2(_m[0]-m[0], _m[1]-m[1]); } /*BUG Fixed*/

    mat2 operator * ( const GLfloat s ) const 
	{ return mat2( s*_m[0], s*_m[1] ); }

    mat2 operator / ( const GLfloat s ) const {
	
	GLfloat r = GLfloat(1.0) / s;
	return *this * r;
    }

    friend mat2 operator * ( const GLfloat s, const mat2& m )
	{ return m * s; }
	
    mat2 operator * ( const mat2& m ) const { /*BUG Fixed*/
        mat2 a(0.0);
        mat2 mt = mat2(m[0][0], m[0][1], m[1][0], m[1][1]);

        for (int i=0; i < 2; i++) {
            for (int j=0; j < 2; j++) {
                a[i][j] = _m[i] * mt[j];
            }
        }
        return a;
    }

    //
    //  --- (modifying) Arithmetic Operators ---
    //

    mat2& operator += ( const mat2& m ) {
	_m[0] += m[0];  _m[1] += m[1];  
	return *this;
    }

    mat2& operator -= ( const mat2& m ) {
	_m[0] -= m[0];  _m[1] -= m[1];  /*BUG Fixed*/
	return *this;
    }

    mat2& operator *= ( const GLfloat s ) {
	_m[0] *= s;  _m[1] *= s;   
	return *this;
    }

    mat2& operator *= ( const mat2& m ) {/*BUG Fixed*/
        *this = (*this) * m;
	    return *this;
    }
    
    mat2& operator /= ( const GLfloat s ) {


	GLfloat r = GLfloat(1.0) / s;
	return *this *= r;
    }

    //
    //  --- Matrix / Vector operators ---
    //

    vec2 operator * ( const vec2& v ) const {  // m * v
        return vec2(_m[0] * v, _m[1] * v);
    }
	
    //
    //  --- Insertion and Extraction Operators ---
    //
	
    friend std::ostream& operator << ( std::ostream& os, const mat2& m )
	{ return os << std::endl << m[0] << std::endl << m[1] << std::endl; }

    friend std::istream& operator >> ( std::istream& is, mat2& m )
	{ return is >> m._m[0] >> m._m[1] ; }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat* () const
	{ return static_cast<const GLfloat*>( &_m[0].x ); }

    operator GLfloat* ()
	{ return static_cast<GLfloat*>( &_m[0].x ); }
};

//
//  --- Non-class mat2 Methods ---
//

//inline
//mat2 matrixCompMult( const mat2& A, const mat2& B ) {
//    return mat2( A[0][0]*B[0][0], A[0][1]*B[0][1],
//		 A[1][0]*B[1][0], A[1][1]*B[1][1] );
//}

inline
mat2 transpose( const mat2& A ) {
    return mat2( A[0][0], A[1][0],
		 A[0][1], A[1][1] );
}

//----------------------------------------------------------------------------
//
//  mat3 - 3D square matrix 
//

class mat3 {

    vec3  _m[3];

   public:
    //
    //  --- Constructors and Destructors ---
    //

    mat3( const GLfloat d = GLfloat(1.0) )  // Create a diagonal matrix
	{ _m[0].x = d;  _m[1].y = d;  _m[2].z = d;   }

    mat3( const vec3& a, const vec3& b, const vec3& c )
	{ _m[0] = a;  _m[1] = b;  _m[2] = c;  }

    mat3( GLfloat m00, GLfloat m10, GLfloat m20,
	  GLfloat m01, GLfloat m11, GLfloat m21,
	  GLfloat m02, GLfloat m12, GLfloat m22 ) 
	{
	    _m[0] = vec3( m00, m01, m02 );
	    _m[1] = vec3( m10, m11, m12 );
	    _m[2] = vec3( m20, m21, m22 );
	}

    mat3( const mat3& m )
	{
	    if ( *this != m ) {
		_m[0] = m._m[0];
		_m[1] = m._m[1];
		_m[2] = m._m[2];
	    } 
	}

    //
    //  --- Indexing Operator ---
    //

    vec3& operator [] ( int i ) { return _m[i]; }
    const vec3& operator [] ( int i ) const { return _m[i]; }

    //
    //  --- (non-modifying) Arithmatic Operators ---
    //

    mat3 operator + ( const mat3& m ) const
	{ return mat3( _m[0]+m[0], _m[1]+m[1], _m[2]+m[2] ); }

    mat3 operator - ( const mat3& m ) const
	{ return mat3( _m[0]-m[0], _m[1]-m[1], _m[2]-m[2] ); }

    mat3 operator * ( const GLfloat s ) const 
	{ return mat3( s*_m[0], s*_m[1], s*_m[2] ); }

    mat3 operator / ( const GLfloat s ) const {

	
	GLfloat r = GLfloat(1.0) / s;
	return *this * r;
    }

    friend mat3 operator * ( const GLfloat s, const mat3& m )
	{ return m * s; }
	
    mat3 operator * ( const mat3& m ) const {
	mat3  a( 0.0 );
    mat3 mt = mat3(m[0][0], m[0][1], m[0][2],
                    m[1][0], m[1][1], m[1][2],
                    m[2][0], m[2][1], m[2][2]);

	for ( int i = 0; i < 3; ++i ) {
	    for ( int j = 0; j < 3; ++j ) {
            a[i][j] = _m[i] * mt[j];
	    }
	}

	return a;
    }

    //
    //  --- (modifying) Arithmetic Operators ---
    //

    mat3& operator += ( const mat3& m ) {
	_m[0] += m[0];  _m[1] += m[1];  _m[2] += m[2]; 
	return *this;
    }

    mat3& operator -= ( const mat3& m ) {
	_m[0] -= m[0];  _m[1] -= m[1];  _m[2] -= m[2]; 
	return *this;
    }

    mat3& operator *= ( const GLfloat s ) {
	_m[0] *= s;  _m[1] *= s;  _m[2] *= s; 
	return *this;
    }

    mat3& operator *= ( const mat3& m ) {
	mat3  a( 0.0 );

	for ( int i = 0; i < 3; ++i ) {
	    for ( int j = 0; j < 3; ++j ) {
		for ( int k = 0; k < 3; ++k ) {
		    a[i][j] += _m[i][k] * m[k][j];
		}
	    }
	}

	return *this = a;
    }

    mat3& operator /= ( const GLfloat s ) {


	GLfloat r = GLfloat(1.0) / s;
	return *this *= r;
    }

    //
    //  --- Matrix / Vector operators ---
    //

    vec3 operator * ( const vec3& v ) const {  // m * v
        return vec3(_m[0] * v, _m[1] * v, _m[2] * v); /*BUG Fixed*/
    }
	
    //
    //  --- Insertion and Extraction Operators ---
    //
	
    friend std::ostream& operator << ( std::ostream& os, const mat3& m ) {
	return os << std::endl 
		  << m[0] << std::endl
		  << m[1] << std::endl
		  << m[2] << std::endl;
    }

    friend std::istream& operator >> ( std::istream& is, mat3& m )
	{ return is >> m._m[0] >> m._m[1] >> m._m[2] ; }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat* () const
	{ return static_cast<const GLfloat*>( &_m[0].x ); }

    operator GLfloat* ()
	{ return static_cast<GLfloat*>( &_m[0].x ); }
};

//
//  --- Non-class mat3 Methods ---
//

inline
mat3 matrixCompMult( const mat3& A, const mat3& B ) {
    return mat3( A[0][0]*B[0][0], A[0][1]*B[0][1], A[0][2]*B[0][2],
		 A[1][0]*B[1][0], A[1][1]*B[1][1], A[1][2]*B[1][2],
		 A[2][0]*B[2][0], A[2][1]*B[2][1], A[2][2]*B[2][2] );
}

inline
mat3 transpose( const mat3& A ) {/*BUG Fixed*/
    return mat3(A[0][0], A[1][0], A[2][0],
        A[0][1], A[1][1], A[2][1],
        A[0][2], A[1][2], A[2][2]);
}

//----------------------------------------------------------------------------
//
//  mat4.h - 4D square matrix
//

class mat4 {

    vec4  _m[4];

   public:
    //
    //  --- Constructors and Destructors ---
    //

    mat4( const GLfloat d = GLfloat(1.0) )  // Create a diagional matrix
	{ _m[0].x = d;  _m[1].y = d;  _m[2].z = d;  _m[3].w = d; }

    mat4( const vec4& a, const vec4& b, const vec4& c, const vec4& d )
	{ _m[0] = a;  _m[1] = b;  _m[2] = c;  _m[3] = d; }

    mat4( GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
	  GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
	  GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
	  GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33 )
	{
	    _m[0] = vec4( m00, m01, m02, m03 );
	    _m[1] = vec4( m10, m11, m12, m13 );
	    _m[2] = vec4( m20, m21, m22, m23 );
	    _m[3] = vec4( m30, m31, m32, m33 );
	}

    mat4( const mat4& m )
	{
	    if ( *this != m ) {
		_m[0] = m._m[0];
		_m[1] = m._m[1];
		_m[2] = m._m[2];
		_m[3] = m._m[3];
	    } 
	}

    //
    //  --- Indexing Operator ---
    //

    vec4& operator [] ( int i ) { return _m[i]; }
    const vec4& operator [] ( int i ) const { return _m[i]; }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    mat4 operator + ( const mat4& m ) const
	{ return mat4( _m[0]+m[0], _m[1]+m[1], _m[2]+m[2], _m[3]+m[3] ); }

    mat4 operator - ( const mat4& m ) const
	{ return mat4( _m[0]-m[0], _m[1]-m[1], _m[2]-m[2], _m[3]-m[3] ); }

    mat4 operator * ( const GLfloat s ) const 
	{ return mat4( s*_m[0], s*_m[1], s*_m[2], s*_m[3] ); }

    mat4 operator / ( const GLfloat s ) const {

	
	GLfloat r = GLfloat(1.0) / s;
	return *this * r;
    }

    friend mat4 operator * ( const GLfloat s, const mat4& m )
	{ return m * s; }
	
    mat4 operator * ( const mat4& m ) const {
	mat4  a( 0.0 );

	for ( int i = 0; i < 4; ++i ) {
	    for ( int j = 0; j < 4; ++j ) {
		for ( int k = 0; k < 4; ++k ) {
		    a[i][j] += _m[i][k] * m[k][j];
		}
	    }
	}

	return a;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    mat4& operator += ( const mat4& m ) {
	_m[0] += m[0];  _m[1] += m[1];  _m[2] += m[2];  _m[3] += m[3];
	return *this;
    }

    mat4& operator -= ( const mat4& m ) {
	_m[0] -= m[0];  _m[1] -= m[1];  _m[2] -= m[2];  _m[3] -= m[3];
	return *this;
    }

    mat4& operator *= ( const GLfloat s ) {
	_m[0] *= s;  _m[1] *= s;  _m[2] *= s;  _m[3] *= s;
	return *this;
    }

    mat4& operator *= ( const mat4& m ) {
	mat4  a( 0.0 );

	for ( int i = 0; i < 4; ++i ) {
	    for ( int j = 0; j < 4; ++j ) {
		for ( int k = 0; k < 4; ++k ) {
		    a[i][j] += _m[i][k] * m[k][j];
		}
	    }
	}

	return *this = a;
    }

    mat4& operator /= ( const GLfloat s ) {


	GLfloat r = GLfloat(1.0) / s;
	return *this *= r;
    }

    //
    //  --- Matrix / Vector operators ---
    //

    vec4 operator * ( const vec4& v ) const {  // m * v
	return vec4( _m[0][0]*v.x + _m[0][1]*v.y + _m[0][2]*v.z + _m[0][3]*v.w,
		     _m[1][0]*v.x + _m[1][1]*v.y + _m[1][2]*v.z + _m[1][3]*v.w,
		     _m[2][0]*v.x + _m[2][1]*v.y + _m[2][2]*v.z + _m[2][3]*v.w,
		     _m[3][0]*v.x + _m[3][1]*v.y + _m[3][2]*v.z + _m[3][3]*v.w
	    );
    }
	
    //
    //  --- Insertion and Extraction Operators ---
    //
	
    friend std::ostream& operator << ( std::ostream& os, const mat4& m ) {
	return os << std::endl 
		  << m[0] << std::endl
		  << m[1] << std::endl
		  << m[2] << std::endl
		  << m[3] << std::endl;
    }

    friend std::istream& operator >> ( std::istream& is, mat4& m )
	{ return is >> m._m[0] >> m._m[1] >> m._m[2] >> m._m[3]; }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat* () const
	{ return static_cast<const GLfloat*>( &_m[0].x ); }

    operator GLfloat* ()
	{ return static_cast<GLfloat*>( &_m[0].x ); }
};

//
//  --- Non-class mat4 Methods ---
//

inline
mat4 matrixCompMult( const mat4& A, const mat4& B ) {
    return mat4(
	A[0][0]*B[0][0], A[0][1]*B[0][1], A[0][2]*B[0][2], A[0][3]*B[0][3],
	A[1][0]*B[1][0], A[1][1]*B[1][1], A[1][2]*B[1][2], A[1][3]*B[1][3],
	A[2][0]*B[2][0], A[2][1]*B[2][1], A[2][2]*B[2][2], A[2][3]*B[2][3],
	A[3][0]*B[3][0], A[3][1]*B[3][1], A[3][2]*B[3][2], A[3][3]*B[3][3] );
}

inline
mat4 transpose( const mat4& A ) {
    return mat4( A[0][0], A[1][0], A[2][0], A[3][0],
		 A[0][1], A[1][1], A[2][1], A[3][1],
		 A[0][2], A[1][2], A[2][2], A[3][2],
		 A[0][3], A[1][3], A[2][3], A[3][3] );
}

//////////////////////////////////////////////////////////////////////////////
//
//  Helpful Matrix Methods
//
//////////////////////////////////////////////////////////////////////////////

#define Error( str ) do { std::cerr << "[" __FILE__ ":" << __LINE__ << "] " \
				    << str << std::endl; } while(0)

inline
vec4 mvmult( const mat4& a, const vec4& b )
{
    Error( "replace with vector matrix multiplcation operator" );

    vec4 c;
    int i, j;
    for(i=0; i<4; i++) {
	c[i] =0.0;
	for(j=0;j<4;j++) c[i]+=a[i][j]*b[j];
    }
    return c;
}

//----------------------------------------------------------------------------
//
//  Rotation matrix generators
//

inline
mat4 rotateXMat(const GLfloat theta)
{
    GLfloat angle = (M_PI / 180.0) * theta;

    mat4 c;
    c[2][2] = c[1][1] = cos(angle);
    c[2][1] = sin(angle);
    c[1][2] = -c[2][1];
    return c;
}

inline
mat4 rotateYMat(const GLfloat theta)
{
    GLfloat angle = (M_PI / 180.0) * theta;

    mat4 c;
    c[2][2] = c[0][0] = cos(angle);
    c[2][0] = sin(angle);
    c[0][2] = -c[2][0];
    return c;
}

inline
mat4 rotateZMat(const GLfloat theta)
{
    GLfloat angle = (M_PI / 180.0) * theta;

    mat4 c;
    c[0][0] = c[1][1] = cos(angle);
    c[1][0] = sin(angle);
    c[0][1] = -c[1][0];
    return c;
}


//----------------------------------------------------------------------------
//
//  Translation matrix generators
//

inline
mat4 translateMat(const GLfloat x, const GLfloat y, const GLfloat z)
{
    mat4 c;
    c[0][3] = x;
    c[1][3] = y;  /*BUG Fixed*/
    c[2][3] = z;
    return c;
}


inline
mat4 translateMat( const vec3& v )
{
    return translateMat( v.x, v.y, v.z );
}

inline
mat4 translateMat( const vec4& v )
{
    return translateMat( v.x, v.y, v.z );
}

//----------------------------------------------------------------------------
//
//  Scale matrix generators
//

inline
mat4 scaleMat( const GLfloat x, const GLfloat y, const GLfloat z )
{
    mat4 c;
    c[0][0] = x;
    c[1][1] = y; /*BUG Fixed*/
    c[2][2] = z;
    return c;
}

inline
mat4 scaleMat( const vec3& v )
{
    return scaleMat( v.x, v.y, v.z );
}

//----------------------------------------------------------------------------
