#include "FluxData.h"

//==================================================================
// Vector Class Methods
//==================================================================

//------------------------------------------------------------------
//  Unary Ops
//------------------------------------------------------------------

// Unary minus
Vector Vector::operator-() {
    Vector v;
    v.x = -x;
    v.y = -y;
    v.z = -z;
    v.dimn = dimn;
    return v;
}

// Unary 2D perp operator
Vector Vector::operator~() {
    if (dimn != 2) err = Edim;   // and continue anyway

    Vector v;
    v.x = -y;
    v.y = x;
    v.z = z;
    v.dimn = dimn;
    return v;
}

//------------------------------------------------------------------
//  Scalar Ops
//------------------------------------------------------------------

// Scalar multiplication
Vector operator*( int c, Vector w ) {
    Vector v;
    v.x = c * w.x;
    v.y = c * w.y;
    v.z = c * w.z;
    v.dimn = w.dim();
    return v;
}

Vector operator*( double c, Vector w ) {
    Vector v;
    v.x = c * w.x;
    v.y = c * w.y;
    v.z = c * w.z;
    v.dimn = w.dim();
    return v;
}

Vector operator*( Vector w, int c ) {
    Vector v;
    v.x = c * w.x;
    v.y = c * w.y;
    v.z = c * w.z;
    v.dimn = w.dim();
    return v;
}

Vector operator*( Vector w, double c ) {
    Vector v;
    v.x = c * w.x;
    v.y = c * w.y;
    v.z = c * w.z;
    v.dimn = w.dim();
    return v;
}

// Scalar division
Vector operator/( Vector w, int c ) {
    Vector v;
    v.x = w.x / c;
    v.y = w.y / c;
    v.z = w.z / c;
    v.dimn = w.dim();
    return v;
}

Vector operator/( Vector w, double c ) {
    Vector v;
    v.x = w.x / c;
    v.y = w.y / c;
    v.z = w.z / c;
    v.dimn = w.dim();
    return v;
}

//------------------------------------------------------------------
//  Arithmetic Ops
//------------------------------------------------------------------

Vector Vector::operator+( Vector w ) {
    Vector v;
    v.x = x + w.x;
    v.y = y + w.y;
    v.z = z + w.z;
    v.dimn = max( dimn, w.dim());
    return v;
}

Vector Vector::operator-( Vector w ) {
    Vector v;
    v.x = x - w.x;
    v.y = y - w.y;
    v.z = z - w.z;
    v.dimn = max( dimn, w.dim());
    return v;
}

//------------------------------------------------------------------
//  Products
//------------------------------------------------------------------

// Inner Dot Product
double Vector::operator*( Vector w ) {
    return (x * w.x + y * w.y + z * w.z);
}

// 2D Exterior Perp Product
double Vector::operator|( Vector w ) {
    if (dimn != 2) err = Edim;    // and continue anyway

    return (x * w.y - y * w.x);
}

// 3D Exterior Cross Product
Vector Vector::operator^( Vector w ) {
    Vector v;
    v.x = y * w.z - z * w.y;
    v.y = z * w.x - x * w.z;
    v.z = x * w.y - y * w.x;
    v.dimn = 3;
    return v;
}

//------------------------------------------------------------------
//  Shorthand Ops
//------------------------------------------------------------------

Vector& Vector::operator*=( double c ) {        // vector scalar mult
    x *= c;
    y *= c;
    z *= c;
    return *this;
}

Vector& Vector::operator/=( double c ) {        // vector scalar div
    x /= c;
    y /= c;
    z /= c;
    return *this;
}

Vector& Vector::operator+=( Vector w ) {        // vector increment
    x += w.x;
    y += w.y;
    z += w.z;
    dimn = max(dimn, w.dim());
    return *this;
}

Vector& Vector::operator-=( Vector w ) {        // vector decrement
    x -= w.x;
    y -= w.y;
    z -= w.z;
    dimn = max(dimn, w.dim());
    return *this;
}

Vector& Vector::operator^=( Vector w ) {        // 3D exterior cross product
    double ox=x, oy=y, oz=z;
    x = oy * w.z - oz * w.y;
    y = oz * w.x - ox * w.z;
    z = ox * w.y - oy * w.x;
    dimn = 3;
    return *this;
}

//------------------------------------------------------------------
//  Special Operations
//------------------------------------------------------------------

void Vector::normalize() {                      // convert to unit length
    double ln = sqrt( x*x + y*y + z*z );

    if (ln == 0) return;                    // do nothing for nothing

    x /= ln;
    y /= ln;
    z /= ln;
}

Vector sum( int n, int c[], Vector w[] ) {     // vector sum
    int     maxd = 0;
    Vector  v;

    for (int i=0; i<n; i++) {
        if (w[i].dim() > maxd)
            maxd = w[i].dim();
    }

    v.dimn = maxd;

    for (int i=0; i<n; i++) {
        v.x += c[i] * w[i].x;
        v.y += c[i] * w[i].y;
        v.z += c[i] * w[i].z;
    }

    return v;
}

