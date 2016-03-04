/**
 * @file Algorithms.hpp
 * @brief Interfaces for new detectors, extractors, matchers and optical flow techniques.
 *    Futhermore it contains specifications for some of this algorithms presents on OpenCV library
 * @note More informations about each specifications may be found on
 * <a href="http://docs.opencv.org/modules/features2d/doc/features2d.html">OpenCV documentation</a>
 */

#ifndef AVR_ALGORITHMS_HPP
#define AVR_ALGORITHMS_HPP

#include <avr/core/Core.hpp>
#include "Feature.hpp"


namespace avr {

using std::vector;

// base classes in this file
class FeatureDetector;
class DescriptorExtractor;
class DescriptorMatcher;
class OpticFlowAlgorithm;
// class for system algorithms setup
class SystemAlgorithms;

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                  Feature Detectors                                                           *
\*----------------------------------------------------------------------------------------------------------------------------*/

//! Abstract base class for 2D image feature detectors.
class FeatureDetector {
public:
   virtual ~FeatureDetector() {/* dtor */}
   /**
    * Detect
    * @param image [in] The image.
    * @param keys [out] Detected keypoints in the image.
    */
   virtual void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const = 0;
};

/**
 * Detector component of Binary Robust Invariant Scalable Keypoints (BRISK) algorithm, by Leutenegger et al. (2011)
 */
class BRISKDetector : public FeatureDetector {
public:
   /**
    * @param _threshold    Detection threshold score.
    * @param _nOctaves     Number of detection octaves, use 0 to do single scale.
    * @param _patternScale Apply this scale to the pattern used for sampling the neighbourhood of a keypoint.
    */
   BRISKDetector(int _threshold=30, int _nOctaves=3, float _patternScale=1.0f) : FeatureDetector(),
      threshold(_threshold), nOctaves(_nOctaves), patternScale(_patternScale) {/* ctor */}
   // detect
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;

private:
   int threshold;
   int nOctaves;
   float patternScale;
};

/**
 * FAST corner detector, by Rosten and Drummond (2006)
 */
class FASTDetector : public FeatureDetector {
public:
   /**
    * @param _threshold Threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.
    * @param _nonmaxSuppression If true, non-maximum suppression is applied to detected corners (keypoints).
    */
   FASTDetector(int _threshold=10, bool _nonmaxSuppression=true) : FeatureDetector(),
      threshold(_threshold), nonmaxSuppression(_nonmaxSuppression) {/* ctor */}
   // detect
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;

private:
   int threshold;
   bool nonmaxSuppression;
};

/**
 * Detector component of ORB (ORiented Brief) algorithm, by Rublee et al. (2011)
 */
class ORBDetector : public FeatureDetector {
public:
   /**
    * @param _nfeatures The maximum number of features to retain.
    * @param _scaleFactor Pyramid decimation ratio, greater than 1.
    * @param _nlevels The number of pyramid levels.
    * @param _edgeThreshold This is size of the border where the features are not detected.
    */
   ORBDetector(int _nfeatures = 500, float _scaleFactor = 1.2f, int _nlevels = 8, int _edgeThreshold = 31) : FeatureDetector(),
      nfeatures(_nfeatures), scaleFactor(_scaleFactor), nlevels(_nlevels), edgeThreshold(_edgeThreshold) {/* ctor */}
   // detect
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;

private:
   int nfeatures;
   float scaleFactor;
   int nlevels;
   int edgeThreshold;
};

/**
 * STAR keypoint detector introduced by K. Konolige (2006)
 */
class STARDetector : public FeatureDetector {
public:
   /**
    * @param _maxSize Maximum size of the features. The following values are supported: 4, 6, 8, 11, 12, 16, 22, 23, 32, 45, 46, 64, 90, 128.
    *       In the case of a different value the result is undefined.
    * @param _responseThreshold Threshold for the approximated laplacian, used to eliminate weak features.
    *       The larger it is, the less features will be retrieved.
    * @param _lineThresholdProjected Another threshold for the laplacian to eliminate edges.
    * @param _lineThresholdBinarized Yet another threshold for the feature size to eliminate edges.
    *       The larger the 2nd threshold, the more points you get.
    */
   STARDetector(int _maxSize=45, int _responseThreshold=30, int _lineThresholdProjected=10, int _lineThresholdBinarized=8) : FeatureDetector(),
      maxSize(_maxSize), responseThreshold(_responseThreshold), lineThresholdProjected(_lineThresholdProjected),
      lineThresholdBinarized(_lineThresholdBinarized) {/* ctor */}
   // detect
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;

private:
   int maxSize;
   int responseThreshold;
   int lineThresholdProjected;
   int lineThresholdBinarized;
};

/**
 * Detector component of Scale Invariant Features Transform (SIFT) algorithm, by D. Lowe (2004)
 */
class SIFTDetector : public FeatureDetector {
public:
   /**
    * @param _nfeatures The number of best features to retain. The features are ranked by their scores (measured as the local contrast).
    * @param _nOctaveLayers The number of layers in each octave. The number of octaves is computed automatically from the image resolution.
    * @param _contrastTh The contrast threshold used to filter out weak features in semi-uniform (low-contrast) regions.
    *       The larger the threshold, the less features are produced by the detector.
    * @param _edgeTh The threshold used to filter out edge-like features. The larger the threshold more features are retained.
    * @param _sigma The sigma of the Gaussian applied to the input image at the octave #0.
    */
   SIFTDetector(int _nfeatures=0, int _nOctaveLayers=3, double _contrastTh=0.04, double _edgeTh=10, double _sigma=1.6) : FeatureDetector(),
      nfeatures(_nfeatures), nOctaveLayers(_nOctaveLayers), contrastThreshold(_contrastTh), edgeThreshold(_edgeTh), sigma(_sigma){/* ctor */}
   // detect
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;

private:
   int nfeatures;
   int nOctaveLayers;
   double contrastThreshold;
   double edgeThreshold;
   double sigma;
};

/**
 * Detector component of Speeded Up Robust Features (SURF) algorithm, by H. Bay et al (2006)
 */
class SURFDetector : public FeatureDetector {
public:
   /**
    * @param _hessianThreshold Threshold for hessian keypoint detector. Only features, whose hessian is larger than threshold
    *       are retained by the detector. Therefore, the larger the value, the less keypoints you will get and more fast it is.
    * @param _nOctaves Number of pyramid octaves the keypoint detector will use.
    * @param _nOctaveLayers Number of octave layers within each octave.
    */
   SURFDetector(double _hessianThreshold = 400.0, int _nOctaves=4, int _nOctaveLayers=2) : FeatureDetector(),
      hessianThreshold(_hessianThreshold), nOctaves(_nOctaves), nOctaveLayers(_nOctaveLayers) {/* ctor */}
   // detect
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys) const;

private:
   double hessianThreshold;
   int nOctaves;
   int nOctaveLayers;
};

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                Descriptor Extractors                                                         *
\*----------------------------------------------------------------------------------------------------------------------------*/

