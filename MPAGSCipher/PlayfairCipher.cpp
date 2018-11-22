
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
  //Encrypt or decrypt - set the shift accordingly
  const std::size_t shift { (cipherMode == CipherMode::Encrypt) ? 1u : 4u };

  std::string outputText { inputText };

  // Change J -> I
  auto changeJI = [] (char elem)
    {
      if (elem =='J') {
        return 'I';
      } else {
        return elem;
      }
    };
  std::transform(outputText.begin(), outputText.end(), outputText.begin(), changeJI);

  // If repeated chars in a digraph add an X or  Q if X

  const std::size_t size { outputText.size() };

  // create an empty temporary string
  std::string tmpText{};

  for(std::size_t i{0}; i < size; i+=2)
    {
      // always add the first character in the digraph
      tmpText += outputText[i];
      if ( i+1 == size )
        {
          // there isn't a second character in this digraph
          break;
        }
      else if(outputText[i] != outputText[i+1])
        {
          // the two characters in the digraph are different,
          // add the second one as well
          tmpText += outputText[i+1];
        }
      else
        {
          // the two characters in the digraph are the same
          // add an X (or a Q if the first is an X)
          tmpText += (outputText[i] == 'X') ? 'Q' : 'X';
          // since we did not use the second character in the digraph,
          // it now becomes the first character in the next digraph,
          // so need to decrement i
          --i;
        }
    }

  // assign the temporary back to output
  outputText = tmpText;
  
  // If size of input is odd, add a trailing Z
  // (or add an X if the last character is already a Z)
  if ( (outputText.size() % 2) == 1 )
    {
      outputText += (outputText.back() == 'Z') ? 'X' : 'Z';
    }  

  // Loop over input in Digraphs
  const std::size_t sizeText = outputText.size();
  for(std::size_t i{0}; i < sizeText; i += 2)
    {
      //  - Find the coords in the grid for each digraph
      auto coordIter1 = lettermap_.find(outputText[i]);
      auto coordIter2 = lettermap_.find(outputText[i+1]);

      PlayfairCoords coordinate1 = (*coordIter1).second;
      PlayfairCoords coordinate2 = (*coordIter2).second;

      //  - Apply the rules to these coords to get 'new' coords
	  
      if (coordinate1.first == coordinate2.first)
        { 
          coordinate1.second = (coordinate1.second+shift)%5;
          coordinate2.second = (coordinate2.second+shift)%5;
        }
      else if (coordinate1.second == coordinate2.second)
        {
          coordinate1.first = (coordinate1.first+shift)%5;
          coordinate2.first = (coordinate2.first+shift)%5;
        }
      else
        {
          std::swap(coordinate1.first, coordinate2.first);
        }

      //  - Find the letter associated with the new coords
      auto letter1 = positionmap_.find(coordinate1);
      auto letter2 = positionmap_.find(coordinate2);
      outputText[i]   = (*letter1).second;
      outputText[i+1] = (*letter2).second;
    }

  // Return the text
  return outputText;
}

void PlayFairCipher::setKey(const std::string& key)
{
  // Store the original key
  key_ = key;

  // Append the alphabet
  key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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

  // Store the coords of each letter
  // Store the playfair cihper key map

  for (std::size_t i = 0; i < 25; i++)
    {
      PlayfairCoords coord{i%5,i/5};
      positionmap_.insert( std::make_pair(coord, key_[i]) );
      lettermap_.insert( std::make_pair(key_[i], coord) );
    }  
}
