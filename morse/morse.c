/*
La Fortuna Morse Code interpreter (c) by Thomas Dodds

La Fortuna Morse Code interpreter is licensed under a
Creative Commons Attribution 3.0 Unported License.

You should have received a copy of the license along with this
work.  If not, see <http://creativecommons.org/licenses/by/3.0/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "morse.h"
uint8_t interpret(uint8_t data[10]){
  int i = 0;
  int len = 10;
  for (int a = 0; a < len; a++){
    if (data[a]){
      i = i + 1;
    }
  }
  if (i < 1){
    return 0;
  }
  else if (i <4){
    return 1;
  }
  else {
    return 2;
  }
}
char parse(uint8_t data[]){
  if (data[0] == 1){ //.*
    if (data[1] == 1){//..*
      if (data[2] == 1){//...*
        if (data[3] == 1){//....
          return 'h';
        }
        else if (data[3] == 2){//...-
          return 'v';
        }
        else{ //...
          return 's';
        }
      }
      else if (data[2] == 2){//..-*
        if (data[3] == 1){//..-.
          return 'f';
        }
        else if (data[3] == 2){//..--
          return '$';//Error
        }
        else{//..-
          return 'u';
        }
      }
      else{//..
        return 'i';
      }
    }
    else if (data[1] == 2){//.-*
      if (data[2] == 1){//.-.*
        if (data[3] == 1){//.-..
          return 'l';
        }
        else if (data[3] == 2){//.-.-
          return '$'; //Error
        }
        else{//.-.
          return 'r';
        }
      }
      else if (data[2] == 2){//.--*
        if (data[3] == 1){//.--.
          return 'p';
        }
        else if (data[3] == 2){//.---
          return 'j';
        }
        else{//.--
          return 'w';
        }
      }
      else{ //.-
        return 'a';
      }
    }
    else{//.
      return 'e';
    }
  }
  else if(data[0] == 2){ //-*
    if (data[1] == 1){//-.*
      if (data[2] == 1){//-..*
        if (data[3] == 1){//-...
          return 'b';
        }
        else if (data[3] == 2){//-..-
          return 'x';
        }
        else{//-..
          return 'd';
        }
      }
      else if (data[2] == 2){//-.-*
        if (data[3] == 1){//-.-.
          return 'c';
        }
        else if (data[3] == 2){//-.--
          return 'y';
        }
        else{//-.-
          return 'k';
        }
      }
      else{//-.
        return 'n';
      }
    }
    else if (data[1] == 2){//--*
      if (data[2] == 1){//--.*
        if (data[3] == 1){//--..
          return 'z';
        }
        else if (data[3] == 2){//--.-
          return 'q';
        }
        else{//--.
          return 'g';
        }
      }
      else if (data[2] == 2){//---*
        if (data[3] == 1){//---.
          return '$';//Error
        }
        else if (data[3] == 2){//----
          return '$';//Error
        }
        else{//---
          return 'o';
        }
      }
      else{//--
        return 'm';
      }
    }
    else{
      return 't';
    }
  }
  return ' ';//Empty
}

// int main(){
//   uint8_t data[] = {1, 1,1,1};
//   char str;
//   str = parse(data);
//   printf("%c",str);
//   printf("\n");
//
//   return 0;
// }