//! Abstract base class for computing descriptors for image keypoints.
class DescriptorExtractor {
public:
   virtual ~DescriptorExtractor() {/* dtor */}
   /**
    * Extract
    * @param image [in] The image.
    * @param keys [in/out] Keypoints of the image.
    * @param descriptors [out] Computed descriptor for each keypoint.
    */
   virtual void operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& descriptors) const = 0;
};

/**
 * The Binary Robust Independent Elementary Features (BRIEF) extractor algorithm, by Calonder et al. (2010)
 */
class BRIEFExtractor : public DescriptorExtractor {
public:
   //! @param bytes is a length of descriptor in bytes. It can be equal 16, 32 or 64 bytes.
   BRIEFExtractor(int _bytes = 32) : DescriptorExtractor(),
      bytes(_bytes) {/* ctor */}
   // extract
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const;

private:
   int bytes;
};

/**
 * Descriptor Extractor component of Binary Robust Invariant Scalable Keypoints (BRISK) algorithm, by Leutenegger et al. (2011)
 */
class BRISKExtractor : public DescriptorExtractor {
public:
   //! @param _patternScale Apply this scale to the pattern used for sampling the neighbourhood of a keypoint.
   BRISKExtractor(float _patternScale=1.0f) : DescriptorExtractor(),
      patternScale(_patternScale) {/* ctor */}
   // extract
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const;

private:
   float patternScale;
};

