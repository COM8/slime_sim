#include "Species.hpp"

namespace sim {
Species::Species(Rgba&& color, float moveSpeed, float turnSpeed, float sensorAngleDegrees, float sensorOffsetDst, int sensorSize) : color(color),
                                                                                                                                    moveSpeed(moveSpeed),
                                                                                                                                    turnSpeed(turnSpeed),
                                                                                                                                    sensorAngleDegrees(sensorAngleDegrees),
                                                                                                                                    sensorOffsetDst(sensorOffsetDst),
                                                                                                                                    sensorSize(sensorSize) {}

}  // namespace sim