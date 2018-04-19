#ifndef BOOST_ALG_HPP
#define BOOST_ALG_HPP

#include <boost/geometry.hpp>
#include "binpack2d.h"

namespace bp2d {

using binpack2d::TCoord;
using Coord = TCoord<binpack2d::PointImpl>;
using binpack2d::PointImpl;
using binpack2d::PolygonImpl;
using binpack2d::PathImpl;
using binpack2d::Orientation;
using binpack2d::OrientationType;
using binpack2d::getX;
using binpack2d::getY;
using binpack2d::setX;
using binpack2d::setY;
using Box = binpack2d::_Box<PointImpl>;
using Segment = binpack2d::_Segment<PointImpl>;

}

namespace boost {
namespace geometry {
namespace traits {

/* ************************************************************************** */
/* Point concept adaptaion ************************************************** */
/* ************************************************************************** */

template<> struct tag<bp2d::PointImpl> {
    using type = point_tag;
};

template<> struct coordinate_type<bp2d::PointImpl> {
    using type = bp2d::Coord;
};

template<> struct coordinate_system<bp2d::PointImpl> {
    using type = cs::cartesian;
};

template<> struct dimension<bp2d::PointImpl>: boost::mpl::int_<2> {};

template<>
struct access<bp2d::PointImpl, 0 > {
    static inline bp2d::Coord get(bp2d::PointImpl const& a) {
        return binpack2d::getX(a);
    }

    static inline void set(bp2d::PointImpl& a,
                           bp2d::Coord const& value) {
        binpack2d::setX(a, value);
    }
};

template<>
struct access<bp2d::PointImpl, 1 > {
    static inline bp2d::Coord get(bp2d::PointImpl const& a) {
        return binpack2d::getY(a);
    }

    static inline void set(bp2d::PointImpl& a,
                           bp2d::Coord const& value) {
        binpack2d::setY(a, value);
    }
};


/* ************************************************************************** */
/* Box concept adaptaion **************************************************** */
/* ************************************************************************** */

template<> struct tag<bp2d::Box> {
    using type = box_tag;
};

template<> struct point_type<bp2d::Box> {
    using type = bp2d::PointImpl;
};

template<> struct indexed_access<bp2d::Box, min_corner, 0> {
    static inline bp2d::Coord get(bp2d::Box const& box) {
        return bp2d::getX(box.minCorner());
    }
    static inline void set(bp2d::Box &box, bp2d::Coord const& coord) {
        bp2d::setX(box.minCorner(), coord);
    }
};

template<> struct indexed_access<bp2d::Box, min_corner, 1> {
    static inline bp2d::Coord get(bp2d::Box const& box) {
        return bp2d::getY(box.minCorner());
    }
    static inline void set(bp2d::Box &box, bp2d::Coord const& coord) {
        bp2d::setY(box.minCorner(), coord);
    }
};

template<> struct indexed_access<bp2d::Box, max_corner, 0> {
    static inline bp2d::Coord get(bp2d::Box const& box) {
        return bp2d::getX(box.maxCorner());
    }
    static inline void set(bp2d::Box &box, bp2d::Coord const& coord) {
        bp2d::setX(box.maxCorner(), coord);
    }
};

template<> struct indexed_access<bp2d::Box, max_corner, 1> {
    static inline bp2d::Coord get(bp2d::Box const& box) {
        return bp2d::getY(box.maxCorner());
    }
    static inline void set(bp2d::Box &box, bp2d::Coord const& coord) {
        bp2d::setY(box.maxCorner(), coord);
    }
};

/* ************************************************************************** */
/* Segement concept adaptaion *********************************************** */
/* ************************************************************************** */

template<> struct tag<bp2d::Segment> {
    using type = segment_tag;
};

template<> struct point_type<bp2d::Segment> {
    using type = bp2d::PointImpl;
};

template<> struct indexed_access<bp2d::Segment, 0, 0> {
    static inline bp2d::Coord get(bp2d::Segment const& seg) {
        return bp2d::getX(seg.first());
    }
    static inline void set(bp2d::Segment &seg, bp2d::Coord const& coord) {
        bp2d::setX(seg.first(), coord);
    }
};

template<> struct indexed_access<bp2d::Segment, 0, 1> {
    static inline bp2d::Coord get(bp2d::Segment const& seg) {
        return bp2d::getY(seg.first());
    }
    static inline void set(bp2d::Segment &seg, bp2d::Coord const& coord) {
        bp2d::setY(seg.first(), coord);
    }
};

template<> struct indexed_access<bp2d::Segment, 1, 0> {
    static inline bp2d::Coord get(bp2d::Segment const& seg) {
        return bp2d::getX(seg.second());
    }
    static inline void set(bp2d::Segment &seg, bp2d::Coord const& coord) {
        bp2d::setX(seg.second(), coord);
    }
};

template<> struct indexed_access<bp2d::Segment, 1, 1> {
    static inline bp2d::Coord get(bp2d::Segment const& seg) {
        return bp2d::getY(seg.second());
    }
    static inline void set(bp2d::Segment &seg, bp2d::Coord const& coord) {
        bp2d::setY(seg.second(), coord);
    }
};


/* ************************************************************************** */
/* Ring concept adaptaion *************************************************** */
/* ************************************************************************** */

// Connversion between binpack2d::Orientation and order_selector ///////////////

template<bp2d::Orientation> struct ToBoostOrienation {};

template<>
struct ToBoostOrienation<bp2d::Orientation::CLOCKWISE> {
    static const order_selector Value = clockwise;
};

template<>
struct ToBoostOrienation<bp2d::Orientation::COUNTER_CLOCKWISE> {
    static const order_selector Value = counterclockwise;
};

static const bp2d::Orientation RealOrientation =
        bp2d::OrientationType<bp2d::PolygonImpl>::Value;

// Ring implementation /////////////////////////////////////////////////////////

// Boost would refer to ClipperLib::Path (alias bp2d::PolygonImpl) as a ring
template<> struct tag<bp2d::PathImpl> {
    using type = ring_tag;
};

template<> struct point_order<bp2d::PathImpl> {
    static const order_selector value =
            ToBoostOrienation<RealOrientation>::Value;
};

// All our Paths should be closed for the bin packing application
template<> struct closure<bp2d::PathImpl> {
    static const closure_selector value = closed;
};

// Polygon implementation //////////////////////////////////////////////////////

template<> struct tag<bp2d::PolygonImpl> {
    using type = polygon_tag;
};

template<> struct exterior_ring<bp2d::PolygonImpl> {
    static inline bp2d::PathImpl& get(bp2d::PolygonImpl& p) {
        return binpack2d::ShapeLike::getContour(p);
    }

    static inline bp2d::PathImpl const& get(bp2d::PolygonImpl const& p) {
        return binpack2d::ShapeLike::getContour(p);
    }
};

template<> struct ring_const_type<bp2d::PolygonImpl> {
    using type = const bp2d::PathImpl;
};

template<> struct ring_mutable_type<bp2d::PolygonImpl> {
    using type = bp2d::PathImpl;
};

template<> struct interior_const_type<bp2d::PolygonImpl> {
   using type = const binpack2d::THolesContainer<bp2d::PolygonImpl>;
};

template<> struct interior_mutable_type<bp2d::PolygonImpl> {
   using type = binpack2d::THolesContainer<bp2d::PolygonImpl>;
};

template<>
struct interior_rings<bp2d::PolygonImpl> {

