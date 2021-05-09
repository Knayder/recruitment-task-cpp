#include <iostream>
#include <cmath>
#include <functional>

#define M_PI 3.14159265358979323846

struct vec3 { 
    float x{0.f},y{0.f},z{0.f};
    
    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    void normalize() {
        float c = length();
        x /= c;
        y /= c;
        z /= c;
    }
    
    vec3 operator*(float value) const {
        return vec3{x*value, y*value, z*value};
    }
    
    vec3 operator*(const vec3& other) const {
        return vec3{x * other.x, y * other.y, z * other.z};   
    }
    
    vec3 operator+(const vec3& other) const {
        return vec3{x + other.x, y + other.y, z + other.z};   
    }
    
    vec3 operator-(const vec3& other) const {
        return vec3{x - other.x, y - other.y, z - other.z};   
    }
};

// Implement this function for the following shapes:
struct Sphere {
    vec3 center;
    float radius;
};
// Axis-aligned bounding box. Cube with edges aligned to x,y,z axis.
struct Aabb
{
    vec3 min;
    vec3 max;
};
// Triangular pyramid.
struct Tetrahedron
{
    vec3 points[4];
    
    inline const vec3& getElement(const std::function<const vec3&(const vec3&, const vec3&)>& f) const {
        return f(f(points[0], points[1]), f(points[2], points[3]));
    }
    
    inline const vec3& xMax() const {
        return getElement([](const vec3& a, const vec3& b) -> const vec3& {
            return ((a.x >= b.x) ? a : b);
        });
    }
    inline const vec3& xMin() const {
        return getElement([](const vec3& a, const vec3& b) -> const vec3& {
            return a.x >= b.x ? b : a;
        });
    }
    
    inline const vec3& yMax() const {
        return getElement([](const vec3& a, const vec3& b) -> const vec3& {
            return a.y >= b.y ? a : b;
        });
    }
    inline const vec3& yMin() const {
        return getElement([](const vec3& a, const vec3& b) -> const vec3& {
            return a.y >= b.y ? b : a;
        });
    }
    
    inline const vec3& zMax() const {
        return getElement([](const vec3& a, const vec3& b) -> const vec3& {
            return a.z >= b.z ? a : b;
        });
    }
    inline const vec3& zMin() const {
        return getElement([](const vec3& a, const vec3& b) -> const vec3& {
            return a.z >= b.z ? b : a;
        });
    }
    
};
// Cylinder with hemispherical ends.
struct Capsule
{
    vec3 points[2];
    float radius;
};


template <typename Shape>
vec3 support(vec3 v, Shape s);

template<>
vec3 support(vec3 v, Sphere s) {
    v.normalize();
    return (v * s.radius) + s.center;
}

template<>
vec3 support(vec3 v, Aabb s) {
    return vec3{
        v.x > 0.f ? s.max.x : s.min.x,
        v.y > 0.f ? s.max.y : s.min.y,
        v.z > 0.f ? s.max.z : s.min.z
    };
}

template<>
vec3 support(vec3 v, Tetrahedron s) {
    float x = std::abs(v.x);
    float y = std::abs(v.y);
    float z = std::abs(v.z);
    if(x >= y && x >= z)
        return v.x >= 0 ? s.xMax() : s.xMin();
    else if(y >= x && y >= z)
        return v.y >= 0 ? s.yMax() : s.yMin();
    return v.z >= 0 ? s.zMax() : s.zMin();
}

template<>
vec3 support(vec3 v, Capsule s) {
    v.normalize();
    
    vec3 w = s.points[1] - s.points[0];
    
    float dot = v.x*w.x + v.y*w.y + v.z*w.z;
    float lenSq1 = v.x*v.x + v.y*v.y + v.z*v.z;
    float lenSq2 = w.x*w.x + w.y*w.y + w.z*w.z;
    float angle = std::acos(dot/std::sqrt(lenSq1 * lenSq2)); //angle between v and w
    if(angle <= M_PI/2.f) //90 degree
        return (v * s.radius) + s.points[1];
    return  (v * s.radius) + s.points[0];
}





void draw() {
    char field[100][100];
    for(int y = 0; y<100; ++y) {
        for(int x = 0; x<100; ++x) {
            field[y][x] = ' ';
        }
    }
    
    /*
    Sphere s;
    s.center = vec3{50.f, 50.f, 0.f};
    s.radius = 19.f;
    
    Aabb s;
    s.min = vec3{10.f, 10.f, 0.f};
    s.max = vec3{30.f, 70.f, 0.f};
    
    Tetrahedron s;
    s.points[0] = vec3{20.f, 20.f, 0.f};
    s.points[1] = vec3{40.f, 20.f, 0.f};
    s.points[2] = vec3{30.f, 20.f, 20.f};
    s.points[3] = vec3{30.f, 40.f, 10.f};
    
    
    Capsule s;
    s.radius = 15.f;
    s.points[0] = vec3{20.f, 20.f, 0.f};
    s.points[1] = vec3{40.f, 70.f, 0.f};
    */
    Capsule s;
    s.radius = 15.f;
    s.points[0] = vec3{20.f, 20.f, 0.f};
    s.points[1] = vec3{40.f, 70.f, 0.f};
    for(int i = 0; i<360; ++i) {
        vec3 point = support(
            vec3{std::cos(float(i) * 3.14f / 180.f), std::sin(float(i) * 3.14f / 180.f), 0} * 10.f,
            s
        );
        field[(int)point.y][(int)point.x] = 'o';
    }
    
    for(int y = 0; y<100; ++y) {
        for(int x = 0; x<100; ++x) {
            std::cout << field[y][x] << ' ';
        
        }
        std::cout << '\n';
    }
    
}

int main() {
    draw(); //My test function to draw everything on screen using ascii.
    return 0;
}
