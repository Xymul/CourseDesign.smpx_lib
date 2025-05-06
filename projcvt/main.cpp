#include<iostream>
#include <cmath>
#include <iomanip>

#include "include/geo_coord.h"
#include "include/geo_gcs.h"

#define PI 3.141592653589793238463
#define P0 206264.8062470963551564

const double e = 0.00669438002290;
const double e1 = 0.00673949677548;
const double b = 6356752.3141;
const double a = 6378137.0;

double xx = 0;
double yy = 0;

using namespace std;

double L_center = 120;
void DadiPoint2ProjectPoint(double B, double L) {
  int L_num = (int)((L + 1.5)/3);
  B = B * PI / 180;
  L = L * PI / 180;
  double N, t, n, c, V, Xz, m1, m2, m3, m4, m5, m6, a0, a2, a4, a6, a8, M0, M2, M4, M6, M8, x0, y0, l;
  //double L_center;

  //L_center = 3 * L_num;
  l = (L / PI * 180 - L_center) * 3600;

  M0 = a * (1 - e);
  M2 = 3.0 / 2.0 * e * M0;
  M4 = 5.0 / 4.0 * e * M2;
  M6 = 7.0 / 6.0 * e * M4;
  M8 = 9.0 / 8.0 * e * M6;
  a0 = M0 + M2 / 2.0 + 3.0 / 8.0 * M4 + 5.0 / 16.0 * M6 + 35.0 / 128.0 * M8;
  a2 = M2 / 2.0 + M4 / 2 + 15.0 / 32.0 * M6 + 7.0 / 16.0 * M8;
  a4 = M4 / 8.0 + 3.0 / 16.0 * M6 + 7.0 / 32.0 * M8;
  a6 = M6 / 32.0 + M8 / 16.0;
  a8 = M8 / 128.0;

  Xz = a0 * B - a2 / 2.0 * sin(2 * B) + a4 / 4.0 * sin(4 * B) - a6 / 6.0 * sin(6 * B) + a8 / 8.0 * sin(8 * B);

  c = a * a / b;
  V = sqrt(1 + e1 * cos(B) * cos(B));
  N = c / V;
  t = tan(B);
  n = e1 * cos(B) * cos(B);
  m1 = N * cos(B);
  m2 = N / 2.0 * sin(B) * cos(B);
  m3 = N / 6.0 * pow(cos(B), 3) * (1 - t * t + n);
  m4 = N / 24.0 * sin(B) * pow(cos(B), 3) * (5 - t * t + 9 * n);
  m5 = N / 120.0 * pow(cos(B), 5) * (5 - 18 * t * t + pow(t, 4) + 14 * n - 58 * n * t * t);
  m6 = N / 720.0 * sin(B) * pow(cos(B), 5) * (61 - 58 * t * t + pow(t, 4));

  x0 = Xz + m2 * l * l / pow(P0, 2) + m4 * pow(l, 4) / pow(P0, 4) + m6 * pow(l, 6) / pow(P0, 6);
  y0 = m1 * l / P0 + m3 * pow(l, 3) / pow(P0, 3) + m5 * pow(l, 5) / pow(P0, 5);

  double x = x0;
  double y = y0 + 500000;
  xx = x;
  yy = y;

  // cout << fixed << "方法一 x=" << x << endl;
  // cout << fixed << "方法一 y=" << y << endl;
}

