#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <map>

// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayFairCipher.hpp
 * \brief Contains the declaration of the PlayFairCipher class
 */

/**
 * \class PlayFairCipher
 * \brief Encrypt or decrypt text using the PlayFair cipher with the given key
 */
class PlayFairCipher {
  public:
    /**
     * Create a new PlayFairCipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    explicit PlayFairCipher( const std::string& key );

    /**
     * Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
  std::string applyCipher(const std::string& inputText, const CipherMode cipherMode) const;

  /**
   *Function to set the cipher key
   */
  void setKey(const std::string& key);

  private:
  /** The alphabet - used to determine the cipher character given the plain character and the key
   *
   *\param alphabet the vector containing the alphabet letters
   */
  // const std::vector<char> alphabet_ = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

  /** The size of the alphabet
   *
   *\param alphabetSize the vector containing the size of the alphabet vector
   */
  //const std::vector<char>::size_type alphabetSize_ = alphabet_.size();

  /** The cipher key, essentially a constant shift to be applied
   *
   *\param key_ the sze_t parameter used to store the cipher key
   */
  std::string key_{'A'};
  
  using Coord2StringMap = std::map<std::pair<std::size_t, std::size_t>, char>;
  using String2CoordMap = std::map<char, std::pair<std::size_t, std::size_t>>;

  Coord2StringMap positionmap_;
  String2CoordMap lettermap_;

};

#endif
