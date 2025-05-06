//
// Created by Xymul on 2025/4/30.
//

#ifndef GEO_PROJECTION_H
#define GEO_PROJECTION_H

#include <utility>
#include <cmath>
#include <memory>

#include "geo_gcs.h"

namespace geo {

  namespace details {
    class projection_manager;
  }

  class projection
  {
  public:
    virtual ~projection() = default;

    // first——X——lat
    // second——Y——ldt
    virtual std::pair<double, double> transform_p(double B, double L) = 0;

    virtual std::pair<double, double> transform_i(double X, double Y) = 0;

    virtual details::projection_manager reset_gcs(gcs* p_gcs) = 0;

    virtual const char* name() = 0;
  };

  namespace details {
    class projection_manager
    {
    public:
      projection_manager(projection* p)
        : m_p(p)
      { }

      operator projection*()
      { return m_p.get(); }

      projection* operator->()
      { return m_p.get(); }

    private:
      std::unique_ptr<projection> m_p;
    };
  }

  /* todo: move the template parameter to this class's member */
  template<std::size_t n>
  class gaussian_zone3_projection : public projection
  {
  private:
    gcs* m_gcs;

  public:
    explicit gaussian_zone3_projection(gcs* p_gcs)
      : m_gcs(p_gcs)
    { }

    std::pair<double, double> transform_p(double B, double L) override
    {
      if (m_gcs->euid == -1) {
        throw ;
      }

      double central_meridian = n * 3;

      double B_rad = B * std::numbers::pi / 180.0;
      double L_rad = L * std::numbers::pi / 180.0;
      double cm_rad = central_meridian * std::numbers::pi / 180.0;

      /* FIXME: the first&second eccentricity is not equal to theoretical values, considering to define them as constants in geo::gcs. */

      double e_pow2 = m_gcs->f * 2 - m_gcs->f * m_gcs->f; // power of the first eccentricity
      double e_sec = e_pow2 / (1 - e_pow2); // the second eccentricity
      double ita2 = e_sec * e_sec * std::cos(B_rad) * std::cos(B_rad); // power of ita

      double rho = 180 * 60 * 60 / std::numbers::pi;
      double l = (L - central_meridian) * 3600 / rho;

      double t = std::tan(B_rad);

      double m0 = m_gcs->a * (1 - e_pow2);
      double m2 = 3.0/2 * e_pow2 * m0;
      double m4 = 5.0/4 * e_pow2 * m2;
      double m6 = 7.0/6 * e_pow2 * m4;
      double m8 = 9.0/8 * e_pow2 * m6;

      double a0 = m0 + 1.0/2*m2 + 3.0/8*m4 + 5.0/16*m6 + 35.0/128*m8;
      double a2 = 1.0/2*m2 + 1.0/2*m4 + 15.0/32*m6 + 7.0/16*m8;
      double a4 = 1.0/8*m4 + 3.0/16*m6 + 7.0/32*m8;
      double a6 = 1.0/32*m6 + 1.0/16*m8;
      double a8 = 1.0/128*m8;
      double X = a0 * B_rad - a2/2*std::sin(2*B_rad) + a4/4*std::sin(4*B_rad)-a6/6*std::sin(6*B_rad)+a8/8*std::sin(8*B_rad);

      double m = std::cos(B_rad)*l; // A constant that occurs frequently
      double N = m_gcs->a /std::sqrt(1 - e_pow2*std::sin(B_rad)*std::sin(B_rad));
      double x = X + N/2*t*std::pow(m,2) + N/24*t*(5-std::pow(t,2)+9*ita2+4*ita2*ita2)*std::pow(m,4)
      + N/720*t*(61-58*std::pow(t,2)+std::pow(t,4))*std::pow(m, 6);
      double y = N*m + N/6*(1-t*t+ita2)*std::pow(m, 3) + N/120*(5-18*std::pow(t,2)+std::pow(t,4)+14*ita2-58*ita2*t*t)*std::pow(m, 5);
      y += 500000;

      return {x, y};
    }

    /* NOTE: the deviation of transform_p doesn't quiet affect the result of transform_i */
    std::pair<double, double> transform_i(double X, double Y) override
    {
      double e_pow2 = m_gcs->f * 2 - m_gcs->f * m_gcs->f; // power of the first eccentricity
      double e_sec = e_pow2 / (1 - e_pow2); // the second eccentricity

      double m0 = m_gcs->a * (1 - e_pow2);
      double m2 = 3.0/2 * e_pow2 * m0;
      double m4 = 5.0/4 * e_pow2 * m2;
      double m6 = 7.0/6 * e_pow2 * m4;
      double m8 = 9.0/8 * e_pow2 * m6;

      double a0 = m0 + 1.0/2*m2 + 3.0/8*m4 + 5.0/16*m6 + 35.0/128*m8;
      double a2 = 1.0/2*m2 + 1.0/2*m4 + 15.0/32*m6 + 7.0/16*m8;
      double a4 = 1.0/8*m4 + 3.0/16*m6 + 7.0/32*m8;
      double a6 = 1.0/32*m6 + 1.0/16*m8;
      double a8 = 1.0/128*m8;

      double precision = 0.0000001;

      auto f = [&](double Bf) -> double {
        return -a2/2*std::sin(2*Bf) + a4/4*std::sin(4*Bf) - a6/6*std::sin(6*Bf) + a8/8*std::sin(8*Bf);
      };

      double epos = Y - 500000;

      double Bf_cur = X / a0;
      double Bf_nxt = X / a0;
      double temp = 0;
      do {
        Bf_cur = Bf_nxt;
        temp = f(Bf_cur);
        Bf_nxt = (X - temp) / a0;
      } while (std::fabs(Bf_cur*3600 - Bf_nxt*3600) > precision);
      Bf_cur = Bf_nxt;
      // note: 迭代数值完全一样，或差异极小

      double t = std::tan(Bf_cur);
      double ita2 = e_sec * e_sec * std::cos(Bf_cur) * std::cos(Bf_cur);
      double M = m_gcs->a*(1-e_pow2)/std::pow(std::sqrt(1-e_pow2*std::sin(Bf_cur)*std::sin(Bf_cur)),3);
      double N = m_gcs->a*(1-e_pow2)/std::sqrt(1-e_pow2*std::sin(Bf_cur)*std::sin(Bf_cur));

      double B = Bf_cur - t/(2*M)*epos*(epos/N)*(
        1 - 1.0/12*(5+3*std::pow(t,2) + ita2 - 9*ita2*std::pow(t,2))*std::pow(epos/N,2)
        + 1.0/720*(61 + 90*t*t + 45 * std::pow(t,4))*std::pow(epos/N,4)

        );

      double l = 1.0/std::cos(Bf_cur)*(epos/N)*(
        1-1.0/6*(1+2*std::pow(t,2)+ita2)*std::pow(epos/N,2)
        + 1.0/120*(5+28*std::pow(t,2)+24*std::pow(t,4)+6*ita2+8*ita2*std::pow(t,2))*std::pow(epos/N,4)
        );

      return {B * 180 / std::numbers::pi, n * 3 + l * 180 / std::numbers::pi};
    }

    details::projection_manager reset_gcs(gcs* p_gcs) override
    { return { new gaussian_zone3_projection{p_gcs} }; }

    const char* name() override
    {
      return "GK_ZONE3";
    }
  };
}

#endif //GEO_PROJECTION_H
