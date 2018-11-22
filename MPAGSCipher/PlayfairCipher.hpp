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
  /// The cipher key phrase
  std::string key_{'A'};
  
  /// Type definition for the coordinates in the 5x5 table
  using PlayfairCoords = std::pair<std::size_t, std::size_t>;

  /// Type definition for the map relating the coordinates to the key letter
  using Coord2StringMap = std::map<PlayfairCoords, char>;

  /// Type definition for the map relating the key letter to the coordinates
  using String2CoordMap = std::map<char, PlayfairCoords>;

  /// Lookup table to go from the coordinates to the character
  Coord2StringMap positionmap_;

  /// Lookup table to go from the character to the coordinates
  String2CoordMap lettermap_;

};

#endif