Vector sum( int n, double c[], Vector w[] ) {  // vector sum
    int     maxd = 0;
    Vector  v;

    for (int i=0; i<n; i++) {
        if (w[i].dim() > maxd)
            maxd = w[i].dim();
    }

    v.dimn = maxd;

    for (int i=0; i<n; i++) {
        v.x += c[i] * w[i].x;
        v.y += c[i] * w[i].y;
        v.z += c[i] * w[i].z;
    }

    return v;
}



// n-dim Point
Point::Point( int n, int a[]) {
    x = y = z = 0;
    err = Enot;

    switch (dimn = n) {
    case 3:
        z = a[2];
    case 2:
        y = a[1];
    case 1:
        x = a[0];
        break;
    default:
        err=Edim;
    }
}

Point::Point( int n, double a[]) {
    x = y = z = 0.0;
    err = Enot;

    switch (dimn = n) {
    case 3:
        z = a[2];
    case 2:
        y = a[1];
    case 1:
        x = a[0];
        break;
    default:
        err=Edim;
    }
}

//------------------------------------------------------------------
// IO streams
//------------------------------------------------------------------

// Read input Point format: "(%f)", "(%f, %f)", or "(%f, %f, %f)"
//istream& operator>>( istream& input, Point& P) {
//        char c;
//        input >> c;                // skip '('
//        input >> P.x;
//        input >> c;
//        if (c == ')') {
//                P.setdim(1);       // 1D coord
//                return input;
//        }
//        // else                    // skip ','
//        input >> P.y;
//        input >> c;
//        if (c == ')') {
//                P.setdim(2);       // 2D coord
//                return input;
//        }
//        // else                    // skip ','
//        input >> P.z;
//        P.setdim(3);               // 3D coord
//        input >> c;                // skip ')'
//        return input;
//}

// Write output Point in format: "(%f)", "(%f, %f)", or "(%f, %f, %f)"
//ostream& operator<<( ostream& output, Point P) {
//        switch (P.dim()) {
//        case 1:
//                output << "(" << P.x << ")";
//                break;
//        case 2:
//                output << "(" << P.x << ", " << P.y << ")";
//                break;
//        case 3:
//                output << "(" << P.x << ", " << P.y << ", " << P.z << ")";
//                break;
//        default:
//                output << "Error: P.dim = " << P.dim();
//        }
//        return output;
//}

//------------------------------------------------------------------
// Assign (set) dimension
//------------------------------------------------------------------

int Point::setdim( int n) {
    switch (n) {
    case 1:
        y = 0;
    case 2:
        z = 0;
    case 3:
        return dimn = n;
    default:                      // out of range value
        err = Edim;           // just flag the error
        return -1;
    }
}

//------------------------------------------------------------------
// Comparison (note: dimension must compare)
//------------------------------------------------------------------

int Point::operator==( Point Q) {
    if (dimn != Q.dim()) return 0;

    switch (dimn) {
    case 1:
        return (x==Q.x);
    case 2:
        return (x==Q.x && y==Q.y);
    case 3:
    default:
        return (x==Q.x && y==Q.y && z==Q.z);
    }
}

int Point::operator!=( Point Q) {
    if (dimn != Q.dim()) return 1;

    switch (dimn) {
    case 1:
        return (x!=Q.x);
    case 2:
        return (x!=Q.x || y!=Q.y);
    case 3:
    default:
        return (x!=Q.x || y!=Q.y || z!=Q.z);
    }
}

//------------------------------------------------------------------
// Point Vector Operations
//------------------------------------------------------------------

Vector Point::operator-( Point Q) {      // Vector diff of Points
    Vector v;
    v.x = x - Q.x;
    v.y = y - Q.y;
    v.z = z - Q.z;
    v.dimn = max( dimn, Q.dim());
    return v;
}

Point Point::operator+( Vector v) {      // +ve translation
    Point P;
    P.x = x + v.x;
    P.y = y + v.y;
    P.z = z + v.z;
    P.dimn = max( dimn, v.dim());
    return P;
}

Point Point::operator-( Vector v) {      // -ve translation
    Point P;
    P.x = x - v.x;
    P.y = y - v.y;
    P.z = z - v.z;
    P.dimn = max( dimn, v.dim());
    return P;
}

Point& Point::operator+=( Vector v) {      // +ve translation
    x += v.x;
    y += v.y;
    z += v.z;
    dimn = max( dimn, v.dim());
    return *this;
}

Point& Point::operator-=( Vector v) {      // -ve translation
    x -= v.x;
    y -= v.y;
    z -= v.z;
    dimn = max( dimn, v.dim());
    return *this;
}

