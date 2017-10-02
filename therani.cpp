#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 3){
    cerr << "Please provide an input and output file." << endl;
    return 1;
  }

  ifstream input(argv[1]);
  ofstream output(argv[2]);

  if(input.fail()){
  	cerr << "Problem opening input file provided." << endl;
  	return 1;
  }

  int experiments = 1;
  int* numsubjects;
  string **history;
  string line;
  string curr;
  bool started = false; // to track if first line is a START command
  int num = 0;

  //for each experiment, you will need to keep track of the number of subjects; and for each subject, of their history

  //loop to read The Rani's logbook.
  while(getline(input, curr)) {
	  line = curr;
	  stringstream ss;
	  ss << curr;
	  ss >> curr;
	  bool error = false; // tracks if an error has occurred or not


	if (curr == "START") {
				  // 					// if history and numsubjects already exist, need to delete first

				  // 		if(started){
		  		// 	for(int i=0; i<num; i++){
		  		// 		delete [] history[i];
		  		// 	}

		  		// 	delete [] history;
		  		// 	delete [] numsubjects;
		  		// }

		started = true;
		ss >> num;
		  
		if (ss.fail()) {
			output << "Error - incorrect command: " << line << endl;
  		    output << "Parameter should be an integer" << endl;
  		    error = true;
		}
		else if (num < 0) {
		    output << "Error - incorrect command: " << line << endl;
		    output << "Number out of range" << endl;
		    error = true;
		}
		else if(!error) {

		  	// allocating 2-D array and filling subject pool
			  history = new string*[1];
			  history[0] = new string[num];
			  for(int i=0; i<num; i++){
			  	history[0][i] = "0"; // adding n subjects to experiment 0 (the subject pool)
			  }


			  // allocating numsubjects (do NOT need to reallocate)
			  numsubjects = new int[num];
			  numsubjects[0] = num;
			  for(int i=1; i<experiments; i++){
			  	numsubjects[i] = 0;
			  }

		  }
	  }

	  else if (!started){
	  	cerr << "Error - no subjects yet." << endl;
	  }

	  else if (curr == "ADD") {
	 	experiments++;

	  	// copying history into temp
	  	string **temp = new string*[experiments];
	  	for(int i=0; i<experiments-1; i++){
	  		temp[i] = new string[num];
	  		for(int j=0; j<num; j++){
	  			temp[i][j] = history[i][j];
	  		}
	  	}

	  	// reallocating history with new parameters
	  	for(int i=0; i<experiments; i++){
	  		delete [] history[i];
	  	}
	  	delete [] history;

	  	history = new string*[experiments+1];
	  	for(int i=0; i<experiments; i++){
	  		history[i] = new string[num];
	  	}


	  	// copying data back into history from temp
	  	for(int i=0; i<experiments-1; i++){
	  		for(int j=0; j<num; j++){
	  			history[i][j] = temp[i][j];
	  		}
	  	}

	  	// deallocating old memory
	  	for(int i=0; i<experiments-1; i++){
	  		delete [] temp[i];
	  	}
	  	delete [] temp;

	  }

	  else if (curr == "MOVE") {
	  	
	  	// error checks
	  	double x0, y0, n0, m0, dummy;
	  	ss >> x0 >> y0 >> n0 >> m0;
	  	if(ss.fail()){
	  		output << "Error - incorrect command: " << line << endl;
	  		output << "Too few parameters." << endl;
	  		error = true;
	  	}
	  	else if(modf(x0, &dummy) != 0 || modf(y0, &dummy) != 0 || modf(n0, &dummy) != 0 || modf(m0, &dummy) != 0){
	  		output << "Error - incorrect command: " << line << endl;
  		    output << "Parameter should be an integer" << endl;
  		    error = true;
	  	}

	  	int x = (int) x0;
	  	int y = (int) y0;
	  	int n = (int) n0;
	  	int m = (int) m0;

	  	// more error checks
	  	if(x<0 || y<0 || n<0 || m<0 || x>experiments || y>experiments){
	  		output << "Error - incorrect command: " << line << endl;
		    output << "Number out of range" << endl;
		    error = true;
	  	}
	  	
	  	if(!error){
	  		int subjsMove = m-n+1; // num subjs being moveds

	  		// actual transferring of subjs
	  		for(int i=n; i<m+1; i++){
	  			string copy = history[x][i];
	  			
	  			stringstream var;
	  			var << y;
	  			string poop;
	  			var >> poop;
	  			copy += poop;

	  			history[y][numsubjects[y]] = copy;
	  			numsubjects[y]++;
	  		}

			// moving elements of history[x] over to the beginning
	  		string* dump = new string[numsubjects[x]-m-1]; // holds elements after shifted subjs
	  		int index = 0;

	  		for(int i=m+1; i<numsubjects[x]; i++){
	  			dump[index] = history[x][i];
	  			index++;
	  		}

	  		index = 0;
	  		for(int i=n; i<numsubjects[x]-m-1; i++){
	  			history[x][i] = dump[index];
	  			index++;
	  		}
	  		numsubjects[x] -= subjsMove;
	  		delete [] dump;
	  	}
	  }

	  else if (curr == "QUERY") {
	  	double x0, n0, dummy;
	  	ss >> x0 >> n0;

	  	if(ss.fail()){
	  		output << "Error - incorrect command: " << line << endl;
	  		output << "Too few parameters." << endl;
	  		error = true;
 	  	}
 	  	else if(modf(x0, &dummy) != 0 || modf(n0, &dummy) != 0){
 	  		output << "Error - incorrect command: " << line << endl;
  		    output << "Parameter should be an integer" << endl;
  		    error = true;
 	  	}
 	  	
 	  	int x = (int) x0;
 	  	int n = (int) n0;

 	  	if(x<0 || n<0 || x>experiments || n>numsubjects[x]){
 	  		output << "Error - incorrect command: " << line << endl;
		    output << "Number out of range" << endl;
		    error = true;
 	  	}
 	  	
 	  	if(!error){
 	  		string printing = history[x][n];
 	  		printing.erase(printing.begin());
 	  		output << printing << endl;
 	  	}
	  }

	  else {
		  output << "Error - incorrect command: " << line << endl;
		  cerr << "Command does not exist." << endl;
		  error = true;
	  }
  }

  for(int i=0; i<experiments; i++){
  	delete [] history[i];
  }
  delete [] history;
  delete [] numsubjects;
  return 0;
}
