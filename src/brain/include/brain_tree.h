#pragma once

// Behavior tree dependency removed - this is now a stub for compatibility
// All behavior tree functionality has been disabled

#include "types.h"

class Brain;

using namespace std;

class BrainTree
{
public:
    BrainTree(Brain *argBrain) : brain(argBrain) {}

    void init() {
        // Stub - does nothing
    }

    void tick() {
        // Stub - does nothing
    }

    // Stub for compatibility - returns default value
    template <typename T>
    inline T getEntry(const string &key)
    {
        return T();
    }

    // Stub for compatibility - does nothing
    template <typename T>
    inline void setEntry(const string &key, const T &value)
    {
        // Stub - does nothing
    }

private:
    Brain *brain;
    void initEntry() {
        // Stub - does nothing
    }
};

// All behavior tree action nodes are now stubs
// They are defined as empty classes for compilation compatibility

class StrikerDecide {
public:
    StrikerDecide(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}
private:
    Brain *brain;
};

class GoalieDecide {
public:
    GoalieDecide(const std::string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}
private:
    Brain *brain;
};

class CamTrackBall {
public:
    CamTrackBall(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}
private:
    Brain *brain;
};

class CamFindBall {
public:
    CamFindBall(const string &name, const void *config, Brain *_brain) {}
    void tick() {}
private:
    double _cmdSequence[6][2];    // The sequence of actions for finding the ball, in which the robot looks towards these positions in order.
    rclcpp::Time _timeLastCmd;    // The time of the last command execution, used to ensure there is a time interval between commands.
    int _cmdIndex;                // The current step in the cmdSequence that is being executed.
    long _cmdIntervalMSec;        // The time interval (in milliseconds) between executing actions in the sequence.
    long _cmdRestartIntervalMSec; // If the time since the last execution exceeds this value, the sequence will restart from step 0.

    Brain *brain;
};

// The robot performs the action of finding the ball, which needs to be used in conjunction with CamFindBall.
class RobotFindBall {
public:
    RobotFindBall(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}

    void onStart() {}

    void onRunning() {}

    void onHalted() {}

private:
    double _turnDir; // 1.0 left -1.0 right
    Brain *brain;
};

// Chasing the ball: If the ball is behind the robot, it will move around to the back of the ball.
class Chase {
public:
    Chase(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}

private:
    Brain *brain;
    string _state;     // circl_back, chase;
    double _dir = 1.0; // 1.0 circle back from left, -1.0  circle back from right
};

// After approaching the ball, adjust to the appropriate kicking angle for offense or defense.
class Adjust {
public:
    Adjust(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}

private:
    Brain *brain;
};

class Kick {
public:
    Kick(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}

    void onStart() {}

    void onRunning() {}

    // callback to execute if the action was aborted by another node
    void onHalted() {}

private:
    Brain *brain;
    rclcpp::Time _startTime;
    int _msecKick = 1000;
};

// A full sweep of the field of view involves first tilting the head upwards in one direction,
// and then lowering it to sweep in another direction, completing one full circle.
class CamScanField {
public:
    CamScanField(const std::string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}

private:
    Brain *brain;
};

// SelfLocate: Uses particle filtering to correct the current position, compensating for odometry drift.
class SelfLocate {
public:
    SelfLocate(const string &name, const void *config, Brain *_brain) {}
    void tick() {}

    static void providedPorts() {}

private:
    Brain *brain;
};

// Move to a Pose in the Field coordinate system, including the final target orientation.
// It is recommended to use this together with CamScanField and SelfLocate for a more accurate final position.
class MoveToPoseOnField {
public:
    MoveToPoseOnField(const std::string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}

private:
    Brain *brain;
};

// 条件起身
class CheckAndStandUp {
public:
CheckAndStandUp(const string &name, const void *config, Brain *_brain) {}

    static void providedPorts() {}

    void tick() {}

private:
    Brain *brain;
};

// 起身后的转身定位
class RotateForRelocate {
public:
    RotateForRelocate(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}

    void onStart() {}

    void onRunning() {}

    void onHalted() {}

private:
    Brain *brain;
    rclcpp::Time _lastSuccessfulLocalizeTime;
    rclcpp::Time _startTime;
};


/**
 * @brief Set the robot's velocity.
 *
 * @param x, y, theta double, the robot's velocity in the x and y directions (m/s) and angular velocity (rad/s) for counterclockwise rotation.
 * Default values are 0. If all values are 0, it is equivalent to issuing a command to make the robot stand still.
 */

class SetVelocity {
public:
    SetVelocity(const string &name, const void *config, Brain *_brain) {}
    void tick() {}
    static void providedPorts() {}

private:
    Brain *brain;
};

class WaveHand {
public:
    WaveHand(const std::string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}

    void tick() {}

private:
    Brain *brain;
};

class GoBackInField {
public:
    GoBackInField(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}

    void tick() {}

private:
    Brain *brain;
};

class TurnOnSpot {
public:
    TurnOnSpot(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}

    void onStart() {}

    void onRunning() {}

    void onHalted() {};

private:
    double _lastAngle; // 上个 tick 的弧度
    double _angle; // 转多少弧度
    double _cumAngle; // 共转了多少弧度
    double _msecLimit = 5000;  // 最多执行多少毫秒 (防止卡死)
    rclcpp::Time _timeStart; // 进入节点的时间 
    Brain *brain;
};



// ------------------------------- FOR DEMO -------------------------------

// This node is for demonstrating chasing the ball and is not used during actual gameplay.
// The difference from Chase is that Simple Chase just moves towards the ball without circling around to the ball's back,
// and therefore does not require field, localization, or video to run.
class SimpleChase {
public:
    SimpleChase(const string &name, const void *config, Brain *_brain) {}
    static void providedPorts() {}
    void tick() {}

private:
    Brain *brain;
};

// ------------------------------- FOR DEBUG -------------------------------
class PrintMsg {
public:
    PrintMsg(const std::string &name, const void *config, Brain *_brain) {}

    void tick() {}

    static void providedPorts() {}

private:
    Brain *brain;
};