/**
 * Descriptor Extractor component of ORB (ORiented Brief) algorithm, by Rublee et al. (2011)
 */
class ORBExtractor : public DescriptorExtractor {
public:
   /**
    * @param _WTA_K The number of points that produce each element of the oriented BRIEF descriptor.
    * @param _patchSize Size of the patch used by oriented BRIEF descriptor.
    */
   ORBExtractor(int _WTA_K=2, int _patchSize=31) : DescriptorExtractor(),
      WTA_K(_WTA_K), patchSize(_patchSize) {/* ctor */}
   // extract
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const;

private:
   int WTA_K;
   int patchSize;
};

/**
 * Descriptor Extractor component of Scale Invariant Features Transform (SIFT) algorithm, by D. Lowe (2004)
 */
class SIFTExtractor : public DescriptorExtractor {
public:
   SIFTExtractor() : DescriptorExtractor() {/* ctor */}
   // extract
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const;
};

/**
 * Descriptor Extractor component of Speeded Up Robust Features (SURF) algorithm, by H. Bay et al (2006)
 */
class SURFExtractor : public DescriptorExtractor {
public:
   //! @param _extended Extended descriptor flag (true - use 128-element descriptor; false use 64-element descriptor).
   SURFExtractor(bool _extended = false) : DescriptorExtractor(),
      extended(_extended) {/* ctor */}
   // extract
   void operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const;

private:
   bool extended;
};

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                 Descriptor Matchers                                                          *
\*----------------------------------------------------------------------------------------------------------------------------*/

//! Abstract base class for matching two sets of descriptors.
class DescriptorMatcher {
public:
   virtual ~DescriptorMatcher() {/* dtor */}
   /**
    * Match
    * @param query [in] Query descriptor (object's features).
    * @param train [in] Train descriptor (scene's features).
    * @param matches [out] The best matchings between each pair of descriptors.
    */
   virtual void operator() (const Mat& query, const Mat& train, vector<cv::DMatch>& matches) const = 0;

private:

};

/**
 * For each descriptor in the first set, this matcher finds the closest
 * descriptor in the second set by trying each one.
 */
class BruteForceMatcher : public DescriptorMatcher {
public:
   /**
    * @param normType One of NORM_L1, NORM_L2, NORM_HAMMING, NORM_HAMMING2. L1 and L2 norms are preferable choices for SIFT and SURF descriptors;
    *    NORM_HAMMING should be used with ORB, BRISK and BRIEF; NORM_HAMMING2 should be used with ORB when WTA_K is larger than 2.
    * @param _crossCheck If true, the match operator will only return pairs (i,j) such that for i-th target descriptor
    *    the j-th scene descriptor is the nearest and vice versa, i.e will only return consistent pairs.
    *    Such technique usually produces best results with minimal number of outliers when there are enough matches.
    */
   BruteForceMatcher(int _normType, bool _crossCheck = false) : DescriptorMatcher(),
      normType(_normType), crossCheck(_crossCheck) {/* ctor */}
   // match
   void operator() (const Mat& query, const Mat& train, vector<cv::DMatch>& matches) const;

private:
   int normType;
   bool crossCheck;
};

/**
 * The Flann based matcher by Muja and Lowe (2009)
 */
class FlannBasedMatcher : public DescriptorMatcher {
public:
   FlannBasedMatcher() : DescriptorMatcher() {/* ctor */}
   // match
   void operator() (const Mat& query, const Mat& train, vector<cv::DMatch>& matches) const;
};

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                     Optical Flow                                                             *
\*----------------------------------------------------------------------------------------------------------------------------*/

