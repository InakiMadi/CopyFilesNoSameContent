/*
  Author: Iñaki Madinabeitia Cabrera

  How to compile:
  g++ -std=c++1z main.cpp -lstdc++fs -o main
  The name of the executable will be "main".

  Copy all the files from a directory to another directory,
  not copying the ones existing - files can have equal content with
  different names.

  It is proposed a not recursively solution to guarantee the
  correct use of the program. Also, an example is developed in main
  function.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using namespace std;


/*
Function that copies files from a directory to another directory
not recursively, given the paths from both directories.
*/
void CopyFilesNoSameContent(fs::path& p_o, fs::path& p_d){
  // We store the content of each file of the destination directory
  // in a vector of strings.
  vector<string> v_d;

  for(auto p: fs::directory_iterator(p_d)){
    ifstream file;
    file.open(p.path().string());
    stringstream sstr;
    sstr << file.rdbuf();
    v_d.push_back( sstr.str() );
  }

  // For each file in the origin directory, we get its content
  // in a string format.
  for(auto p: fs::directory_iterator(p_o)){
    ifstream file;
    file.open(p.path().string(), ios::in);
    stringstream sstr;
    sstr << file.rdbuf();
    string str = sstr.str();

    // We find out if its content is the same content as one of the
    // files in the destination directory.
    bool equal = false;
    for(unsigned int i=0; i<v_d.size() && !equal; ++i){
      if(str == v_d[i])
        equal = true;
    }
    // If there is no same content, we will start the copy. Else,
    // we do nothing.
    if(not equal){
      // If there is two files with the same name but different contents,
      // we will copy the file creating a new one, whose name will be
      // the one it had, plus "from <origin directory>".
      if( fs::exists(p_d / p.path().filename()) ){
        fs::path p2 = ( p_d / p.path().filename() );
        p2 += "_from_";
        p2 += p_o.filename();
        ofstream ofs(p2);
        ofs << str;
        ofs.close();
      }
      // Else, no same name and no same content,
      // we will simply copy the file in the destination directory.
      else
        fs::copy(p.path(), p_d);
    }
  }
}



/* EXAMPLE */
int main()
{
  // We will have four different contents.
  const string A = "aaaaa\n\naaa";
  const string B = "bbbb";
  const string C = "cccccccc\ncc";
  const string D = "cccccccc\nc";

  /*
  Here, this is a good description of the following example:

  In our relative path, we will create two directories - "dir1" and
  "dir2" -, and we will copy the files from "dir1" to "dir2".

  "dir1" will have 2 files - "a" with content A,
  and "c" with content C.

  "dir2" will have 3 files - "aa" with content A (different names but
  same contents), "b" with content B,
  and "c" with content D (same name but different contents).

  Then, the result will be that "dir2" will have "aa", "b",
  "c" (the "c" it had) and "c_from_dir1" (the "c" from "dir1").
  Note that file "a" will not be copied as "aa" has its same
  content.
  */
  fs::path curr_path = fs::current_path();
  fs::create_directories(curr_path / "dir1");
  fs::create_directories(curr_path / "dir2");

  fs::path p_o = curr_path / "dir1";
  fs::path p_d = curr_path / "dir2";

  fs::path p_aux = p_o;
  p_aux /= "a";
  ofstream of1(p_aux);
  of1 << A;
  of1.close();

  p_aux = p_o;
  p_aux /= "c";
  ofstream of2(p_aux);
  of2 << C;
  of2.close();

  p_aux = p_d;
  p_aux /= "aa";
  ofstream of3(p_aux);
  of3 << A;
  of3.close();

  p_aux = p_d;
  p_aux /= "b";
  ofstream of4(p_aux);
  of4 << B;
  of4.close();

  p_aux = p_d;
  p_aux /= "c";
  ofstream of5(p_aux);
  of5 << D;
  of5.close();

  // We use the function we developed.
  CopyFilesNoSameContent(p_o,p_d);
}
