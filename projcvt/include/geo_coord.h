//
// Created by Xymul on 2025/4/30.
//

#ifndef GEO_COORD_H
#define GEO_COORD_H

#include "geo_projection.h"
#include "geo_gcs.h"

namespace geo {
  namespace details {
    struct coord_dms
    {
      int degree = 0;
      int minute = 0;
      int second = 0;
    };

    double to_degrees(const coord_dms& dms)
    {
      return dms.degree/3600.0 + dms.minute/60.0 + dms.second;
    }

    coord_dms to_dms(double degrees)
    {
      coord_dms ret;
      ret.degree = static_cast<int>(degrees);
      double temp = degrees - ret.degree;
      ret.minute = static_cast<int>(temp * 60);
      temp -= ret.minute / 60.0;
      ret.second = static_cast<int>(temp * 3600);
      return ret;
    }
  }

  class coordination {
  public:
    constexpr static int EASTERN = 0b0001;
    constexpr static int WESTERN = 0b0010;
    constexpr static int SOUTHERN = 0b0100;
    constexpr static int NORTHERN = 0b1000;

    explicit coordination() = default;

    explicit coordination(double longitude, double latitude, int hemisphere = EASTERN | NORTHERN)
      : m_lat(latitude), m_ldt(longitude), m_hemisphere(hemisphere)
    { }

    double longitude() const
    { return m_ldt; }

    double latitude() const
    { return m_lat; }

    void set_longitude(double ldt)
    { m_ldt = ldt; }

    void set_latitude(double lat)
    { m_lat = lat; }

    const details::coord_dms longitude_dms() const
    { return details::to_dms(m_ldt); }

    const details::coord_dms latitude_dms() const
    { return details::to_dms(m_lat); }

    int hemisphere() const
    { return m_hemisphere; }

    void set_hemisphere(int hemisphere)
    { m_hemisphere = hemisphere; }

  private:
    /* longitude & latitude of a geographic coordination, degrees as unit, not radians */
    double m_lat; // 纬度
    double m_ldt; // 经度
    int m_hemisphere = 0;

  };

  class projected_coord
  {
  public:
    explicit projected_coord(const coordination& coord, projection* proj)
    {
      auto [x, y] = proj->transform_p(coord.latitude(), coord.longitude());
      X = x;
      Y = y;
      m_projection = proj;
    }

    void transform_projection(projection* proj)
    {
      auto bl = m_projection->transform_i(X, Y);
      auto xy = proj->transform_p(bl.first, bl.second);

      X = xy.first;
      Y = xy.second;

      this->m_projection = proj;
    }

    double x()
    { return X; }

    double y()
    { return Y; }

  private:
    double X;
    double Y;
    projection* m_projection = nullptr;
    gcs* m_gcs = nullptr;
  };
}

#endif //GEO_COORD_H