    static inline binpack2d::THolesContainer<bp2d::PolygonImpl>& get(
            bp2d::PolygonImpl& p)
    {
        return binpack2d::ShapeLike::holes(p);
    }

    static inline const binpack2d::THolesContainer<bp2d::PolygonImpl>& get(
            bp2d::PolygonImpl const& p)
    {
        return binpack2d::ShapeLike::holes(p);
    }
};

}   // traits
}   // geometry

// This is an addition to the ring implementation
template<>
struct range_value<bp2d::PathImpl> {
    using type = bp2d::PointImpl;
};

}   // boost

namespace binpack2d {

// Tell binpack2d how to make string out of a ClipperPolygon object
template<>
inline double PointLike::distance(const PointImpl& p1,
                                  const PointImpl& p2 )
{
    return boost::geometry::distance(p1, p2);
}

template<>
inline double PointLike::distance(const PointImpl& p,
                                  const bp2d::Segment& seg )
{
    return boost::geometry::distance(p, seg);
}

// Tell binpack2d how to make string out of a ClipperPolygon object
template<>
inline bool ShapeLike::intersects(const PathImpl& sh1,
                                  const PathImpl& sh2)
{
    return boost::geometry::intersects(sh1, sh2);
}

// Tell binpack2d how to make string out of a ClipperPolygon object
template<>
inline bool ShapeLike::intersects(const PolygonImpl& sh1,
                                  const PolygonImpl& sh2) {
    return boost::geometry::intersects(sh1, sh2);
}

template<>
inline double ShapeLike::area(const PolygonImpl& shape) {
    return boost::geometry::area(shape);
}

template<>
inline bool ShapeLike::isInside(const PointImpl& point,
                                const PolygonImpl& shape)
{
    return boost::geometry::within(point, shape);
}

template<>
inline bool ShapeLike::isInside(const PolygonImpl& sh1,
                                const PolygonImpl& sh2)
{
    return boost::geometry::within(sh1, sh2);
}

template<>
inline bp2d::Box ShapeLike::boundingBox(const PolygonImpl& sh) {
    bp2d::Box b;
    boost::geometry::envelope(sh, b);
    return b;
}

}



#endif // BOOST_ALG_HPP
