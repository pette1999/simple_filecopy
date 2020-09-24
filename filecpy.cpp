#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
// #include <libexplain/open.h>
//https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019
//https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/

using namespace std;

int main(int argc, char** argv) 
{
  char buf1[10]; //create buffer space to store data
  int file_open[2]; //create 2 file descriptor
  char* input_file;
  char* output_file;
  int sz;

  if (argc > 2)
  {
    input_file = argv[1]; // the first argument would be the input file
    output_file = argv[2]; // tge second argument would be output file
  }
  else
  {
    // If there are less than two arguments, then there will be something wrong with the command
    cout << "wrong argument" << endl;
    return 0;
  }

  //Open the input file with read permission ONLY
  file_open[0] = open(input_file, O_RDONLY);
  // Output file has more premission than the input file describer
  // Output file descriptor has premission to create a new file if there isn't a file exists and it has the premission to both read and write
  // Output file descriptor also has mode 00700, 00070 and 00007 which allows users, groups and others to have the permission to read, write and execute
  file_open[1] = open(output_file, O_RDONLY, 00777);
  // O_CREAT | O_RDWR

  // File describer would return a nonnegative number if there are no error occurs
  if (file_open[0] > 0)
  {
    cout << "Input File exists" << endl;
  }
  else
  {
    // If file scriptor returns -1, there are errors occur
    // perror("c1");
    perror("Can't open the Input File");
    return 0;
  }

  // while there arstill texts needs to be read, read the file
  while(int sz = read(file_open[0], buf1, 1) > 0) 
  {
    // reach to the end of the line
    if (buf1 == "\n")
    {
      // write content to the new file
      sz = write(file_open[1], buf1, 1);
      // check if reach the end of file
      if(sz == -1)
      {
        int errsv = errno;
        perror("ERROR");
        sz = 0;
        return 0;
      }
    }
    else
    {
      // write content to the new file
      sz = write(file_open[1], buf1, 1);
      // check if reach the end of file
      if (sz == -1)
      {
        int errsv = errno;
        perror("ERROR");
        sz = 0;
        return 0;
      }
    }
  }

  close(file_open[0]);
  close(file_open[1]);

  return 0;
}