double Bb = 0;
double Ll = 0;
void ProjectPoint2DadiPoint(double x, double y, double l0) {
  double Bf, B0, FBf, M, N, V, t, n, c, y1, n1, n2, n3, n4, n5, n6, a0, a2, a4, a6, M0, M2, M4, M6, M8, l;
  int L_num, L_center;

  L_num = (int)(x / 1000000.0);
  y1 = y - 500000;

  M0 = a * (1 - e);
  M2 = 3.0 / 2.0 * e * M0;
  M4 = 5.0 / 4.0 * e * M2;
  M6 = 7.0 / 6.0 * e * M4;
  M8 = 9.0 / 8.0 * e * M6;
  a0 = M0 + M2 / 2.0 + 3.0 / 8.0 * M4 + 5.0 / 16.0 * M6 + 35.0 / 128.0 * M8;
  a2 = M2 / 2.0 + M4 / 2 + 15.0 / 32.0 * M6 + 7.0 / 16.0 * M8;
  a4 = M4 / 8.0 + 3.0 / 16.0 * M6 + 7.0 / 32.0 * M8;
  a6 = M6 / 32.0 + M8 / 16.0;

  Bf = x / a0;
  B0 = Bf;

  while ((fabs(Bf - B0) > 0.0000001) || (B0 == Bf)) {
    B0 = Bf;
    FBf = -a2 / 2.0 * sin(2 * B0) + a4 / 4.0 * sin(4 * B0) - a6 / 6.0 * sin(6 * B0);
    Bf = (x - FBf) / a0;
  }

  t = tan(Bf);
  c = a * a / b;
  V = sqrt(1 + e1 * cos(Bf) * cos(Bf));
  N = c / V;
  M = c / pow(V, 3);
  n = e1 * cos(Bf) * cos(Bf);
  n1 = 1 / (N * cos(Bf));
  n2 = -t / (2.0 * M * N);
  n3 = -(1 + 2 * t * t + n) / (6.0 * pow(N, 3) * cos(Bf));
  n4 = t * (5 + 3 * t * t + n - 9 * n * t * t) / (24.0 * M * pow(N, 3));
  n5 = (5 + 28 * t * t + 24 * pow(t, 4) + 6 * n + 8 * n * t * t) / (120.0 * pow(N, 5) * cos(Bf));
  n6 = -t * (61 + 90 * t * t + 45 * pow(t, 4)) / (720.0 * M * pow(N, 5));

  double B = (Bf + n2 * y1 * y1 + n4 * pow(y1, 4) + n6 * pow(y1, 6)) / PI * 180;
  double L0 = l0;
  l = n1 * y1 + n3 * pow(y1, 3) + n5 * pow(y1, 5);
  double L = L0 + l / PI * 180;

  Bb = B;
  Ll = L;

  // cout << "方法一 B=" << B << endl;
  // cout << "方法一 L=" << L << endl;
}

geo::gcs wgs84 {
  "EPSG:4326",
  4326,
  a,
  (a - b) / a
  // 6378137,
  // 0.003352810664
};

void test_unit_1()
{

  geo::gaussian_zone3_projection<40> gk3_40{&wgs84};

  double ldt = 120.1, lat = 30.1;
  while (ldt < 121.5) {
    while (lat < 40) {
      lat += 1.5;
      DadiPoint2ProjectPoint(lat, ldt); // 标准高精度正算函数
      ProjectPoint2DadiPoint(xx, yy, 120); // 标准高精度反算函数
      auto [x, y] = gk3_40.transform_p(lat, ldt);
      auto [B, L] = gk3_40.transform_i(x, y);
      // std::cout << std::fixed << std::fabs(x - xx) << std::endl;
      // std::cout << std::fixed << std::fabs(y - yy) << std::endl;
      // std::cout << std::setprecision(10) << std::fixed << std::fabs(B - Bb) << std::endl;
      std::cout << std::fixed << std::fabs(L - Ll) << std::endl;
    }
    ldt += 0.3;
    lat = 30.1;
  }
}

int main()
{
  // test_unit_1();
  DadiPoint2ProjectPoint(31, 121);
  double x = xx;
  double y = yy;
  std::cout << std::fixed << x << ", " << y << std::endl;
  ProjectPoint2DadiPoint(x, y, 120); // 反算
  double B = Bb;
  double L = Ll;
  // 正算
  L_center = 117;
  DadiPoint2ProjectPoint(B, L);
  std::cout << std::fixed << xx << ", " << yy << std::endl;


  geo::coordination coord{121, 31};
  geo::gaussian_zone3_projection<40> gk3_40{&wgs84};
  geo::projected_coord pcoord{coord, &gk3_40};
  geo::gaussian_zone3_projection<39> gk3_39{&wgs84};
  auto new_proj = gk3_39.reset_gcs(&wgs84);
  std::cout << std::fixed << pcoord.x() << ", " << pcoord.y() << std::endl;
  pcoord.transform_projection(&gk3_39);
  std::cout << std::fixed << pcoord.x() << ", " << pcoord.y() << std::endl;

  return 0;
}
