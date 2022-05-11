//#include "SeparateChaining.cpp"
//#include "SeparateChaining.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include "QuadraticProbing.cpp"
#include "QuadraticProbing.h"
#include "DoubleHashing.h"

using namespace std;

bool solved(long long board);
bool solve(long long board, vector<vector<vector<long long> > > valid_moves, vector<vector<int> > &moveList);
bool hsolve(long long board, vector<vector<int> > &moveList, vector<vector<vector<long long> > > valid_moves, HashTable<int> &H, int size);
bool Dblsolve(long long board, vector<vector<int> > &moveList, vector<vector<vector<long long> > > valid_moves, DHashTable<int> &DH, int size);
vector<vector<long long> > nextMoves(long long board, vector<vector<vector<long long> > > valid_moves);
long long makeMove(long long board, vector<long long> move);
vector<int> pegger(vector<long long> move);

const long long one = 1;

// Mapping of areas that would be outside of a traditional peg solitare board
const long long invalid = 0b1100011110001100000000000000000000011000111100011;
int numProbes;
int dnumProbes;
int NumSearches1[5] = {0, 0, 0, 0, 0};
int NumSearches2[5] = {0, 0, 0, 0, 0};
int numProbes1[5] = {0, 0, 0, 0, 0};
int numProbes2[5] = {0, 0, 0, 0, 0};

int dNumSearches1[5] = {0, 0, 0, 0, 0};
int dNumSearches2[5] = {0, 0, 0, 0, 0};
int dnumProbes1[5] = {0, 0, 0, 0, 0};
int dnumProbes2[5] = {0, 0, 0, 0, 0};

