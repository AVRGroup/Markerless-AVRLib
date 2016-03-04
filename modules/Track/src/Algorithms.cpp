#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp> // ORB, BRISK, etc..
#include <opencv2/nonfree/features2d.hpp>    // SIFT and SURF
#include <opencv2/video/tracking.hpp>        // optflow

#include <avr/track/Algorithms.hpp>

#ifndef NULL
   #define NULL 0x0
#endif // NULL


namespace avr {

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                  Feature Detectors                                                           *
\*----------------------------------------------------------------------------------------------------------------------------*/

void BRISKDetector::operator() (const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::BRISK detector(this->threshold, this->nOctaves, this->patternScale);
   detector.detect(image, keys);
}

void FASTDetector::operator() (const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::FastFeatureDetector detector(this->threshold, this->nonmaxSuppression);
   detector.detect(image, keys);
}

void ORBDetector::operator() (const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::OrbFeatureDetector detector(this->nfeatures, this->scaleFactor, this->nlevels, this->edgeThreshold);
   detector.detect(image, keys);
}

void STARDetector::operator() (const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::StarFeatureDetector detector(this->maxSize, this->responseThreshold, this->lineThresholdProjected, this->lineThresholdBinarized);
   detector.detect(image, keys);
}

void SIFTDetector::operator() (const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::SiftFeatureDetector detector(this->nfeatures, this->nOctaveLayers, this->contrastThreshold, this->edgeThreshold, this->sigma);
   detector.detect(image, keys);
}

void SURFDetector::operator() (const Mat& image, vector<cv::KeyPoint>& keys) const {
   cv::SurfFeatureDetector detector(this->hessianThreshold, this->nOctaves, this->nOctaveLayers);
   detector.detect(image, keys);
}

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                Descriptor Extractors                                                         *
\*----------------------------------------------------------------------------------------------------------------------------*/

void BRIEFExtractor::operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const {
   cv::BriefDescriptorExtractor extractor(this->bytes);
   extractor.compute(image, keys, out);
}

void BRISKExtractor::operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const {
   cv::BRISK extractor(30, 3, this->patternScale);
   extractor.compute(image, keys, out);
}

void ORBExtractor::operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const {
   cv::OrbDescriptorExtractor extractor(500, 1.2f, 8, 31, 0, this->WTA_K, cv::ORB::HARRIS_SCORE, this->patchSize);
   extractor.compute(image, keys, out);
}

void SIFTExtractor::operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const {
   cv::SiftDescriptorExtractor extractor;
   extractor.compute(image, keys, out);
}

void SURFExtractor::operator() (const Mat& image, vector<cv::KeyPoint>& keys, Mat& out) const {
   cv::SurfDescriptorExtractor extractor(400.0, 4, 2, this->extended, false);
   extractor.compute(image, keys, out);
}

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                 Descriptor Matchers                                                          *
\*----------------------------------------------------------------------------------------------------------------------------*/

void BruteForceMatcher::operator() (const Mat& query, const Mat& train, vector<cv::DMatch>& goodMatches) const {
   cv::BFMatcher matcher(this->normType, this->crossCheck);
   vector<vector<cv::DMatch> > matches;
   register bool ccheck = this->crossCheck;

   matcher.knnMatch(query, train, matches, ccheck ? 1 : 2);
   for(auto it : matches)
      if(ccheck || it[0].distance < 0.7 * it[1].distance)
         goodMatches.push_back(it[0]);
}

void FlannBasedMatcher::operator() (const Mat& query, const Mat& train, vector<cv::DMatch>& goodMatches) const {
   cv::FlannBasedMatcher matcher;
   vector<vector<cv::DMatch> > matches;

   matcher.knnMatch(query, train, matches, 2);
   for(auto it : matches)
      if(it[0].distance < 0.7 * it[1].distance)
         goodMatches.push_back(it[0]);
}

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                     Optical Flow                                                             *
\*----------------------------------------------------------------------------------------------------------------------------*/

void LucasKanadeAlgorithm::operator() (const Mat& prevImage, const vector<Point2f>& prevTracked,
                                       const Mat& currImage, vector<Point2f>& tracked,
                                       vector<float>& error) const
{
   vector<unsigned char> status; vector<float> err;
   cv::calcOpticalFlowPyrLK(prevImage, currImage, prevTracked, tracked, status, err,
                            cv::Size(31,31), 3, cv::TermCriteria(3, 20, 0.03), 0, 1e-3);

   for(size_t i = 0; i < status.size(); i++) {
      error.push_back( status[i] ? err[i] : -1.0f );
   }
}

/*----------------------------------------------------------------------------------------------------------------------------*\
*                                                           System                                                             *
\*----------------------------------------------------------------------------------------------------------------------------*/

SystemAlgorithms SystemAlgorithms::Create(bool optimazePerformance, bool optimazeQuality) {
   SPtr<FeatureDetector> detector;
   SPtr<DescriptorExtractor> extractor;
   SPtr<DescriptorMatcher> matcher;
   SPtr<OpticFlowAlgorithm> tracker = new LucasKanadeAlgorithm;

   // ambos ligados ou ambos desligados -> balanceamento
   if(!optimazePerformance xor optimazeQuality) {
      detector = new SIFTDetector(500);
      extractor = new BRISKExtractor;
      matcher = new BruteForceMatcher(cv::NORM_HAMMING);
   } else if(optimazePerformance) {
      detector = new STARDetector;
      extractor = new SURFExtractor;
      matcher = new BruteForceMatcher(cv::NORM_L1);
   } else { // optimazeQuality
      detector = new SIFTDetector;
      extractor = new SIFTExtractor;
      matcher = new BruteForceMatcher(cv::NORM_L2);
   }

   return SystemAlgorithms(detector, extractor, matcher, tracker);
}

} // namespace avr
