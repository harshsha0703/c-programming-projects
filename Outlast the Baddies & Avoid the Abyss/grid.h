#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;


template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;     // array of column elements
    size_t NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // array of ROWs
  size_t  NumRows;  // total # of rows (0..NumRows-1)
    
public:
  //
  // default constructor:
  //
  // It creates a 4x4 grid where all elements are initialized to the default value of type T.
  //
  Grid() {
    Rows = new ROW[4];  // 4 rows
    NumRows = 4;

    // It will initialize each row to have 4 columns:
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      // It will initialize the elements to their default value:
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // default value for type T:
      }
    }
  }

  //
  // parameterized constructor:
  //
  // Creates a grid with R rows and C columns.
  // All elements are initialized to the default value of type T.
  //
  Grid(size_t R, size_t C) {
    NumRows = R; 
    Rows = new ROW[R];
    // It will inittialize each row
    for (size_t i = 0; i < NumRows; ++i){
      Rows[i].Cols = new T[C];
      Rows[i].NumCols = C;
        // It will initialize each element
      for (size_t j = 0; j < C; ++j){
        Rows[i].Cols[j] = T();
      }
    }

  }
    
  //
  // destructor:
  //
  // It calls automatically to free memory for this Grid.
  //
  virtual ~Grid() {
    // Delete each colums array
      for (size_t i = 0; i < NumRows; ++i){
        delete[] Rows[i].Cols;
      }
      delete[] Rows;
  }


  //
  // copy constructor:
  //
  //Creates a new grid as a copy of an existing grid.
  // Example: this occurs when passing
  // Grid as a parameter by value
  //
  //   void somefunction(Grid<int> G4)  <--- G4 is a copy:
  //   { ... }
  //
  Grid(const Grid<T>& other) {
    NumRows = other.NumRows;
    Rows = new ROW[NumRows];
    // copy each row
    for (size_t i = 0; i < NumRows; ++i){
      Rows[i].NumCols = other.Rows[i].NumCols;
      Rows[i].Cols = new T[Rows[i].NumCols];
        // copy each element inside row
      for (size_t j = 0; j < Rows[i].NumCols; ++j){
        Rows[i].Cols[j] = other.Rows[i].Cols[j];
      }
    }

  
  }
    
  //
  // copy operator=
  //
  // Assigns the contents of one grid to another.
  //
  Grid& operator=(const Grid& other) {
    if (this == &other) {
        return *this;
    }
    // Free old memory
    for (size_t i = 0; i < NumRows; ++i) {
        delete[] Rows[i].Cols;
    }
    delete[] Rows;
    NumRows = other.NumRows;
    Rows = new ROW[NumRows];

    for (size_t i = 0; i < NumRows; ++i){
      Rows[i].NumCols = other.Rows[i].NumCols;
      Rows[i].Cols = new T[Rows[i].NumCols];

      for (size_t j = 0; j < Rows[i].NumCols; ++j){
        Rows[i].Cols[j] = other.Rows[i].Cols[j];
      }
    }
    return *this;  
  }

  //
  // numrows
  //
  // Returns the total numbers of rows in the Grid.  
  // The indices for these rows are 0..numrows-1.
  //
  size_t numrows() const {
    return NumRows;
  }
  

  //
  // numcols
  //
  // It will Returns the number of rows in the grid. 
  // For now, each row has the same number of columns.
  //
  size_t numcols(size_t r) const {
      if (r >= NumRows){
        throw out_of_range("Row index out of bounds");
      }
      return Rows[r].NumCols;
  }


  //
  // size
  //
  // Returns the total numbers of elements in the Grid.
  //
  size_t size() const {
      size_t totalSize = 0;
      for (size_t i = 0; i< NumRows; ++i){
        // It will add the number of columns in each row
        totalSize += Rows[i].NumCols;
      }
      return totalSize;
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c);
  // It will throw an exception if the indices are out of bounds.
  //
  //    grid(r, c) = ...
  //    It will print the grid
  //
  T& operator()(size_t r, size_t c) {
      if (r >= NumRows || c >= Rows[r].NumCols) {
        throw out_of_range("Grid indices out of bounds");
      }
      return Rows[r].Cols[c];
  }

  //
  // _output
  //
  // Prints the contents of the grid for debugging.
  // This is not tested.
  //
  void _output() {
      for (size_t i = 0; i < NumRows; ++i){
        for (size_t j = 0; j < Rows[i].NumCols; ++j){
            cout << Rows[i].Cols[j] << " ";
        }
        cout << endl;
      }
      
  }

};
