#include <cmath>

#include "brain.h"
#include "robot_client.h"

#include "utils/math.h"
#include "utils/print.h"
#include "utils/misc.h"

#include "booster_msgs/message_utils.h"

void RobotClient::init()
{
    publisher = brain->create_publisher<booster_msgs::msg::RpcReqMsg>("LocoApiTopicReq", 10);
}

int RobotClient::moveHead(double pitch, double yaw)
{
    yaw = cap(yaw, brain->config->headYawLimitLeft, brain->config->headYawLimitRight);
    pitch = max(pitch, brain->config->headPitchLimitUp);

    booster_msgs::msg::RpcReqMsg msg = booster_msgs::CreateRotateHeadMsg(pitch, yaw);
    publisher->publish(msg);
    return 0;
}

int RobotClient::waveHand(bool doWaveHand)
{
    auto msg = booster_msgs::CreateWaveHandMsg(booster::robot::b1::HandIndex::kRightHand, doWaveHand ? booster::robot::b1::HandAction::kHandOpen : booster::robot::b1::HandAction::kHandClose);
    publisher->publish(msg);
    return 0;
}

int RobotClient::standUp()
{
    auto msg = booster_msgs::CreateGetUpMsg();
    publisher->publish(msg);
    return 0;
}

int RobotClient::enterDamping()
{
    auto msg = booster_msgs::CreateChangeModeMsg(booster::robot::RobotMode::kDamping);
    publisher->publish(msg);
    return 0;
}

int RobotClient::kickBall(double kick_speed, double kick_dir)
{
    setVelocity(0.0, 0.0, 0.0, false, false, false); // stop the robot before kicking
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // wait for the stop command to take effect

    auto _kick_speed = cap(kick_speed, 3.0, 1.0);
    auto _kick_dir = cap(kick_dir, -0.5, 0.5);
    booster_internal::robot::b1::RLKickBallParameter kick_param(_kick_speed, _kick_dir);
    std::string param = kick_param.ToJson().dump();

    std::string uuid = gen_uuid();
    auto message = booster_msgs::msg::RpcReqMsg();
    message.uuid = uuid;

    nlohmann::json req_header;
    req_header["api_id"] = static_cast<int64_t>(booster_internal::robot::b1::LocoInternalApiId::kRLKickBall);
    message.header = req_header.dump();
    message.body = param;

    publisher->publish(message);

    return 0;
}

int RobotClient::setVelocity(double x, double y, double theta, bool applyMinX, bool applyMinY, bool applyMinTheta)
{
    brain->log->setTimeNow();
    brain->log->log("RobotClient/setVelocity_in",
                    rerun::TextLog(format("vx: %.2f  vy: %.2f  vtheta: %.2f", x, y, theta)));

    rerun::Collection<rerun::Vec2D> vxLine = {{0, 0}, {x, 0}};
    rerun::Collection<rerun::Vec2D> vyLine = {{0, 0}, {0, -y}};
    rerun::Collection<rerun::Vec2D> vthetaLine = {{0, 0}, {2.0 * cos(-theta), 2.0 * sin(-theta)}};

    brain->log->log("robotframe/velocity",
                    rerun::LineStrips2D({vxLine, vyLine, vthetaLine})
                        .with_colors({0xFF0000FF, 0x00FF00FF, 0x0000FFFF})
                        .with_radii({0.05, 0.05, 0.02})
                        .with_draw_order(1.0));


    double minx = 0.05, miny = 0.08, mintheta = 0.05;
    if (applyMinX && fabs(x) < minx && fabs(x) > 1e-5)
        x = x > 0 ? minx : -minx;
    if (applyMinY && fabs(y) < miny && fabs(y) > 1e-5)
        y = y > 0 ? miny : -miny;
    if (applyMinTheta && fabs(theta) < mintheta && fabs(theta) > 1e-5)
        theta = theta > 0 ? mintheta : -mintheta;
    x = cap(x, brain->config->vxLimit, -brain->config->vxLimit);
    y = cap(y, brain->config->vyLimit, -brain->config->vyLimit);
    theta = cap(theta, brain->config->vthetaLimit, -brain->config->vthetaLimit);

    brain->log->log("RobotClient/setVelocity_out",
                    rerun::TextLog(format("vx: %.2f  vy: %.2f  vtheta: %.2f", x, y, theta)));

    auto msg = booster_msgs::CreateMoveMsg(x, y, theta);
    publisher->publish(msg);
    return 0;
}

int RobotClient::moveToPoseOnField(double tx, double ty, double ttheta, double longRangeThreshold, double turnThreshold, double vxLimit, double vyLimit, double vthetaLimit, double xTolerance, double yTolerance, double thetaTolerance)
{
    Pose2D target_f, target_r; 
    target_f.x = tx;
    target_f.y = ty;
    target_f.theta = ttheta;
    target_r = brain->data->field2robot(target_f);
    double targetAngle = atan2(target_r.y, target_r.x);
    double targetDist = norm(target_r.x, target_r.y);

    double vx, vy, vtheta;

    if (
        (fabs(brain->data->robotPoseToField.x - target_f.x) < xTolerance) && (fabs(brain->data->robotPoseToField.y - target_f.y) < yTolerance) && (fabs(toPInPI(brain->data->robotPoseToField.theta - target_f.theta)) < thetaTolerance))
    {
        return setVelocity(0, 0, 0);
    }

    static double breakOscillate = 0.0;
    if (targetDist > longRangeThreshold - breakOscillate)
    {
        breakOscillate = 0.5;

        if (fabs(targetAngle) > turnThreshold)
        {
            vtheta = cap(targetAngle, vthetaLimit, -vthetaLimit);
            return setVelocity(0, 0, vtheta);
        }

        // else

        vx = cap(target_r.x, vxLimit, -vxLimit);
        vtheta = cap(targetAngle, vthetaLimit, -vthetaLimit);
        return setVelocity(vx, 0, vtheta, false, false, false);
    }

    breakOscillate = 0.0;
    vx = cap(target_r.x, vxLimit, -vxLimit);
    vy = cap(target_r.y, vyLimit, -vyLimit);
    vtheta = cap(target_r.theta, vthetaLimit, -vthetaLimit);
    return setVelocity(vx, vy, vtheta);
}
