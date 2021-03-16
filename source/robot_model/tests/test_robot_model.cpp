#include "robot_model/Model.hpp"

#include <stdexcept>
#include <gtest/gtest.h>

#include "robot_model/Exceptions/InvalidJointStateSizeException.hpp"
#include "robot_model/Exceptions/FrameNotFoundException.hpp"

using namespace RobotModel;

class RobotModelTest : public testing::Test {
protected:
  void SetUp() override {
    robot_name = "franka";
    urdf = std::string(TEST_FIXTURES) + "panda_arm.urdf";
    franka = Model(robot_name, urdf);
    joint_state = state_representation::JointState(robot_name, 7);
    set_test_configurations();
  }

  Model empty = Model();
  Model franka;
  std::string robot_name;
  std::string urdf;

  state_representation::JointState joint_state;

  double tol = 1e-5;

  // Known test configurations generated by fixtures/generateRobotModelTestConfigurations.m
  std::vector<state_representation::JointState> test_configs;
  std::vector<std::vector<double>> test_gravity_expects;
  std::vector<std::vector<double>> test_coriolis_expects;
  std::vector<std::vector<double>> test_inertia_expects;

  void set_test_configurations() {
    // Random test configuration 1:
    state_representation::JointState config1("robot", 7);
    config1.set_positions({-0.059943, 1.667088, 1.439900, -1.367141, -1.164922, 0.948034, 2.239983});
    config1.set_velocities({-0.106398, 0.631975, -0.803325, 0.719187, -0.944742, 0.798313, 0.799871});
    config1.set_accelerations({0.048212, -0.759601, -0.644412, 0.412215, 0.662720, -0.930332, 0.515677});
    test_configs.push_back(config1);

    // Expected results for configuration 1:
    test_gravity_expects.push_back({-0.000000, -37.179122, 22.968841, -0.658312, 1.120245, 0.576686, 0.060074});
    test_coriolis_expects.push_back({0.337476, 0.366005, -0.389938, -0.858310, 0.017088, -0.173611, 0.001417});
    test_inertia_expects.push_back({-0.554977, -0.609051, 0.115905, 0.355741, -0.022378, -0.035718, 0.008713});

    // Random test configuration 2:
    state_representation::JointState config2("robot", 7);
    config2.set_positions({2.648782, -0.553976, 0.801067, -2.042097, -1.642935, 2.946476, 1.292717});
    config2.set_velocities({-0.442322, 0.164863, -0.157988, -0.815863, -0.951945, -0.017708, -0.443466});
    config2.set_accelerations({-0.320486, -0.425301, -0.658194, -0.201473, 0.395299, -0.592647, 0.332653});
    test_configs.push_back(config2);

    // Expected results for configuration 2:
    test_gravity_expects.push_back({0.000000, 0.342126, -9.938223, 16.752369, 1.208900, -0.346888, 0.073840});
    test_coriolis_expects.push_back({0.536237, -0.632604, 0.070196, -0.034678, -0.018236, 0.143008, 0.001937});
    test_inertia_expects.push_back({-0.554062, 0.089175, -0.760300, -0.002785, -0.007597, -0.082136, 0.009440});

    // Random test configuration 3:
    state_representation::JointState config3("robot", 7);
    config3.set_positions({-0.329909, -0.235174, -1.881858, -2.491807, 0.674615, 0.996670, 0.345810});
    config3.set_velocities({0.889568, 0.428943, 0.358439, 0.918761, 0.550669, 0.215454, 0.896005});
    config3.set_accelerations({-0.880717, -0.462575, 0.973360, 0.544413, -0.049291, 0.361799, -0.166130});
    test_configs.push_back(config3);

    // Expected results for configuration 3:
    test_gravity_expects.push_back({-0.000000, 9.074281, 4.750870, 12.948832, 0.865387, -0.924859, -0.043955});
    test_coriolis_expects.push_back({1.306320, 0.394845, 1.544151, -0.895624, 0.013075, -0.217746, -0.010039});
    test_inertia_expects.push_back({-0.204865, -0.173593, -0.076119, 0.453052, 0.045487, 0.026416, -0.007190});
  }
};

TEST_F(RobotModelTest, TestInitEmptyModel) {
  EXPECT_THROW(empty.init_model(), std::invalid_argument);
}

TEST_F(RobotModelTest, TestSetName) {
  empty.set_robot_name(robot_name);
  EXPECT_EQ(empty.get_robot_name(), robot_name);
}

TEST_F(RobotModelTest, TestSetUrdfPath) {
  empty.set_urdf_path(urdf);
  EXPECT_EQ(empty.get_urdf_path(), urdf);
}

TEST_F(RobotModelTest, TestInitModel) {
  empty.set_robot_name(robot_name);
  empty.set_urdf_path(urdf);
  EXPECT_NO_THROW(empty.init_model());
}

TEST_F(RobotModelTest, TestConstructor) {
  Model tmp(robot_name, urdf);
  EXPECT_NO_THROW(franka = tmp);
}

TEST_F(RobotModelTest, TestNumberOfJoints) {
  EXPECT_EQ(franka.get_number_of_joints(), 7);
}

