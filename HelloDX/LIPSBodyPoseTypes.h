#ifndef H_LIPSBODYPOSETYPES
#define H_LIPSBODYPOSETYPES

#ifdef SHOW_FACE
#define LIPSBODYPOSE_CONNECT_TOTAL 16
#else
#define LIPSBODYPOSE_CONNECT_TOTAL 12
#endif

#if defined(_WIN64) || defined(_WIN32)
#define LIPSBODYPOSE_API __declspec(dllexport)
#else
#define LIPSBODYPOSE_API
#endif

#include <vector>

#include "LIPSBodyPoseDataStruct.h"

namespace lips {

class Exception : public std::exception {
public:
  Exception(const std::string &msg) { what_str = "lips::exception, " + msg; }
  virtual const char *what() const noexcept { return what_str.c_str(); }

private:
  std::string what_str;
};

enum LIPSBODYPOSE_API LIPSBODYPOSE_KEYPOINT_ENUM {
  LIPSBODYPOSE_NOSE = 0,
  LIPSBODYPOSE_NECK,
  LIPSBODYPOSE_R_SHOULDER,
  LIPSBODYPOSE_R_ELBOW,
  LIPSBODYPOSE_R_WRIST,
  LIPSBODYPOSE_L_SHOULDER,
  LIPSBODYPOSE_L_ELBOW,
  LIPSBODYPOSE_L_WRIST,
  LIPSBODYPOSE_R_HIP,
  LIPSBODYPOSE_R_KNEE,
  LIPSBODYPOSE_R_ANKLE,
  LIPSBODYPOSE_L_HIP,
  LIPSBODYPOSE_L_KNEE,
  LIPSBODYPOSE_L_ANKLE,
  LIPSBODYPOSE_R_EYE,
  LIPSBODYPOSE_L_EYE,
  LIPSBODYPOSE_R_EAR,
  LIPSBODYPOSE_L_EAR,
  LIPSBODYPOSE_POINT_TOTAL
};

enum LIPSBODYPOSE_API { LIPSBODYPOSE_PEOPLE_MAX = 36 };

// user can use LIPSBodyPoseRecord
class LIPSBODYPOSE_API LIPSBodyPoseRecord {
public:
  LIPSBodyPoseRecord();
  LIPSBodyPoseRecord(const std::vector<OutPutHuman> &one);

  const Coord2D &keypoint2d(size_t who, size_t part) {
    return OutPutHumans.at(who).joint_2d[part];
  }

  const Coord3D &keypoint3d(size_t who, size_t part) {
    return OutPutHumans.at(who).joint_3d[part];
  }

  const int get_id(size_t who) { return OutPutHumans.at(who).id; }

  inline int getNumberPeople() { return OutPutHumans.size(); }

private:
  std::vector<OutPutHuman> OutPutHumans;
};
} // namespace lips
#endif
