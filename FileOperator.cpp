/*
 * read and write file
 *
 * Created by Zexi Chen(zchen22)
 * Contributors: Zexi Chen, Qiang Zhang
 * Date: 2/24/2016
 *
 */

#include "FileOperator.h"

void readRawFile(const char* filename, uchar* data, ulint page_start, size_t len){

	//boost::iostreams::mapped_file_source file;
	int fd;
	uchar* map;
	size_t readlen = len * sizeof(uchar);

	//file.open(filename,readlen,page_start);
	//cout << file.alignment() << endl;
	fd = open(filename, O_RDONLY);


	if(fd == -1){
		cout << "Could not open the file" << endl;
		return ; 
	}

	map = (uchar *)mmap(0, readlen, PROT_READ, MAP_SHARED, fd, page_start);

	if(map == MAP_FAILED){
		close(fd);
		cout << "Could not map memory from the file" << endl;
		return ;
	}

	memcpy(data, map, readlen);
	close(fd);
	/*
	if(file{
		memcpy(data,file.data(),readlen);
		file.close();
	}else{
		cout << "Could not map memory from the file" << endl;
	}
	*/
	return ;
}

void writeData(uchar *array, int& count, int colSize,string filename, int len)
{
	int index = 0;
	ofstream myfile;
	cout << filename << endl;
	if (!count)
		myfile.open(filename,std::ofstream::out);
	else
		myfile.open(filename,std::ofstream::app);
	if (myfile.is_open())
	{
		while(index < len){
			myfile << (int)array[index] << " ";
			count++;
			index++;
			if((count+1) % colSize == 0)
				myfile << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file";

}

void readData(float *resArray, int rowNums,int colNums,string filename)
{
	string line;
	ifstream myfile;
	float fl;

	cout << filename << endl;

	myfile.open(filename);
	if (myfile.is_open())
	  {
	    while (! myfile.eof() )
	    {
	      for(int i=0;i<rowNums;i++)
	      {
	    	  for(int j=0;j<colNums;j++)
	    	  {
	    		  myfile >> fl;
				  resArray[i*colNums+j] = fl;
	    	  } 
	      }
	    }
	    myfile.close();
	  }

	  else cout << "Unable to open file"<< endl;
}

