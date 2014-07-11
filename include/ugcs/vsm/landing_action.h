// Copyright (c) 2014, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

/**
 * @file landing_action.h
 * Landing action definition.
 */
#ifndef _LANDING_ACTION_H_
#define _LANDING_ACTION_H_

#include <ugcs/vsm/action.h>
#include <ugcs/vsm/coordinates.h>
#include <ugcs/vsm/mavlink.h>

namespace ugcs {
namespace vsm {

/** Land at the specified position. */
class Landing_action: public Action {
    DEFINE_COMMON_CLASS(Landing_action, Action)

public:

    /** Construct the landing action explicitly. */
    Landing_action(Wgs84_position position, double heading, double elevation,
            double descend_rate, double acceptance_radius) :
    Action(Type::LANDING),
    position(position),
    heading(heading),
    elevation(elevation),
    descend_rate(descend_rate),
    acceptance_radius(acceptance_radius) {}

    /**
     * Construct landing action from Mavlink extended mission item.
     *
     * @param item With command equal to mavlink::ugcs::MAV_CMD::MAV_CMD_NAV_LAND_EX
     */
    Landing_action(const mavlink::ugcs::Pld_mission_item_ex& item) :
        Action(Type::LANDING),
        position(Geodetic_tuple(item->x * M_PI / 180.0,
                                item->y * M_PI / 180.0,
                                item->z)),
        heading(item->param4 * M_PI / 180.0),
        elevation(item->elevation),
        descend_rate(item->param3),
        acceptance_radius(item->param2)
    {
        ASSERT(item->command == mavlink::ugcs::MAV_CMD::MAV_CMD_NAV_LAND_EX);
    }


    /** Landing position. The landing phase should start at the specified
     * position, i.e. altitude is an initial altitude for the landing phase. */
    Wgs84_position position;

    /** Heading in radians. */
    double heading;

    /**
     * Elevation in meters (i.e. terrain height) underneath the position.
     */
    double elevation;

    /** Descending rate, m/s. */
    double descend_rate;

    /** Acceptance radius of the position. Maximum distance at which the position
     * is considered reached by the vehicle. Set in meters. */
    double acceptance_radius;
};

/** Type mapper for landing action. */
template<>
struct Action::Mapper<Action::Type::LANDING> {
    /** Real type. */
    typedef Landing_action type;
};

} /* namespace vsm */
} /* namespace ugcs */

#endif /* _LANDING_ACTION_H_ */