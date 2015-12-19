#include <opencv2/video/tracking.hpp>   // optflow

#include <avr/track/Tracking.hpp>

namespace avr {

LukasKanadeAlgorithm::LukasKanadeAlgorithm() : OpticFlowAlgorithm() {/* ctor */}

void LukasKanadeAlgorithm::operator() (const Mat& prevFrame, const vector<Point2f>& prevTracked,
                                       const Mat& currFrame, vector<Point2f>& tracked,
                                       vector<Point2f>& object)
{
   /**
    * calcOpticalFlowPyrLK(prevImg, nextImg, prevPts, nextPts, status, err, winSize=Size(21,21), maxLevel=3,
    *                      criteria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01), flags=0, minEigThreshold=1e-4)
    *    - prevImg   Frame anterior
    *    - nextImg   Frame corrente
    *    - prevPts   Features na prevImg
    *    - nextPts   Features na nextImg [Output]
    *    - status    Status de cada ponto em nextPts (se 1 foi rastreado, 0 c.c) [Output]
    *    - err       Erro de cada ponto em nextPts [Output]
    *    - winSize   Tamanho da janela de busca
    *    - maxLevel  Nível máximo da pirâmide (espaço escala)
    *    - criteria  TermCriteria(type, maxCount, epsilon), Critério de parada do loop (it <= maxCount, search window moves by less than epsilon)
    *    - flags     OPTFLOW_USE_INITIAL_FLOW usa conteúdo de nextPts como estimações iniciais, se não setada usa prevPts
    *                OPTFLOW_LK_GET_MIN_EIGENVALS usa os menores autovalores como medida de erro, se não setada usa L1 divido por pixels na janela
    *    - minEigThreshold limiar mínimo para os autovalores
    */

   vector<unsigned char> status; vector<float> err;
   cv::calcOpticalFlowPyrLK(prevFrame, currFrame, prevTracked, tracked, status, err,
                            cv::Size(31,31), 3, cv::TermCriteria(3, 20, 0.03), 0, 1e-3);
   // TODO Filtrar os pontos que foram rastreados pelo status
   size_t k = 0;
   for(size_t i = 0; i < status.size(); i++) {
      if(status[i]) {
         tracked[k] = tracked[i];
         object[k++] = object[i];
      }
   }
   tracked.resize(k);
   object.resize(k);

#ifdef DEBUG_
   cout << "rest: " << tracked.size() << endl;
#endif // DEBUG_
}

} // namespace avr
