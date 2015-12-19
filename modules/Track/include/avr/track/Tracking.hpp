#ifndef AVR_TRACKING_HPP
#define AVR_TRACKING_HPP

#include <avr/core/Core.hpp>

namespace avr {

using std::vector;

//! Abstract base class for optical flow algorithms implementation
class OpticFlowAlgorithm {
public:
   virtual ~OpticFlowAlgorithm() {/* dtor */}
   /**
    * Track
    * @param prevFrame [in]   Previous frame
    * @param prevTracked [in] Points to track
    * @param currFrame [in]   Current frame
    * @param tracked [out]    New tracked points
    */
   virtual void operator() (const Mat& prevFrame, const vector<Point2f>& prevTracked,
                            const Mat& currFrame, vector<Point2f>& tracked,
                            vector<Point2f>& object) = 0;
};

class LukasKanadeAlgorithm : public OpticFlowAlgorithm {
public:
   LukasKanadeAlgorithm();
   // track
   void operator() (const Mat& prevFrame, const vector<Point2f>& prevTracked,
                    const Mat& currFrame, vector<Point2f>& tracked,
                    vector<Point2f>& object);


private:

};

} // namespace avr

#endif // AVR_TRACKING_HPP
