//
// Created by Xymul on 2025/5/3.
//

#ifndef GEO_GCS_H
#define GEO_GCS_H

namespace geo {
  /*
   * Strictly speaking, it represents not a coordinate system such as ECEF or ENU but ellipsoidal parameters.
   *
   */
  struct gcs
  {
    /* name of the Geodetic Coordinate System */
    std::string name;
    /* EPSG code */
    int euid;
    /* The long axis of the ellipsoid */
    double a;
    /* The flattening of the spheroid */
    double f;
  };
}

#endif //GEO_GCS_H
