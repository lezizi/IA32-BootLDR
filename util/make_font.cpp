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
            Font Maker (utility)
 ==========================================

*/
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std ;
int main( int argc , char** argv )
{
	unsigned int count = 0 ;
	if( argc != 3 ){
		return -1;
	}
	else{
		ofstream out_chinese_font( argv[ 2 ] ) ;
		string chinese = "¹Ø±ÕÀÖ×Ì×Ì¹¤×÷ÊÒ" ;

		for( int i = 0 ; i < chinese.length() ; i += 2 ){
			ifstream in_chinese_font( argv[ 1 ] ) ;
			unsigned char c1 = chinese[ i ] ;
			unsigned char c2 = chinese[ i + 1 ] ;
			int q = c1 - 0xa0 ;
			int w = c2 - 0xa0 ;
			unsigned int pos = ( 94 * ( q - 1 ) + ( w - 1 ) ) * 32 ;
			char ch ;
			for( int j = 0 ; j < pos ; ++j ){
				in_chinese_font.get( ch ) ;
			}
			
			for( int j = 0 ; j < 32 ; ++j ){
				in_chinese_font.get( ch ) ;
				out_chinese_font.put( ch ) ;
			}
			in_chinese_font.close() ;
		}
		out_chinese_font.close() ;
	}
}