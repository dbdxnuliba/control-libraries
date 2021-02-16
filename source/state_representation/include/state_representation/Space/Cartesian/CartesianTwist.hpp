/**
 * @author Baptiste Busch
 * @date 2019/06/07
 */

#pragma once

#include "state_representation/Space/Cartesian/CartesianPose.hpp"
#include "state_representation/Space/Cartesian/CartesianState.hpp"

namespace StateRepresentation {
class CartesianPose;

/**
 * @class CartesianTwist
 * @brief Class to define twist in cartesian space as 3D linear and angular velocity vectors
 */
class CartesianTwist : public CartesianState {
public:
  /**
   * Empty constructor
   */
  explicit CartesianTwist();

  /**
   * @brief Empty constructor for a CartesianTwist
   */
  explicit CartesianTwist(const std::string& name, const std::string& reference = "world");

  /**
   * @brief Copy constructor
   */
  CartesianTwist(const CartesianTwist& twist);

  /**
   * @brief Copy constructor from a CartesianState
   */
  CartesianTwist(const CartesianState& state);

  /**
   * @brief Copy constructor from a CartesianPose by considering that it is equivalent to dividing the pose by 1 second
   */
  CartesianTwist(const CartesianPose& pose);

  /**
   * @brief Construct a CartesianTwist from a linear_velocity given as a vector of coordinates for the linear velocity.
   */
  explicit CartesianTwist(const std::string& name, const Eigen::Vector3d& linear_velocity, const std::string& reference = "world");

  /**
   * @brief Construct a CartesianTwist from a linear_velocity given as a vector of coordinates and a quaternion.
   */
  explicit CartesianTwist(const std::string& name, const Eigen::Vector3d& linear_velocity, const Eigen::Vector3d& angular_velocity, const std::string& reference = "world");

  /**
   * @brief Construct a CartesianTwist from a single 6d twist vector
   */
  explicit CartesianTwist(const std::string& name, const Eigen::Matrix<double, 6, 1>& twist, const std::string& reference = "world");

  /**
   * @brief Constructor for the zero twist
   * @param name the name of the state
   * @param the name of the reference frame
   * @return CartesianTwist with zero values
   */
  static CartesianTwist Zero(const std::string& name, const std::string& reference = "world");

  /**
   * @brief Constructor for a random twist
   * @param name the name of the state
   * @param the name of the reference frame
   * @return CartesianTwist random twist
   */
  static CartesianTwist Random(const std::string& name, const std::string& reference = "world");

  /**
   * @brief Copy assignement operator that have to be defined to the custom assignement operator
   * @param twist the twist with value to assign
   * @return reference to the current twist with new values
   */
  CartesianTwist& operator=(const CartesianTwist& twist);

  /**
   * @brief Overload the = operator from a CartesianState
   * @param state CartesianState to get velocity from
   */
  CartesianTwist& operator=(const CartesianState& state);

  /**
   * @brief Overload the *= operator
   * @param twist CartesianTwist to multiply with
   * @return the current CartesianTwist multiplied by the CartesianTwist given in argument
   */
  CartesianTwist& operator*=(const CartesianTwist& twist);

  /**
   * @brief Overload the * operator with a twist
   * @param twist CartesianTwist to multiply with
   * @return the current CartesianTwist multiplied by the CartesianTwist given in argument
   */
  CartesianTwist operator*(const CartesianTwist& twist) const;

  /**
   * @brief Overload the += operator
   * @param twist CartesianTwist to add to
   * @return the current CartesianTwist added the CartesianTwist given in argument
   */
  CartesianTwist& operator+=(const CartesianTwist& twist);

  /**
   * @brief Overload the + operator with a twist
   * @param twist CartesianTwist to add to
   * @return the current CartesianTwist added the CartesianTwist given in argument
   */
  CartesianTwist operator+(const CartesianTwist& twist) const;

  /**
   * @brief Overload the -= operator
   * @param twist CartesianTwist to substract
   * @return the current CartesianTwist minus the CartesianTwist given in argument
   */
  CartesianTwist& operator-=(const CartesianTwist& twist);

