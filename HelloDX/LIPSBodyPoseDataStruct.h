#ifndef H_LIPSBODYPOSEDATASTRUCT
#define H_LIPSBODYPOSEDATASTRUCT
#include <array>

#define POINT_TOTAL 18

struct Coord3D {
  float x;
  float y;
  float z;
};

struct Coord2D {
  float x;
  float y;
};

struct OutPutHuman {
  OutPutHuman(std::array<Coord2D, POINT_TOTAL> a,
              std::array<Coord3D, POINT_TOTAL> b, int c)
      : joint_2d(a), joint_3d(b), id(c) {}

  std::array<Coord2D, POINT_TOTAL> joint_2d;
  std::array<Coord3D, POINT_TOTAL> joint_3d;
  int id;
};

#endif
