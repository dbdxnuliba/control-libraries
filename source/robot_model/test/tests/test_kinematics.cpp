#include "robot_model/Model.hpp"

#include <stdexcept>
#include <memory>
#include <gtest/gtest.h>

#include "robot_model/exceptions/InvalidJointStateSizeException.hpp"
#include "robot_model/exceptions/FrameNotFoundException.hpp"

using namespace robot_model;

class RobotModelKinematicsTest : public testing::Test {
protected:
  void SetUp() override {
    robot_name = "franka";
    urdf_path = std::string(TEST_FIXTURES) + "panda_arm.urdf";
    franka = std::make_unique<Model>(robot_name, urdf_path);
    joint_state = state_representation::JointState(robot_name, 7);
  }

  std::unique_ptr<Model> franka;
  std::string robot_name;
  std::string urdf_path;

  state_representation::JointState joint_state;

  double tol = 1e-5;

  // Known test configurations generated by fixtures/generateRobotModelKinematicsTestConfigurations.m
  std::vector<state_representation::JointState> test_configs;
  std::vector<state_representation::CartesianPose> test_fk_ee_expects;
  std::vector<state_representation::CartesianPose> test_fk_link4_expects;
  std::vector<state_representation::CartesianTwist> test_velocity_fk_expects;
  Eigen::Matrix<double, 6, 1> twist;

  void setTestConfigurations() {
    // Random test configuration 1:
    state_representation::JointState config1("robot", 7);
    config1.set_positions({2.747925, 0.249748, 2.879048, -1.410068, 0.089576, 1.229171, -0.405612});
    config1.set_velocities({-0.016387, -0.857926, 0.775478, -0.870733, -0.127630, 0.653259, -0.210931});
    test_configs.push_back(config1);

    // Expected results for configuration 1:
    test_fk_ee_expects.emplace_back(state_representation::CartesianPose("ee",
                                                                        Eigen::Vector3d(0.324032, -0.267293, 0.784635),
                                                                        Eigen::Quaterniond(0.048245,
                                                                                           -0.991201,
                                                                                           0.105526,
                                                                                           -0.063693)));
    test_fk_link4_expects.emplace_back(state_representation::CartesianPose("link4",
                                                                           Eigen::Vector3d(-0.009046,
                                                                                           -0.019428,
                                                                                           0.658888),
                                                                           Eigen::Quaterniond(0.420653,
                                                                                              0.704906,
                                                                                              0.187567,
                                                                                              -0.539423)));
    twist << 0.184588, 1.085958, 0.853861, 0.727804, 0.063256, -0.623188;
    test_velocity_fk_expects.emplace_back(state_representation::CartesianTwist("ee", twist));

    // Random test configuration 2:
    state_representation::JointState config2("robot", 7);
    config2.set_positions({0.657542, 1.123400, 2.238078, -0.276603, -1.791779, 0.957355, 2.305472});
    config2.set_velocities({0.186724, 0.007680, 0.225619, 0.638844, 0.063778, -0.595850, -0.092213});
    test_configs.push_back(config2);

    // Expected results for configuration 2:
    test_fk_ee_expects.emplace_back(state_representation::CartesianPose("ee",
                                                                        Eigen::Vector3d(0.446927, 0.531577, 0.597279),
                                                                        Eigen::Quaterniond(0.009740,
                                                                                           0.817399,
                                                                                           -0.561707,
                                                                                           -0.127470)));
    test_fk_link4_expects.emplace_back(state_representation::CartesianPose("link4",
                                                                           Eigen::Vector3d(0.168407,
                                                                                           0.211915,
                                                                                           0.515738),
                                                                           Eigen::Quaterniond(0.146989,
                                                                                              -0.219352,
                                                                                              -0.897765,
                                                                                              -0.352558)));
    twist << -0.376099, 0.865864, 0.108224, 0.016221, -0.080835, -0.171010;
    test_velocity_fk_expects.emplace_back(state_representation::CartesianTwist("ee", twist));

    // Random test configuration 3:
    state_representation::JointState config3("robot", 7);
    config3.set_positions({-0.417727, 1.643116, 0.695671, -0.984239, 1.275766, 1.290295, 0.098453});
    config3.set_velocities({0.113389, -0.687010, 0.124112, 0.389607, -0.147089, 0.672541, 0.462774});
    test_configs.push_back(config3);

    // Expected results for configuration 3:
    test_fk_ee_expects.emplace_back(state_representation::CartesianPose("ee",
                                                                        Eigen::Vector3d(0.675670, 0.098818, 0.043896),
                                                                        Eigen::Quaterniond(0.819766,
                                                                                           -0.371660,
                                                                                           0.158158,
                                                                                           0.406004)));
    test_fk_link4_expects.emplace_back(state_representation::CartesianPose("link4",
                                                                           Eigen::Vector3d(0.305341,
                                                                                           -0.077677,
                                                                                           0.247004),
                                                                           Eigen::Quaterniond(0.003074,
                                                                                              0.422247,
                                                                                              0.800825,
                                                                                              -0.424709)));
    twist << -0.906095, -0.344373, -0.092142, 0.322764, -0.096389, 0.429407;
    test_velocity_fk_expects.emplace_back(state_representation::CartesianTwist("ee", twist));
  }
};

