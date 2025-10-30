#pragma once
#include <array>
#include <vector>

// 3D vector, templated for a type T (e.g., double, float)
template <typename T>
class Vector3D {
 public:
  // constructor - takes values for the 3 components and fills
  // the array m_data with them.
  Vector3D(T x = 0.0, T y = 0.0, T z = 0.0);
  // given an integer dim, this getter function returns the
  // value of m_data in the respective component.
  // You may want to verify that 0<=dim<=2 for debugging purposes.
  const T get(int dim) const;
  // given an integer dim, this setter function fills the
  // respective component of m_data with the given value
  // new_val.
  void set(int dim, T new_val);
  // length of this vector
  T length() const;
  // assignment operator - given another vector other,
  // this function copies the respective values from other into
  // this m_data.
  void operator=(const Vector3D<T>& other);
  // inplace addition operator (element-wise addition of
  // another vector into this)
  void operator+=(const Vector3D<T>& other);
  // inplace subtraction operator (element-wise subraction of
  // another vector into this)
  void operator-=(const Vector3D<T>& other);
  // inplace multiplication operator (scalar multiplication
  // of each vector component in m_data)
  void operator*=(const T factor);
  // multiplication operator - creates a new(!) vector
  // where each element of m_data has been multiplied by a
  // factor
  Vector3D<T> operator*(const T factor) const;
  // subtraction (creates a new vector with the component-wise
  // difference)
  Vector3D<T> operator-(const Vector3D<T>& other) const;
  // set all components of this vector to zero
  void set_zero();
  // compare this vector to another (exact)
  bool operator==(const Vector3D<T>& other) const;
  // compare this vector to another (with given tolerance for
  // maximum absolute difference)
  bool approx(const Vector3D<T>& other, T tol) const;

 private:
  // the three components of our vector
  std::array<T, 3> m_data;
};

// specify specializations of the template
template class Vector3D<float>;
template class Vector3D<double>;
template class Vector3D<std::size_t>;
