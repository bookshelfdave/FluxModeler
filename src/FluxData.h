#ifndef FLUXDATA_H
#define FLUXDATA_H

//==================================================================
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from it's use.
// Users of this code must verify correctness for their application.
//==================================================================




//enum boolean {FALSE=0, TRUE=1, ERROR=(-1)};

// Error codes
enum Error {
    Enot,	// no error
    Edim,	// error: dim of point invalid for operation
    Esum	// error: sum not affine (cooefs add to 1)
};

// utility macros
#define	abs(x)		((x) >= 0 ? x : -(x));
#define	min(x,y)	((x) < (y) ? (x) : (y));
#define	max(x,y)	((x) > (y) ? (x) : (y));


//==================================================================
//  Point Class Definition
//==================================================================



class Vector;

class Point {
    friend class Vector;
public:

    //----------------------------------------------------------
    // Lots of Constructors (add more as needed)
    Point() : x(0.0), y(0.0), z(0.0), dimn(3), err(Enot) {}
    // 1D Point
    explicit Point( int a) : x(a), y(0), z(0), dimn(1),err(Enot) {}
    explicit Point( double a) : x(a),y(0.0),z(0.0),dimn(1),err(Enot) {}

    // 2D Point
    Point( int a, int b) : x(a), y(b), z(0), dimn(2), err(Enot) {}
    Point( double a, double b) : x(a), y(b), z(0.0), dimn(2), err(Enot) {}

    // 3D Point
    Point( int a, int b, int c) : x(a),y(b),z(c), dimn(3), err(Enot) {}
    Point( double a, double b, double c) : x(a),y(b),z(c), dimn(3), err(Enot) {}

    // n-dim Point
    Point( int n, int a[]);
    Point( int n, double a[]);

    // Destructor
    ~Point() {}

    //----------------------------------------------------------
    // Input/Output streams
//        friend istream& operator>>( istream&, Point&);
//        friend ostream& operator<<( ostream&, Point);

    //----------------------------------------------------------
    // Assignment "=": use the default to copy all members
    int dim() {
        return dimn;    // get dimension
    }
    int setdim( int);               // set new dimension

    //----------------------------------------------------------
    // Comparison (dimension must match, or not)
    int operator==( Point);
    int operator!=( Point);

    //----------------------------------------------------------
    // Point and Vector Operations (always valid)
    Vector operator-( Point);       // Vector difference
    Point  operator+( Vector);      // +translate
    Point  operator-( Vector);      // -translate
    Point& operator+=( Vector);     // inc translate
    Point& operator-=( Vector);     // dec translate

    //----------------------------------------------------------
    // Point Scalar Operations (convenient but often illegal)
    // using any type of scalar (int, float, or double)
    //    are not valid for points in general,
    //    unless they are 'affine' as coeffs of
    //    a sum in which all the coeffs add to 1,
    //    such as: the sum (a*P + b*Q) with (a+b == 1).
    //    The programmer must enforce this (if they want to).

    // Scalar Multiplication
    friend Point operator*( int, Point);
    friend Point operator*( double, Point);
    friend Point operator*( Point, int);
    friend Point operator*( Point, double);
    // Scalar Division
    friend Point operator/( Point, int);
    friend Point operator/( Point, double);

    //----------------------------------------------------------
    // Point Addition (also convenient but often illegal)
    //    is not valid unless part of an affine sum.
    //    The programmer must enforce this (if they want to).
    friend Point operator+( Point, Point);     // add points

    // Affine Sum
    // Returns weighted sum, even when not affine, but...
    // Tests if coeffs add to 1.  If not, sets: err = Esum.
    friend Point asum( int, int[], Point[]);
    friend Point asum( int, double[], Point[]);

    //----------------------------------------------------------
    // Point Relations
    friend double d( Point, Point);         // Distance
    friend double d2( Point, Point);        // Distance^2
    double isLeft( Point, Point);           // 2D only
    double Area( Point, Point); 		// any dim for triangle PPP

    // Collinearity Conditions (any dim n)
    bool isOnLine( Point, Point, char);  // is On line (char= flag)
    bool isOnLine( Point, Point);        // is On line (flag= all)
    bool isBefore( Point, Point);        // is On line (flag= before)
    bool isBetween( Point, Point);       // is On line (flag= between)
    bool isAfter( Point, Point);         // is On line (flag= after)
    bool isOnRay( Point, Point);         // is On line (flag= between|after)

    //----------------------------------------------------------
    // Error Handling
    void  clerr() {
        err = Enot;   // clear error
    }
    int   geterr() {
        return err;   // get error
    }
    const char* errstr();                         // return error string

public:
    double x, y, z;      // z=0 for 2D, y=z=0 for 1D

protected:
    int dimn;            // # coords (1, 2, or 3 max here)
    Error err;           // error indicator
};

#include "math.h"
//==================================================================
//  Vector Class Definition
//==================================================================

class Vector : public Point {
public:
    // Constructors same as Point class
    Vector() : Point() {}
    explicit Vector( int a) : Point(a) {}
    explicit Vector( double a) : Point(a) {}
    Vector( int a, int b) : Point(a,b) {}
    Vector( double a, double b) : Point(a,b) {}
    Vector( int a, int b, int c) : Point(a,b,c) {}
    Vector( double a, double b, double c) : Point(a,b,c) {}
    Vector( int n, int a[]) : Point(n,a) {}
    Vector( int n, double a[]) : Point(n,a) {}
    ~Vector() {}

    //----------------------------------------------------------
    // IO streams and Comparisons: inherit from Point class

    //----------------------------------------------------------
    // Vector Unary Operations
    Vector operator-();                // unary minus
    Vector operator~();                // unary 2D perp operator

    //----------------------------------------------------------
    // Scalar Multiplication
    friend Vector operator*( int, Vector);
    friend Vector operator*( double, Vector);
    friend Vector operator*( Vector, int);
    friend Vector operator*( Vector, double);
    // Scalar Division
    friend Vector operator/( Vector, int);
    friend Vector operator/( Vector, double);

    //----------------------------------------------------------
    // Vector Arithmetic Operations
    Vector operator+( Vector);        // vector add
    Vector operator-( Vector);        // vector subtract
    double operator*( Vector);        // inner dot product
    double operator|( Vector);        // 2D exterior perp product
    Vector operator^( Vector);        // 3D exterior cross product

    Vector& operator*=( double);      // vector scalar mult
    Vector& operator/=( double);      // vector scalar div
    Vector& operator+=( Vector);      // vector increment
    Vector& operator-=( Vector);      // vector decrement
    Vector& operator^=( Vector);      // 3D exterior cross product

    //----------------------------------------------------------
    // Vector Properties
    double len() {                    // vector length
        return sqrt(x*x + y*y + z*z);
    }
    double len2() {                   // vector length squared (faster)
        return (x*x + y*y + z*z);
    }

    //----------------------------------------------------------
    // Special Operations
    void normalize();                 // convert vector to unit length
    friend Vector sum( int, int[], Vector[]);     // vector sum
    friend Vector sum( int, double[], Vector[]);  // vector sum
};


#endif // DATA_H
