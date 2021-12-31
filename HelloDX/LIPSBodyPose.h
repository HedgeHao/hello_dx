#ifndef H_LIPSBODYPOSE
#define H_LIPSBODYPOSE
#include <memory>
#include <opencv2/opencv.hpp>

#include "LIPSBodyPoseTypes.h"

class LIPSBodyPoseImpl;

#include <map>
class LIPSBODYPOSE_API LIPSLibVersions {
public:
  LIPSLibVersions();
  ~LIPSLibVersions();
  std::map<std::string, std::string> VerMap; // "version_map"
};

class LIPSBODYPOSE_API Intrinsics {
public:
  Intrinsics();
  ~Intrinsics();

  // std::vector<float> getIntrinsics();
  std::vector<float>
      intrinsicsForPtCloud; // "Intrinsics for constructing point cloud"
};

namespace lips {
enum LIPSBODYPOSE_API LogTo : int {
  LOG_NONE = 0,
  LOG_CONSOLE,
  LOG_FILE,
  LOG_BOTH
};

class LIPSBODYPOSE_API LIPSBodyPose {
public:
  /**
   * @brief Constructor potentially throwing lips::Exception when startup error
   * happen
   */
  LIPSBodyPose(const bool is_mirror = true, const LogTo log = LOG_NONE,
               const bool enable_crash_handle = false);
  ~LIPSBodyPose();

  /**
   * @brief No copy-constructor is provided
   */
  LIPSBodyPose(const LIPSBodyPose &) = delete;
  LIPSBodyPose(LIPSBodyPose &&) = delete;

  /**
   * @brief No assign is allowed
   */
  LIPSBodyPose &operator=(const LIPSBodyPose &) = delete;
  LIPSBodyPose &operator=(LIPSBodyPose &&) = delete;

  /**
   * @brief LIPSBodyPose run as 'Trial Mode' when license file is invalid or not
   * provided.
   * @ret True if runs in 'Trial Mode'
   */
  bool isTrialMode() const noexcept;

  /**
   * @brief Read output from LIPSBodyPose pipeline, after waitForUpdate() call
   * to ensure no outdated frame returned.
   */
  void readFrame(cv::Mat &, cv::Mat &,
                 std::shared_ptr<LIPSBodyPoseRecord> &) noexcept;

  LIPSLibVersions getVersions(); // create and return a class object

  /**
   * @brief block until new frame arrived.
   */
  bool waitForUpdate();

  Intrinsics getIntrinsics();
  Intrinsics getIntrinsicsObj;

private:
  std::unique_ptr<LIPSBodyPoseImpl> m_impl;
  LIPSLibVersions LIPSLibVersionsObj;
};
} // namespace lips
#endif