  /**
   * @brief Overload the - operator with a twist
   * @param twist CartesianTwist to substract
   * @return the current CartesianTwist minus the CartesianTwist given in argument
   */
  CartesianTwist operator-(const CartesianTwist& twist) const;

  /**
   * @brief Overload the *= operator with a scalar
   * @param lambda the scalar to multiply with
   * @return the CartesianTwist multiplied by lambda
   */
  CartesianTwist& operator*=(double lambda);

  /**
   * @brief Overload the * operator with a scalar
   * @param lambda the scalar to multiply with
   * @return the CartesianState multiplied by lambda
   */
  CartesianTwist operator*(double lambda) const;

  /**
   * @brief Overload the *= operator with a gain matrix
   * @param lambda the matrix to multiply with
   * @return the CartesianTwist multiplied by lambda
   */
  CartesianTwist& operator*=(const Eigen::Matrix<double, 6, 6>& lambda);

  /**
   * @brief Overload the * operator with a time period
   * @param dt the time period to multiply with
   * @return the CartesianPose corresponding to the displacement over the time period
   */
  CartesianPose operator*(const std::chrono::nanoseconds& dt) const;

  /**
   * @brief Clamp inplace the magnitude of the twist to the values in argument
   * @param max_linear the maximum magnitude of the linear velocity
   * @param max_angular the maximum magnitude of the angular velocity
   * @param linear_noise_ratio if provided, this value will be used to apply a deadzone under which
   * the linear velocity will be set to 0
   * @param angular_noise_ratio if provided, this value will be used to apply a deadzone under which
   * the angular velocity will be set to 0
   */
  void clamp(double max_linear, double max_angular, double linear_noise_ratio = 0, double angular_noise_ratio = 0);

  /**
   * @brief Return the clamped twist
   * @param max_linear the maximum magnitude of the linear velocity
   * @param max_angular the maximum magnitude of the angular velocity
   * @param noise_ratio if provided, this value will be used to apply a deadzone under which
   * the linear velocity will be set to 0
   * @param angular_noise_ratio if provided, this value will be used to apply a deadzone under which
   * the angular velocity will be set to 0
   * @return the clamped twist
   */
  CartesianTwist clamped(double max_linear, double max_angular, double noise_ratio = 0, double angular_noise_ratio = 0) const;

  /**
   * @brief Return a copy of the CartesianTwist
   * @return the copy
   */
  CartesianTwist copy() const;

  /**
   * @brief Return the value of the 6D twist as Eigen array
   * @retrun the Eigen array representing the twist
   */
  Eigen::Array<double, 6, 1> array() const;

  /**
   * @brief Overload the ostream operator for printing
   * @param os the ostream to happend the string representing the CartesianTwist to
   * @param CartesianTwist the CartesianTwist to print
   * @return the appended ostream
   */
  friend std::ostream& operator<<(std::ostream& os, const CartesianTwist& twist);

  /**
   * @brief Overload the * operator with a scalar
   * @param lambda the scalar to multiply with
   * @return the CartesianTwist provided multiplied by lambda
   */
  friend CartesianTwist operator*(double lambda, const CartesianTwist& twist);

  /**
   * @brief Overload the * operator with a gain matrix
   * @param lambda the matrix to multiply with
   * @return the CartesianTwist provided multiplied by lambda
   */
  friend CartesianTwist operator*(const Eigen::Matrix<double, 6, 6>& lambda, const CartesianTwist& twist);

  /**
   * @brief Overload the * operator with a time period
   * @param dt the time period to multiply with
   * @return the CartesianPose corresponding to the displacement over the time period
   */
  friend CartesianPose operator*(const std::chrono::nanoseconds& dt, const CartesianTwist& twist);
};

inline CartesianTwist& CartesianTwist::operator=(const CartesianTwist& twist) {
  CartesianState::operator=(twist);
  return (*this);
}
}// namespace StateRepresentation