//! Abstract base class for optical flow algorithms implementation
class OpticFlowAlgorithm {
public:
   virtual ~OpticFlowAlgorithm() {/* dtor */}
   /**
    * Track
    * @param prevImage [in]   Previous 8-bit image
    * @param prevTracked [in] Input 2D points for which the flow needs to be found
    * @param currImage [in]   Current image of the same size and same type as prevImage
    * @param tracked [out]    Output 2D points containing the calculated new positions of input features in the second image
    * @param error [out]      Output vector of errors, each element of the vector is set to an error for the corresponding feature
    */
   virtual void operator() (const Mat& prevImage, const vector<Point2f>& prevTracked,
                            const Mat& currImage, vector<Point2f>& tracked,
                            vector<float>& error) const = 0;
};

/**
 * Optical Flow algorithm, by B. Lucas and T. Kanade (1981)
 */
class LucasKanadeAlgorithm : public OpticFlowAlgorithm {
public:
   LucasKanadeAlgorithm() : OpticFlowAlgorithm() {/* ctor */}

   /**
    * @brief Calculates an optical flow for a sparse feature set using the iterative Lucas-Kanade method
    * @note if the flow wasn’t found then the error is set to -1.0
    */
   void operator() (const Mat& prevImage, const vector<Point2f>& prevTracked,
                    const Mat& currImage, vector<Point2f>& tracked,
                    vector<float>& error) const;
};

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                               Global Algorithms Setup                                                        *
\*----------------------------------------------------------------------------------------------------------------------------*/

/**
 * @class SystemAlgorithms Algorithms.hpp <avr/track/Algorithms.hpp>
 * @brief This class defines the vision algorithms used for the system, only algorithms setup here will used during its execution.
 *
 * This class together with all interfaces present here make the library highly configurable and customizable.
 * It is important to choose a good algotithms combination for a better application's performance, to help in this choice
 * the class provides differents optimization options to build the object.
 */
class SystemAlgorithms {
public:
   const SPtr<FeatureDetector>      detector;
   const SPtr<DescriptorExtractor>  extractor;
   const SPtr<DescriptorMatcher>    matcher;
   const SPtr<OpticFlowAlgorithm>   tracker;

   //! Initialization construtor
   SystemAlgorithms( const SPtr<FeatureDetector>&      detector,
                     const SPtr<DescriptorExtractor>&  extractor,
                     const SPtr<DescriptorMatcher>&    matcher,
                     const SPtr<OpticFlowAlgorithm>&   tracker
                  ) : detector(detector), extractor(extractor), matcher(matcher), tracker(tracker) {/* ctor */}

   //! Copy constructor
   SystemAlgorithms(const SystemAlgorithms& system) : detector(system.detector), extractor(system.extractor),
                                                      matcher(system.matcher), tracker(system.tracker) {/* ctor */}

   /**
    * This method creates the object setting the algorithms based on optimization options that are a better performance or a better quality.
    * If both flags are set the method chooses a combination trying to find a balancing between performance and quality.
    */
   static SystemAlgorithms Create(bool optimazePerformance, bool optimazeQuality);

   // interface for the algorithms
   //! Detects keypoints in image, see FeatureDetector for more details
   void Detect(const Mat& image, vector<cv::KeyPoint>& keys) const {
      if(this->detector != nullptr) (*this->detector)(image, keys);
   }
   //! Extracts descriptors of image, see DescriptorExtractor for more details
   void Extract(const Mat& image, vector<cv::KeyPoint>& keys, Mat& descriptors) const {
      if(this->extractor != nullptr) (*this->extractor)(image, keys, descriptors);
   }
   //! Matches descriptors of images, see DescriptorMatcher for more details
   void Match(const Mat& query, const Mat& train, vector<cv::DMatch>& matches) const {
      if(this->matcher != nullptr) (*this->matcher) (query, train, matches);
   }
   //! Tracks a set of image points in another image, see OpticFlowAlgorithm for more details
   void Track(const Mat& prevFrame, const vector<Point2f>& prevTracked, const Mat& currFrame, vector<Point2f>& tracked, vector<float>& error) const {
      if(this->tracker != nullptr) (*this->tracker) (prevFrame, prevTracked, currFrame, tracked, error);
   }

};

} // namespace avr

#endif // AVR_ALGORITHMS_HPP