TEST_F(RobotModelTest, TestForwardGeometryJointStateSize) {
  state_representation::JointState dummy = state_representation::JointState(robot_name, 6);
  EXPECT_THROW(franka.forward_geometry(dummy), Exceptions::InvalidJointStateSizeException);
}

TEST_F(RobotModelTest, TestForwardGeometry) {
  EXPECT_EQ(franka.forward_geometry(joint_state).get_position(),
            franka.forward_geometry(joint_state, "panda_link8").get_position());
}

TEST_F(RobotModelTest, TestForwardGeometryInvalidFrameName) {
  EXPECT_THROW(franka.forward_geometry(joint_state, "panda_link99"), Exceptions::FrameNotFoundException);
}

TEST_F(RobotModelTest, TestJacobianJointNames) {
  state_representation::JointState dummy = state_representation::JointState(robot_name, 7);
  state_representation::Jacobian jac = franka.compute_jacobian(dummy);
  for (int i = 0; i < 7; ++i) {
    std::string jname = "panda_joint" + std::to_string(i + 1);
    EXPECT_TRUE(jname.compare(jac.get_joint_names()[i]) == 0);
  }
}

TEST_F(RobotModelTest, TestJacobianInvalidFrameName) {
  EXPECT_THROW(franka.compute_jacobian(joint_state, "panda_link99"), Exceptions::FrameNotFoundException);
}

TEST_F(RobotModelTest, TestJacobianNbRows) {
  state_representation::Jacobian jac = franka.compute_jacobian(joint_state, "panda_joint2");
  EXPECT_EQ(jac.get_nb_rows(), 6);
}

TEST_F(RobotModelTest, TestJacobianNbCols) {
  state_representation::Jacobian jac = franka.compute_jacobian(joint_state, "panda_joint2");
  EXPECT_EQ(jac.get_nb_cols(), joint_state.get_size());
}

TEST_F(RobotModelTest, TestComputeInertiaMatrix) {
  state_representation::JointPositions jp = state_representation::JointPositions::Random("robot", 7);
  Eigen::MatrixXd inertia = franka.compute_inertia_matrix(jp);
  EXPECT_TRUE(inertia.rows() == jp.get_size() && inertia.cols() == jp.get_size());
  // expect the matrix to be symmetric
  Eigen::MatrixXd upper_part = inertia.triangularView<Eigen::StrictlyUpper>();
  Eigen::MatrixXd lower_part = inertia.triangularView<Eigen::StrictlyLower>();
  EXPECT_TRUE(upper_part.isApprox(lower_part.transpose()));
}

TEST_F(RobotModelTest, TestComputeInertiaTorques) {
  state_representation::JointState js = state_representation::JointState::Random("robot", 7);
  state_representation::JointTorques inertia_torques = franka.compute_inertia_torques(js);
  EXPECT_TRUE(inertia_torques.data().norm() > 0);

  for (std::size_t config = 0; config < test_configs.size(); ++config) {
    inertia_torques = franka.compute_inertia_torques(test_configs[config]);
    for (std::size_t joint = 0; joint < 7; ++joint) {
      EXPECT_NEAR(inertia_torques.get_torques()[joint], test_inertia_expects[config][joint], tol);
    }
  }
}

TEST_F(RobotModelTest, TestComputeCoriolisMatrix) {
  state_representation::JointState js = state_representation::JointState::Random("robot", 7);
  Eigen::MatrixXd coriolis = franka.compute_coriolis_matrix(js);
  EXPECT_TRUE(coriolis.rows() == js.get_size() && coriolis.cols() == js.get_size());
}

TEST_F(RobotModelTest, TestComputeCoriolisTorques) {
  state_representation::JointState js = state_representation::JointState::Random("robot", 7);
  state_representation::JointTorques coriolis_torques = franka.compute_coriolis_torques(js);
  EXPECT_TRUE(coriolis_torques.data().norm() > 0);

  for (std::size_t config = 0; config < test_configs.size(); ++config) {
    coriolis_torques = franka.compute_coriolis_torques(test_configs[config]);
    for (std::size_t joint = 0; joint < 7; ++joint) {
      EXPECT_NEAR(coriolis_torques.get_torques()[joint], test_coriolis_expects[config][joint], tol);
    }
  }
}

TEST_F(RobotModelTest, TestComputeGravityTorques) {
  state_representation::JointPositions jp = state_representation::JointPositions::Random("robot", 7);
  state_representation::JointTorques gravity_torques = franka.compute_gravity_torques(jp);
  EXPECT_TRUE(gravity_torques.data().norm() > 0);

  for (std::size_t config = 0; config < test_configs.size(); ++config) {
    gravity_torques = franka.compute_gravity_torques(test_configs[config]);
    for (std::size_t joint = 0; joint < 7; ++joint) {
      EXPECT_NEAR(gravity_torques.get_torques()[joint], test_gravity_expects[config][joint], tol);
    }
  }
}

TEST_F(RobotModelTest, TestGravityGetterAndSetters) {
  Eigen::Vector3d dummy_vector = Eigen::Vector3d::Random();
  EXPECT_FALSE((dummy_vector - franka.get_gravity_vector()).norm() < 1e-4);
  // set new gravity as dummy_vector and expect equality
  franka.set_gravity_vector(dummy_vector);
  EXPECT_TRUE((dummy_vector - franka.get_gravity_vector()).norm() < 1e-4);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}