TEST_F(RobotModelKinematicsTest, TestForwardGeometryJointStateSize) {
  state_representation::JointState dummy = state_representation::JointState(robot_name, 6);
  EXPECT_THROW(franka->forward_geometry(dummy), exceptions::InvalidJointStateSizeException);
}

TEST_F(RobotModelKinematicsTest, TestForwardGeometryEE) {
  EXPECT_EQ(franka->forward_geometry(joint_state).get_position(),
            franka->forward_geometry(joint_state, "panda_link8").get_position());
}

TEST_F(RobotModelKinematicsTest, TestForwardGeometryInvalidFrameName) {
  EXPECT_THROW(franka->forward_geometry(joint_state, "panda_link99"), exceptions::FrameNotFoundException);
}

TEST_F(RobotModelKinematicsTest, TestForwardGeometry) {
  for (std::size_t config = 0; config < test_configs.size(); ++config) {
    state_representation::CartesianPose ee_pose = franka->forward_geometry(test_configs[config]);
    EXPECT_TRUE(ee_pose.get_position().isApprox(test_fk_ee_expects.at(config).get_position()));
    EXPECT_TRUE(ee_pose.get_orientation().isApprox(test_fk_ee_expects.at(config).get_orientation()));
    state_representation::CartesianPose link4_pose = franka->forward_geometry(test_configs[config], "panda_link4");
    EXPECT_TRUE(link4_pose.get_position().isApprox(test_fk_link4_expects.at(config).get_position()));
    EXPECT_TRUE(link4_pose.get_orientation().isApprox(test_fk_link4_expects.at(config).get_orientation()));
  }
}

TEST_F(RobotModelKinematicsTest, TestForwardKinematics) {
  for (std::size_t config = 0; config < test_configs.size(); ++config) {
    state_representation::CartesianTwist ee_twist = franka->forward_kinematic(test_configs[config]);
    EXPECT_TRUE(ee_twist.get_linear_velocity().isApprox(test_velocity_fk_expects.at(config).get_linear_velocity()));
    EXPECT_TRUE(ee_twist.get_angular_velocity().isApprox(test_velocity_fk_expects.at(config).get_angular_velocity()));
  }
}

TEST_F(RobotModelKinematicsTest, TestInverseKinematics) {
  for (std::size_t config = 0; config < test_configs.size(); ++config) {
    state_representation::CartesianTwist ee_twist = franka->forward_kinematic(test_configs[config]);
    state_representation::JointVelocities joint_twist = franka->inverse_kinematic(test_configs[config], ee_twist);
    EXPECT_TRUE(joint_twist.get_velocities().isApprox(test_configs[config].get_velocities()));
  }
}