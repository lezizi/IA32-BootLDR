/*
Copyright (C) 2012 LeZiZi Studio
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
;
       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 ==========================================
                Join (utility)
 ==========================================

*/

#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std ;

int main( int argc , char** argv )
{
	char ch ;
	argv++ ;
	unsigned int count = 0 ;
	if( argc == 1 ){
		cerr << "join target src1 src2 ..." << endl ;
	}
	else{
		ofstream outfile( *argv++ , ios_base::binary ) ;
		for( int i = 2 ; i < argc ; ++i ){
			ifstream infile( *argv++ ) ;
			while( infile.get( ch ) ){
				outfile.put( ch ) ;
				count++ ;
			}
			infile.close() ;
		}
		ch = 0 ;
		while( count++ != 1474560 ){
			outfile.put( ch ) ;
		}
		outfile.close() ;
	}
}
