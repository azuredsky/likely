#include <ctime>
#include <iostream>

#include "likely.h"
#include "test.h"

using namespace cv;
using namespace likely;
using namespace std;

const double errorTolerance = 0.0005;

static Matrix matrixFromMat(const Mat &mat)
{
    if (!mat.isContinuous()) {
        fprintf(stderr, "ERROR - Continuous data required.\n");
        abort();
    }

    Matrix m;
    m.data = mat.data;
    m.channels = mat.channels();
    m.columns = mat.cols;
    m.rows = mat.rows;
    m.frames = 1;

    switch (mat.depth()) {
      case CV_8U:  m.hash = likely_matrix::u8;  break;
      case CV_8S:  m.hash = likely_matrix::i8;  break;
      case CV_16U: m.hash = likely_matrix::u16; break;
      case CV_16S: m.hash = likely_matrix::i16; break;
      case CV_32S: m.hash = likely_matrix::i32; break;
      case CV_32F: m.hash = likely_matrix::f32; break;
      case CV_64F: m.hash = likely_matrix::f64; break;
      default:     fprintf(stderr, "ERROR - Unsupported matrix depth.\n"); abort();
    }

    return m;
}

static Mat matrixToMat(const Matrix &m)
{
    int depth = -1;
    switch (m.type()) {
      case likely_matrix::u8:  depth = CV_8U;  break;
      case likely_matrix::i8:  depth = CV_8S;  break;
      case likely_matrix::u16: depth = CV_16U; break;
      case likely_matrix::i16: depth = CV_16S; break;
      case likely_matrix::i32: depth = CV_32S; break;
      case likely_matrix::f32: depth = CV_32F; break;
      case likely_matrix::f64: depth = CV_64F; break;
      default:                 fprintf(stderr, "ERROR - Unsupported matrix depth.\n"); abort();
    }
    return Mat(m.rows, m.columns, CV_MAKETYPE(depth, m.channels), m.data);
}

int Test::run() const
{
    UnaryFunction f = makeUnaryFunction(function());

    // Generate input matrix
    Mat srcOpenCV(50, 50, CV_32FC1);
    randu(srcOpenCV, 0, 255);
    Matrix srcLikely = matrixFromMat(srcOpenCV);

    // Test correctness
    Mat dstOpenCV = computeBaseline(srcOpenCV);
    Matrix dstLikely;
    f(&srcLikely, &dstLikely);

    float difference = sum(matrixToMat(dstLikely) - dstOpenCV)[0]/(dstOpenCV.rows*dstOpenCV.cols);
    if (abs(difference) >= errorTolerance) {
        fprintf(stderr, "ERROR - Test for %s differs by %.3f on average!\n", function(), difference);
        abort();
    }

    // Test speed
    int iterations;
    clock_t startTime, endTime;

    iterations = 0;
    startTime = endTime = clock();
    while ((startTime - endTime) / CLOCKS_PER_SEC < 1) {
        computeBaseline(srcOpenCV);
        endTime = clock();
        iterations++;
    }
    double speedOpenCV = double(iterations) / (startTime - endTime);

//    iterations = 0;
//    startTime = endTime = clock();
//    while ((startTime - endTime) / CLOCKS_PER_SEC < 1) {
//        f(&srcLikely, &dstLikely);
//        endTime = clock();
//        iterations++;
//    }
//    double speedLikely = double(iterations) / (startTime - endTime);

//    cout << "Speedup: " << speedLikely / speedOpenCV << "x";

    return 0;
}