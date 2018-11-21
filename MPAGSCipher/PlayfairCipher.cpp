
// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Out project headers
#include "PlayfairCipher.hpp"


PlayFairCipher::PlayFairCipher( const std::string& key )
{
    setKey(key);
  }



std::string PlayFairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{
  std::string text = inputText;
  //Encrypt
  if ( cipherMode == CipherMode::Encrypt) 
    {  
      // Change J -> I
      
      auto changeJI = [] (char elem)
	{
	  if (elem =='J')
	    return 'I';
	  else
	    return elem;
	};
      std::transform(text.begin(), text.end(), text.begin(), changeJI);
      
      // If repeated chars in a digraph add an X or  Q if X

      const size_t size = inputText.size();
      
      int j =0;

      for(size_t i=1; i < size; i++)
	{
	  j = i%2;
	  if (j==0)
	    {
	      if(inputText[i] == inputText[i-1])
		{
		  if(inputText[i-1] != 'X')
		    {
		      text[i] = 'X';
		    }else
		    {
		      text[i] = 'Q';
		    }
		}
	    }
	}
  
      // If size of input is odd, add a trailing Z
      if (j!=0)
	{
	  text = text + 'Z';
	}  

      // Loop over input in Digraphs

      const size_t sizeText = text.size();

      for(size_t i=0; i<sizeText; i = i+2)
	{
	  //  - Find the coords in the grid for each digraph
	  auto coordIter1 = lettermap_.find(text[i]);
	  auto coordIter2 = lettermap_.find(text[i+1]);

	  std::pair<size_t, size_t> coordinate1 = (*coordIter1).second;
	  std::pair<size_t, size_t> coordinate2 = (*coordIter2).second;

	  

	  //  - Apply the rules to these coords to get 'new' coords
	  
	  if(coordinate1.first == coordinate2.first)
	    { 
	      coordinate1.second = (coordinate1.second+1)%5;
	      coordinate2.second = (coordinate2.second+1)%5;
	    }
	  else if (coordinate1.second == coordinate2.second)
	    {
	      coordinate1.first = (coordinate1.first+1)%5;
	      coordinate2.first = (coordinate2.first+1)%5;
	    }
	  else
	    {
	      coordinate1.second = coordinate2.second;
	      coordinate2.second = coordinate1.second;
	    }
	  
	  //  - Find the letter associated with the new coords
	  auto letter1 = positionmap_.find(coordinate1);
	  auto letter2 = positionmap_.find(coordinate2);
	  text[i] = (*letter1).second;
	  text[i+1] = (*letter2).second;
	}
      
      // Return the text
      return text;
    }


  //Dectrypt:
  if ( cipherMode == CipherMode::Decrypt) 
    {
      // Loop over input in Digraphs
      for(size_t i=0; i<inputText.size(); i = i+2)
	{
	  //  - Find the coords in the grid for each digraph
	  auto coordIter1 = lettermap_.find(inputText[i]);
	  auto coordIter2 = lettermap_.find(inputText[i+1]);

	  std::pair<size_t, size_t> coordinate1 = (*coordIter1).second;
	  std::pair<size_t, size_t> coordinate2 = (*coordIter2).second;
 

	  //  - Apply the rules to these coords to get 'new' coords
	  if(coordinate1.first == coordinate2.first)
	    { 
	      coordinate1.second = (coordinate1.second-1+5)%5;
	      coordinate2.second = (coordinate2.second-1+5)%5;
	    }
	  else if (coordinate1.second == coordinate2.second)
	    {
	      coordinate1.first = (coordinate1.first-1+5)%5;
	      coordinate2.first = (coordinate2.first-1+5)%5;
	    }
	  else
	    {
	      coordinate1.second = coordinate2.second;
	      coordinate2.second = coordinate1.second;
	    }
	  
	  //  - Find the letter associated with the new coords
	  auto letter1 = positionmap_.find(coordinate1);
	  auto letter2 = positionmap_.find(coordinate2);
	  text[i] = (*letter1).second;
	  text[i+1] = (*letter2).second;
    	}

      // Return the text
      return text;
    }
  return text;

}


void PlayFairCipher::setKey(const std::string& key)
{
  // Store the original key
  key_ = key;

  // Append the alphabet
  key_ = key_+ "A"+"B"+"C"+"D"+"E"+"F"+"G"+"H"+"I"+"J"+"K"+"L"+"M"+"N"+"O"+"P"+"Q"+"R"+"S"+"T"+"U"+"V"+"W"+"X"+"Y"+"Z";

  // Make sure the key is upper case
  std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

  // Remove non-alpha characters
  auto iter = std::remove_if(key_.begin(), key_.end(), [] (char elem) { return !isalpha(elem);});
  key_.erase(iter, key_.end());

  // Change J -> I 
  auto changeJI = [] (char elem)
    {
      if (elem =='J')
	return 'I';
      else
	return elem;
    };
  std::transform(key_.begin(), key_.end(), key_.begin(), changeJI);

  // Remove duplicate letters
  std::string duplicate{};
  
  auto removeDuplicate = [&duplicate] (char elem)
    {
      if (duplicate.find(elem) == std::string::npos)
	{
	duplicate+=elem;
	return false;
	}
      else
	{
	  return true;
	}
    };
  
  auto iter2 = std::remove_if(key_.begin(), key_.end(), removeDuplicate );
  key_.erase(iter2, key_.end());

  // std::cout << key_ << "\n";

  // Store the coords of each letter
  // Store the playfair cihper key map

  //  using Coord2StringMap = std::map<std::pair, std::string>;
  // using String2CoordMap = std::map<std::string, std::pair>;

  //Coord2StringMap positionmap;
  //String2CoordMap lettermap;

  for (int i = 0; i <=25; i++)
    {
      std::pair<int, int> coord{i%5,i/5};
      auto positionItem{ std::make_pair(coord, key_[i])};
      auto letterItem{ std::make_pair(key_[i], coord)};
      positionmap_.insert(positionItem);
      lettermap_.insert(letterItem);
    }  
}