int main(int argc, char *argv[])
{
  int peg;
  int size = 1000;

  // Establish an input stream to read in position of pegs in a given board
  ifstream input;

  // We will be describing the board as the binary representation of a long long (128 bits)
  long long board;

  // Hash tables for single hashing and double hashing
  HashTable<int> H;
  DHashTable<int> DH;
  vector<int> positions;

  vector<vector<int> > moveList;
  vector<vector<vector<long long> > > valid_moves;
  int delta[4][2] = {{-1, -2}, {1, 2}, {-7, -14}, {7, 14}};

  string inputboard;
  cout << "Please enter the name of the text file containing the board: ";
  cin >> inputboard;

  // Open board file to be read from
  input.open(inputboard);

  // Keep reading from file until EOF
  while (input >> peg)
  {
    positions.push_back(peg);
  }

  // Shift the bits into positions in the long long, as read from the peg board
  for (int i = 0; i < positions.size(); i++)
  {
    board = board | one << (positions[i]);
  }

  // fill valid moves w vectors ready to be filled with moves
  for (int i = 0; i < 49; i++)
  {
    valid_moves.push_back({});
  }
  for (int i = 2; i < 49; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      int x0 = i;
      int x1 = i + delta[j][0];
      int x2 = i + delta[j][1];

      bool pt1 = ((x0 / 7) == (x1 / 7) && (x0 / 7) == (x2 / 7));
      bool pt2 = ((x0 % 7) == (x1 % 7) && (x0 % 7) == (x2 % 7));

      if ((pt1 || pt2) && (x2 >= 2 && x2 <= 46))
      {
        long long move = one << x0 | one << x1 | one << x2;

        if ((invalid & move) == 0)
        {
          long long mask = one << x0 | one << x1;
          vector<long long> v_move = {move, mask};
          valid_moves[x0].push_back(v_move);
        }
      }
    }
  }

  string option = "";

  cout << "How would you like to solve the board?" << endl;
  cout << "1: solve the board without Hashing" << endl;
  cout << "2: solve using Open Hashing" << endl;
  cout << "3: QUIT" << endl;
  cin >> option;
  while (option != "-1")
  {
    moveList.clear();
    if (option == "1")
    {
      double solve_finish_time;
      double solve_start_time = clock();
      if (solve(board, valid_moves, moveList))
      {
        cout << endl
             << "Solution: ";
        for (int i = moveList.size() - 1; i >= 0; i--)
        {
          cout << "(";
          for (int j = 0; j < 3; j++)
          {
            cout << moveList[i][j];
            if (j < 2)
            {
              cout << " ";
            }
          }
          cout << ") ";
        }
        cout << endl;
      }
      else
      {
        cout << "No Solutions" << endl;
      }
      cout << endl;
      solve_finish_time = clock();
      double solveTime = (double)(solve_finish_time - solve_start_time) / CLOCKS_PER_SEC;
      cout << "Time taken to find solution: " << solveTime << " ms" << endl;

      cout << "How would you like to solve the board?" << endl;
      cout << "1: solve the board without Hashing" << endl;
      cout << "2: solve using Open Hashing" << endl;
      cout << "3: QUIT" << endl;
      cin >> option;
      moveList.clear();
    }
    if (option == "2")
    {
      double hsolve_finish_time;
      double hsolve_start_time = clock();
      if (hsolve(board, moveList, valid_moves, H, size))
      {
        cout << endl
             << "Hash Solution: ";
        for (int i = moveList.size() - 1; i >= 0; i--)
        {
          cout << "(";
          for (int j = 0; j < 3; j++)
          {
            cout << moveList[i][j];
            if (j < 2)
            {
              cout << " ";
            }
          }
          cout << ") ";
        }
        cout << endl;
      }
      else
      {
        cout << "No Solutions" << endl;
      }
      cout << endl;
      hsolve_finish_time = clock();
      double hsolveTime = (double)(hsolve_finish_time - hsolve_start_time) / CLOCKS_PER_SEC;
      cout << "Time taken to find solution: " << hsolveTime << "ms" << endl;

      cout << "NumSearches1: (";
      for (int i = 0; i < 5; i++)
      {
        cout << NumSearches1[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;

      cout << "NumSearches2: (";
      for (int i = 0; i < 5; i++)
      {
        cout << NumSearches2[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;
      cout << "NumProbes1: (";
      for (int i = 0; i < 5; i++)
      {
        cout << numProbes1[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;

      cout << "NumProbes2: (";
      for (int i = 0; i < 5; i++)
      {
        cout << numProbes2[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;

      moveList.clear();

      /*fill(NumSearches1.begin(),NumSearches1.end(),0);
      fill(NumSearches2.begin(),NumSearches2.end(),0);
      fill(numProbes1.begin(),numProbes1.end(),0);
      fill(numProbes2.begin(),numProbes2.end(),0);*/

      double dhsolve_finish_time;
      double dhsolve_start_time = clock();
      if (Dblsolve(board, moveList, valid_moves, DH, size))
      {
        cout << endl
             << "Double Hash Solution: ";
        for (int i = moveList.size() - 1; i >= 0; i--)
        {
          cout << "(";
          for (int j = 0; j < 3; j++)
          {
            cout << moveList[i][j];
            if (j < 2)
            {
              cout << " ";
            }
          }
          cout << ") ";
        }
        cout << endl;
      }
      else
      {
        cout << "No Solutions" << endl;
      }
      cout << endl;
      dhsolve_finish_time = clock();
      double dhsolveTime = (double)(dhsolve_finish_time - dhsolve_start_time) / CLOCKS_PER_SEC;
      cout << "Time taken to find solution: " << dhsolveTime << "ms" << endl;
      cout << endl;
      cout << "NumSearches1: (";
      for (int i = 0; i < 5; i++)
      {
        cout << dNumSearches1[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;

      cout << "NumSearches2: (";
      for (int i = 0; i < 5; i++)
      {
        cout << dNumSearches2[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;
      cout << "NumProbes1: (";
      for (int i = 0; i < 5; i++)
      {
        cout << dnumProbes1[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;

      cout << "NumProbes2: (";
      for (int i = 0; i < 5; i++)
      {
        cout << dnumProbes2[i];
        if (i < 4)
        {
          cout << ", ";
        }
      }
      cout << ")" << endl;

      cout << "How would you like to solve the board?" << endl;
      cout << "1: solve the board without Hashing" << endl;
      cout << "2: solve using Open Hashing" << endl;
      cout << "3: QUIT" << endl;
      cin >> option;
      moveList.clear();
    }
    if (option == "3")
    {
      cout << "exiting" << endl;
      exit(1);
    }
  }
}

bool solve(long long board, vector<vector<vector<long long> > > valid_moves, vector<vector<int> > &moveList)
{
  if (solved(board))
  {
    return true;
  }
  vector<vector<long long> > currMoves = nextMoves(board, valid_moves);
  for (auto move : currMoves)
  {
    long long postMove = makeMove(board, move);
    if (solve(postMove, valid_moves, moveList))
    {
      vector<int> mv = pegger(move);
      moveList.push_back(mv);
      return true;
    }
  }
  return false;
}

bool hsolve(long long board, vector<vector<int> > &moveList, vector<vector<vector<long long> > > valid_moves, HashTable<int> &H, int size)
{
  if (solved(board))
  {
    return true;
  }
  vector<vector<long long> > currMoves = nextMoves(board, valid_moves);
  for (auto move : currMoves)
  {
    float fsize = size;
    float hsize = H.getSize();
    long long postMove = makeMove(board, move);
    float loadFactor = (hsize / fsize);

    // cout << "Csize: " << size << endl;
    // cout << "LF" << loadFactor << endl;

    if (!(H.contains(postMove)))
    {
      if (loadFactor >= 0 && loadFactor <= 0.1)
      {
        numProbes2[0] += numProbes;
        NumSearches2[0] += 1;
      }
      else if (loadFactor >= 0.1 && loadFactor <= 0.2)
      {
        numProbes2[1] += numProbes;
        NumSearches2[1] += 1;
      }
      else if (loadFactor >= 0.2 && loadFactor <= 0.3)
      {
        numProbes2[2] += numProbes;
        NumSearches2[2] += 1;
      }
      else if (loadFactor >= 0.3 && loadFactor <= 0.4)
      {
        numProbes2[3] += numProbes;
        NumSearches2[3] += 1;
      }
      else if (loadFactor >= 0.4 && loadFactor <= 0.5)
      {
        numProbes2[4] += numProbes;
        NumSearches2[4] += 1;
      }

      if (hsolve(postMove, moveList, valid_moves, H, size))
      {
        vector<int> mv = pegger(move);
        moveList.push_back(mv);
        return true;
      }
      else
      {
        if (H.getSize() <= size)
        {
          H.insert(postMove);
        }
      }
    }
    else
    {
      if (loadFactor >= 0 && loadFactor <= 0.1)
      {
        numProbes1[0] += numProbes;
        NumSearches1[0] += 1;
      }
      else if (loadFactor >= 0.1 && loadFactor <= 0.2)
      {
        numProbes1[1] += numProbes;
        NumSearches1[1] += 1;
      }
      else if (loadFactor >= 0.2 && loadFactor <= 0.3)
      {
        numProbes1[2] += numProbes;
        NumSearches1[2] += 1;
      }
      else if (loadFactor >= 0.3 && loadFactor <= 0.4)
      {
        numProbes1[3] += numProbes;
        NumSearches1[3] += 1;
      }
      else if (loadFactor >= 0.4 && loadFactor <= 0.5)
      {
        numProbes1[4] += numProbes;
        NumSearches1[4] += 1;
      }
    }
  }
  return false;
}

vector<vector<long long> > nextMoves(long long board, vector<vector<vector<long long> > > valid_moves)
{
  vector<vector<long long> > moves;
  long long temp = board;
  int peg = 0;
  while (temp > 0)
  {
    if (temp & one)
    {
      for (auto move : valid_moves[peg])
      {
        if ((move[0] & board) == move[1])
        {
          moves.push_back(move);
        }
      }
    }
    peg++;
    temp = temp >> 1;
  }
  return moves;
}
bool solved(long long board)
{
  if ((board & (board - 1)) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

long long makeMove(long long board, vector<long long> move_list)
{
  long long move = move_list[0];
  long long mask = move_list[1];

  long long temp = board;

  if ((move & temp) == mask)
  {
    temp = temp ^ move;
    board = temp;
    return board;
  }
  else
    exit(0);
}

vector<int> pegger(vector<long long> move)
{
  int peg = 0;
  long long mv = move[0];
  long long msk = move[1];
  vector<int> pegs;
  int p1, p2, p3;

  bool LtH = false;
  bool checked = false;

  while (mv > 0)
  {
    mv = mv >> 1;
    peg++;
    if (mv & one)
    {
      if (!(checked))
      {
        checked = true;
        if ((msk >> peg) & one)
        {
          LtH = true;
        }
      }
      pegs.push_back(peg);
    }
  }

  if (!(LtH))
  {
    vector<int> reverse;
    for (int i = pegs.size() - 1; i >= 0; i--)
    {
      reverse.push_back(pegs[i]);
    }
    return reverse;
  }
  return pegs;
}

bool Dblsolve(long long board, vector<vector<int> > &moveList, vector<vector<vector<long long> > > valid_moves, DHashTable<int> &DH, int size)
{
  if (solved(board))
  {
    return true;
  }
  vector<vector<long long> > currMoves = nextMoves(board, valid_moves);
  for (auto move : currMoves)
  {
    float FdblS = DH.getSize();
    float Fsize = size;
    float loadFactor = (FdblS / Fsize);

    long long postMove = makeMove(board, move);
    if (!(DH.contains(postMove)))
    {
      // cout << "LF: " << loadFactor << endl;
      if (loadFactor >= 0 && loadFactor <= 0.1)
      {
        dnumProbes2[0] += dnumProbes;
        dNumSearches2[0] += 1;
      }
      else if (loadFactor >= 0.1 && loadFactor <= 0.2)
      {
        dnumProbes2[1] += dnumProbes;
        dNumSearches2[1] += 1;
      }
      else if (loadFactor >= 0.2 && loadFactor <= 0.3)
      {
        dnumProbes2[2] += dnumProbes;
        dNumSearches2[2] += 1;
      }
      else if (loadFactor >= 0.3 && loadFactor <= 0.4)
      {
        dnumProbes2[3] += dnumProbes;
        dNumSearches2[3] += 1;
      }
      else if (loadFactor >= 0.4 && loadFactor <= 0.5)
      {
        dnumProbes2[4] += dnumProbes;
        dNumSearches2[4] += 1;
      }
      if (Dblsolve(postMove, moveList, valid_moves, DH, size))
      {
        vector<int> mv = pegger(move);
        moveList.push_back(mv);
        return true;
      }
      else
      {
        if (DH.getSize() <= size)
        {
          DH.insert(postMove);
        }
      }
    }
    else
    {
      if (loadFactor >= 0 && loadFactor <= 0.1)
      {
        dnumProbes1[0] += dnumProbes;
        dNumSearches1[0] += 1;
      }
      else if (loadFactor >= 0.1 && loadFactor <= 0.2)
      {
        dnumProbes1[1] += dnumProbes;
        dNumSearches1[1] += 1;
      }
      else if (loadFactor >= 0.2 && loadFactor <= 0.3)
      {
        dnumProbes1[2] += dnumProbes;
        dNumSearches1[2] += 1;
      }
      else if (loadFactor >= 0.3 && loadFactor <= 0.4)
      {
        dnumProbes1[3] += dnumProbes;
        dNumSearches1[3] += 1;
      }
      else if (loadFactor >= 0.4 && loadFactor <= 0.5)
      {
        dnumProbes1[4] += dnumProbes;
        dNumSearches1[4] += 1;
      }
    }
  }
  return false;
}