//------------------------------------------------------------------
// Point Scalar Operations (convenient but often illegal)
//        are not valid for points in general,
//        unless they are 'affine' as coeffs of
//        a sum in which all the coeffs add to 1,
//        such as: the sum (a*P + b*Q) with (a+b == 1).
//        The programmer must enforce this (if they want to).
//------------------------------------------------------------------

Point operator*( int c, Point Q) {
    Point P;
    P.x = c * Q.x;
    P.y = c * Q.y;
    P.z = c * Q.z;
    P.dimn = Q.dim();
    return P;
}

Point operator*( double c, Point Q) {
    Point P;
    P.x = c * Q.x;
    P.y = c * Q.y;
    P.z = c * Q.z;
    P.dimn = Q.dim();
    return P;
}

Point operator*( Point Q, int c) {
    Point P;
    P.x = c * Q.x;
    P.y = c * Q.y;
    P.z = c * Q.z;
    P.dimn = Q.dim();
    return P;
}

Point operator*( Point Q, double c) {
    Point P;
    P.x = c * Q.x;
    P.y = c * Q.y;
    P.z = c * Q.z;
    P.dimn = Q.dim();
    return P;
}

Point operator/( Point Q, int c) {
    Point P;
    P.x = Q.x / c;
    P.y = Q.y / c;
    P.z = Q.z / c;
    P.dimn = Q.dim();
    return P;
}

Point operator/( Point Q, double c) {
    Point P;
    P.x = Q.x / c;
    P.y = Q.y / c;
    P.z = Q.z / c;
    P.dimn = Q.dim();
    return P;
}

//------------------------------------------------------------------
// Point Addition (also convenient but often illegal)
//    is not valid unless part of an affine sum.
//    The programmer must enforce this (if they want to).
//------------------------------------------------------------------

Point operator+( Point Q, Point R) {
    Point P;
    P.x = Q.x + R.x;
    P.y = Q.y + R.y;
    P.z = Q.z + R.z;
    P.dimn = max( Q.dim(), R.dim());
    return P;
}

//------------------------------------------------------------------
// Affine Sums
// Returns weighted sum, even when not affine, but...
// Tests if coeffs add to 1.  If not, sets: err = Esum.
//------------------------------------------------------------------

Point asum( int n, int c[], Point Q[]) {
    int        maxd = 0;
    int        cs = 0;
    Point      P;

    for (int i=0; i<n; i++) {
        cs += c[i];

        if (Q[i].dim() > maxd)
            maxd = Q[i].dim();
    }

    if (cs != 1)                 // not an affine sum
        P.err = Esum;        // flag error, but compute sum anyway

    for (int i=0; i<n; i++) {
        P.x += c[i] * Q[i].x;
        P.y += c[i] * Q[i].y;
        P.z += c[i] * Q[i].z;
    }

    P.dimn = maxd;
    return P;
}

Point asum( int n, double c[], Point Q[]) {
    int        maxd = 0;
    double     cs = 0.0;
    Point      P;

    for (int i=0; i<n; i++) {
        cs += c[i];

        if (Q[i].dim() > maxd)
            maxd = Q[i].dim();
    }

    if (cs != 1)                 // not an affine sum
        P.err = Esum;        // flag error, but compute sum anyway

    for (int i=0; i<n; i++) {
        P.x += c[i] * Q[i].x;
        P.y += c[i] * Q[i].y;
        P.z += c[i] * Q[i].z;
    }

    P.dimn = maxd;
    return P;
}

//------------------------------------------------------------------
// Distance between Points
//------------------------------------------------------------------

double d( Point P, Point Q) {      // Euclidean distance
    double dx = P.x - Q.x;
    double dy = P.y - Q.y;
    double dz = P.z - Q.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

double d2( Point P, Point Q) {     // squared distance (more efficient)
    double dx = P.x - Q.x;
    double dy = P.y - Q.y;
    double dz = P.z - Q.z;
    return (dx*dx + dy*dy + dz*dz);
}

//------------------------------------------------------------------
// Sidedness of a Point wrt a directed line P1->P2
//        - makes sense in 2D only
//------------------------------------------------------------------

double Point::isLeft( Point P1, Point P2) {
    if (dimn != 2 || P1.dim() != 2 || P2.dim() != 2) {
        err = Edim;        // flag error, but compute anyway
    }

    return ((P1.x - x) * (P2.y - y) - (P2.x - x) * (P1.y - y));
}

//------------------------------------------------------------------
// Error Routines
//------------------------------------------------------------------

const char* Point::errstr() {            // return error string
    switch (err) {
    case Enot:
        return "no error";
    case Edim:
        return "error: invalid dimension for operation";
    case Esum:
        return "error: Point sum is not affine";
    default:
        return "error: unknown err value";
    }
}
