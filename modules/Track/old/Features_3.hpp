#ifndef AVR_FEATURES_HPP
#define AVR_FEATURES_HPP

#include <avr/core/Core.hpp>
#include "keypoint.hpp"

namespace avr {

using std::vector;

typedef avr::Mat Descriptors;

/**
 * @class avr::FeatureDetector Features <avr/Track/Features.hpp>
 * @brief Abstract base class for 2D image feature detectors.
 */
class FeatureDetector {
public:
   virtual ~FeatureDetector() {/* dtor */}

   /**
    * @brief Detects keypoints in an image.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   virtual void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const = 0;
};

/**
 * @class avr::DescriptorExtractor Features.hpp <avr/Track/Features.hpp>
 * @brief Abstract base class for computing descriptors for image keypoints.
 *
 * In this interface we assume a keypoint descriptor can be represented as a
 * dense, fixed-dimensional vector of some basic type. We represent a collection
 * of descriptors as a Mat, where each row is one keypoint descriptor.
 */
class DescriptorExtractor {
public:
   virtual ~DescriptorExtractor() {/* dtor */}

   /**
    * @brief Computes the descriptors for a set of keypoints in an image.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   virtual void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const = 0;
};

/**
 * @class avr::DescriptorMatcher Features.hpp <avr/Track/Features.hpp>
 * @brief Abstract base class for matching two sets of descriptors.
 */
class DescriptorMatcher {
public:
   virtual ~DescriptorMatcher() {/* dtor */}

   /**
    * @brief Finds good matches for each target object descriptor in the scene
    * @param target   [in] The target object descriptors.
    * @param scene    [in] The scene descriptors.
    * @param matches [out] Image coordinates that was matched.
    */
   virtual void operator() (const Descriptors& target, const Descriptors& scene, vector<Point2f>& matches) const = 0;
};

//////////////////////////////////////////////////////////// Detectors ///////////////////////////////////////////////////////////////
class BRISKDetector : public FeatureDetector {
public:
	/**
    * @brief Detects keypoints in an image with BRISK detector algorithm.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;
};
class FASTDetector : public FeatureDetector {
public:
	/**
    * @brief Detects keypoints in an image with FAST detector algorithm.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;
};
class ORBDetector : public FeatureDetector {
public:
	/**
    * @brief Detects keypoints in an image with ORB detector algorithm.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;
};
class STARDetector : public FeatureDetector {
public:
	/**
    * @brief Detects keypoints in an image with STAR detector algorithm.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;
};
class SIFTDetector : public FeatureDetector {
public:
	/**
    * @brief Detects keypoints in an image with SIFT detector algorithm.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;
};
class SURFDetector : public FeatureDetector {
public:
	/**
    * @brief Detects keypoints in an image with SURF detector algorithm.
    * @param image [in] The input image.
    * @param keys [out] The detected keypoints.
    */
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;
};

//////////////////////////////////////////////////////////// Extractors ///////////////////////////////////////////////////////////////
class BRIEFExtractor : public DescriptorExtractor {
public:
   /**
    * @brief Computes the descriptors for a set of keypoints in an image with BRIEF extractor algorithm.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const;
};
class BRISKExtractor : public DescriptorExtractor {
public:
   /**
    * @brief Computes the descriptors for a set of keypoints in an image with BRISK extractor algorithm.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const;
};
class FREAKExtractor : public DescriptorExtractor {
public:
   /**
    * @brief Computes the descriptors for a set of keypoints in an image with FREAK extractor algorithm.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const;
};
class ORBExtractor : public DescriptorExtractor {
public:
   /**
    * @brief Computes the descriptors for a set of keypoints in an image with ORB extractor algorithm.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const;
};
class SIFTExtractor : public DescriptorExtractor {
public:
   /**
    * @brief Computes the descriptors for a set of keypoints in an image with SIFT extractor algorithm.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const;
};
class SURFExtractor : public DescriptorExtractor {
public:
   /**
    * @brief Computes the descriptors for a set of keypoints in an image with SURF extractor algorithm.
    * @param image  [in]   The input image.
    * @param keys   [in]   The input keypoints. Keypoints for which a descriptor cannot be computed are removed.
    * @param out   [out]   Computed descriptors. Row i is the descriptor for keypoint i.
    */
   void operator() (const Mat& image, const vector<cv::KeyPoint>& keys, Descriptors& out) const;
};

//////////////////////////////////////////////////////////// Matchers ///////////////////////////////////////////////////////////////
class BruteForceMatcher : public DescriptorMatcher {
public:
   /**
    * @brief Finds good matches for each target object descriptor in the scene
    * @param target   [in] The target object descriptors.
    * @param scene    [in] The scene descriptors.
    * @param matches [out] Image coordinates that was matched.
    */
   void operator() (const Descriptors& target, const Descriptors& scene, vector<Point2f>& matches) const;
};
class FlannBasedMatcher : public DescriptorMatcher {
public:
   /**
    * @brief Finds good matches for each target object descriptor in the scene
    * @param target   [in] The target object descriptors.
    * @param scene    [in] The scene descriptors.
    * @param matches [out] Image coordinates that was matched.
    */
   void operator() (const Descriptors& target, const Descriptors& scene, vector<Point2f>& matches) const;
};

} // namespace avr

#endif // AVR_FEATURES_HPP
