#ifndef AFFINEALIGNOBJ_H
#define AFFINEALIGNOBJ_H

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

namespace DIAlign 
{
enum TracebackType {SS = 0, DM = 1, DA = 2, DB = 3, TM = 4, TA = 5, TB = 6, LM = 7, LA = 8, LB = 9};
enum tbJump {M = 0, A = 1, B = 2};

// This function overloads << to display TracebackType.
std::ostream& operator<<(std::ostream& out, const TracebackType value);

// This function converts TracebackType Enum to characters.
std::vector<char> EnumToChar(std::vector<TracebackType> v);

struct AffineAlignObj
{
  double* M; // Match or Mismatch matrix, residues of A and B are aligned without a gap. M(i,j) = Best score upto (i,j) given Ai is aligned to Bj.
  double* A; // Insert in sequence A, residue in A is aligned to gap in B. A(i,j) is the best score given that Ai is aligned to a gap in B.
  double* B; // Insert in sequence B, residue in B is aligned to gap in A. B(i,j) is the best score given that Bj is aligned to a gap in A.
  TracebackType* Traceback;
  bool* Path; // Path matrix would represent alignment path through similarity matrix as binary-hot encoding.

  int signalA_len; // Number of data-points in signal A
  int signalB_len; // Number of data-points in signal B
  double GapOpen; // Penalty for Gap opening
  double GapExten; // Penalty for Gap extension
  // For single gap: Penalty = GapOpen
  // For two consecutive gaps: Penalty = GapOpen + GapExten
  // For n consecutive gaps: Penalty = GapOpen + (n-1)*GapExten
  bool FreeEndGaps; // True for Overlap alignment
  std::vector<int> indexA_aligned; // Aligned signalA indices after affine alignment
  std::vector<int> indexB_aligned; // Aligned signalB indices after affine alignment
  std::vector<double> score;  // Score along the aligned path

  // Not a default constructor
  AffineAlignObj() {}
  AffineAlignObj(int ROW_SIZE, int COL_SIZE)
  {
    M = new double[ROW_SIZE * COL_SIZE];
    A = new double[ROW_SIZE * COL_SIZE];
    B = new double[ROW_SIZE * COL_SIZE];
    Traceback = new TracebackType[3* ROW_SIZE * COL_SIZE];
    Path = new bool[ROW_SIZE * COL_SIZE];

    std::memset(M, 0, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memset(A, 0, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memset(B, 0, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memset(Traceback, SS, 3 * ROW_SIZE * COL_SIZE * sizeof(TracebackType));
    std::memset(Path, 0, ROW_SIZE * COL_SIZE * sizeof(bool));

    signalA_len = ROW_SIZE-1;
    signalB_len = COL_SIZE-1;
    GapOpen = 0.0;
    GapExten = 0.0;
    FreeEndGaps = true;

  }

  AffineAlignObj& operator=(const AffineAlignObj& rhs)
  {
    delete[] M;
    delete[] A;
    delete[] B;
    delete[] Traceback;
    delete[] Path;

    //std::cout << " this " << this << std::endl;
    signalA_len = rhs.signalA_len;
    signalB_len = rhs.signalB_len;
    GapOpen = rhs.GapOpen;
    GapExten = rhs.GapExten;
    FreeEndGaps = rhs.FreeEndGaps;
    indexA_aligned = rhs.indexA_aligned;
    indexB_aligned = rhs.indexB_aligned;
    score = rhs.score;

    int ROW_SIZE = rhs.signalA_len + 1;
    int COL_SIZE = rhs.signalB_len + 1;

    M = new double[ROW_SIZE * COL_SIZE];
    A = new double[ROW_SIZE * COL_SIZE];
    B = new double[ROW_SIZE * COL_SIZE];
    Traceback = new TracebackType[3* ROW_SIZE * COL_SIZE];
    Path = new bool[ROW_SIZE * COL_SIZE];
    
    std::memcpy(M, rhs.M, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memcpy(A, rhs.A, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memcpy(B, rhs.B, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memcpy(Traceback, rhs.Traceback, 3 *ROW_SIZE * COL_SIZE * sizeof(TracebackType));
    std::memcpy(Path, rhs.Path, ROW_SIZE * COL_SIZE * sizeof(bool));
  }

  AffineAlignObj(const AffineAlignObj& rhs)
  {
    signalA_len = rhs.signalA_len;
    signalB_len = rhs.signalB_len;
    GapOpen = rhs.GapOpen;
    GapExten = rhs.GapExten;
    FreeEndGaps = rhs.FreeEndGaps;
    indexA_aligned = rhs.indexA_aligned;
    indexB_aligned = rhs.indexB_aligned;
    score = rhs.score;

    int ROW_SIZE = rhs.signalA_len + 1;
    int COL_SIZE = rhs.signalB_len + 1;

    M = new double[ROW_SIZE * COL_SIZE];
    A = new double[ROW_SIZE * COL_SIZE];
    B = new double[ROW_SIZE * COL_SIZE];
    Traceback = new TracebackType[3* ROW_SIZE * COL_SIZE];
    Path = new bool[ROW_SIZE * COL_SIZE];
    
    std::memcpy(M, rhs.M, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memcpy(A, rhs.A, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memcpy(B, rhs.B, ROW_SIZE * COL_SIZE * sizeof(double));
    std::memcpy(Traceback, rhs.Traceback, 3 *ROW_SIZE * COL_SIZE * sizeof(TracebackType));
    std::memcpy(Path, rhs.Path, ROW_SIZE * COL_SIZE * sizeof(bool));
  }

  ~AffineAlignObj()
  {
  delete[] M;
  delete[] A;
  delete[] B;
  delete[] Traceback;
  delete[] Path;
  }

};
} // namespace DIAlign

#endif // AFFINEALIGNOBJ_H
