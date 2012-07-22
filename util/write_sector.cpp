/*
Copyright (C) 2012 LeZiZi Studio
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 ==========================================
       Floppy Sector Writer (utility)
 ==========================================

*/
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std ;

int main( int argc , char** argv )
{
	if( argc == 1 ){
		cerr << "Floppy Sector Writer"<<endl;
		cerr << "useage: dest src track resistive sector" << endl;
		cerr << "NOTE: all the numbers begin from 0" << endl;
		return -1;
	}
	else{
		unsigned int count = 0 ;
		char ch ;
		unsigned int pos = 512*(atoi(argv[3])*36+atoi(argv[4])*18+atoi(argv[5])) ;
		ofstream outfile( argv[ 1 ] , ios_base::in | ios_base::out ) ;
		outfile.seekp( pos ) ;
		ifstream infile( argv[2] ) ;
		while( infile.get( ch ) ){
			outfile.put( ch ) ;
		}
		infile.close() ;
		outfile.close() ;
		cout << "  Sector Writer: Data ("<< argv[2] <<") is successfully written to "<< pos <<"("<< argv[3]<<","<<argv[4]<<","<<argv[5]<<")"<<endl;
	}
	return 0;
}
