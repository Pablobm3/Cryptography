#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cmath>

const unsigned int NUM_ROUNDS = 4 + 6;

static const unsigned char SBOX[256] =
{
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static unsigned char* INV_SBOX = NULL;

static const unsigned char RCON[255] =
{
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
  0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
  0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
  0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
  0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
  0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
  0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
  0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb
};


void keyExpansion(const unsigned char* key, unsigned char* roundKey)
{
    unsigned char temp[4], k;

    for (unsigned int i = 0; i < 4; ++i)
    {
        roundKey[i * 4 + 0] = key[i * 4 + 0];
        roundKey[i * 4 + 1] = key[i * 4 + 1];
        roundKey[i * 4 + 2] = key[i * 4 + 2];
        roundKey[i * 4 + 3] = key[i * 4 + 3];
    }

    for (unsigned int i = 4; i < 4 * (NUM_ROUNDS + 1); ++i)
    {
        for (unsigned int j = 0; j != 4; ++j)
            temp[j] = roundKey[(i - 1) * 4 + j];
        if (i % 4 == 0)
        {
            k = SBOX[temp[0]];
            temp[0] = SBOX[temp[1]];
            temp[1] = SBOX[temp[2]];
            temp[2] = SBOX[temp[3]];
            temp[3] = k;

            temp[0] =  temp[0] ^ RCON[i / 4];
        }
        roundKey[i * 4 + 0] = roundKey[(i - 4) * 4 + 0] ^ temp[0];
        roundKey[i * 4 + 1] = roundKey[(i - 4) * 4 + 1] ^ temp[1];
        roundKey[i * 4 + 2] = roundKey[(i - 4) * 4 + 2] ^ temp[2];
        roundKey[i * 4 + 3] = roundKey[(i - 4) * 4 + 3] ^ temp[3];
    }
}


void addRoundKey(unsigned char* state, const unsigned char* roundKey, int round)
{
  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      state[j * 4 + i] ^= roundKey[round * 4 * 4 + i * 4 + j];
  }
}


void subBytes(unsigned char* state)
{
  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      state[i * 4 + j] = SBOX[state[i * 4 + j]];
  }
}


void invSubBytes(unsigned char* state)
{
  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      state[i * 4 + j] = INV_SBOX[state[i * 4 + j]];
  }
}


void shiftRows(unsigned char* state)
{
  unsigned char temp;

  // Rotate first row 1 columns to left
  temp = state[1 * 4 + 0];
  state[1 * 4 + 0] = state[1 * 4 + 1];
  state[1 * 4 + 1] = state[1 * 4 + 2];
  state[1 * 4 + 2] = state[1 * 4 + 3];
  state[1 * 4 + 3] = temp;

  // Rotate second row 2 columns to left
  temp = state[2 * 4 + 0];
  state[2 * 4 + 0] = state[2 * 4 + 2];
  state[2 * 4 + 2] = temp;

  temp = state[2 * 4 + 1];
  state[2 * 4 + 1] = state[2 * 4 + 3];
  state[2 * 4 + 3] = temp;

  // Rotate third row 3 columns to left
  temp = state[3 * 4 + 0];
  state[3 * 4 + 0] = state[3 * 4 + 3];
  state[3 * 4 + 3] = state[3 * 4 + 2];
  state[3 * 4 + 2] = state[3 * 4 + 1];
  state[3 * 4 + 1] = temp;
}


void invShiftRows(unsigned char* state)
{
  unsigned char temp;

  // Rotate first row 1 columns to right
  temp = state[1 * 4 + 3];
  state[1 * 4 + 3] = state[1 * 4 + 2];
  state[1 * 4 + 2] = state[1 * 4 + 1];
  state[1 * 4 + 1] = state[1 * 4 + 0];
  state[1 * 4 + 0] = temp;

  // Rotate second row 2 columns to right
  temp = state[2 * 4 + 0];
  state[2 * 4 + 0] = state[2 * 4 + 2];
  state[2 * 4 + 2] = temp;

  temp = state[2 * 4 + 1];
  state[2 * 4 + 1] = state[2 * 4 + 3];
  state[2 * 4 + 3] = temp;

  // Rotate third row 3 columns to right
  temp = state[3 * 4 + 0];
  state[3 * 4 + 0] = state[3 * 4 + 1];
  state[3 * 4 + 1] = state[3 * 4 + 2];
  state[3 * 4 + 2] = state[3 * 4 + 3];
  state[3 * 4 + 3] = temp;
}


// XTIME is a macro that finds the product of {02} and the argument to XTIME modulo {1b}
#define XTIME(x) (((x) << 1) ^ ((((x) >> 7) & 1) * 0x1b))

// Multiplty is a macro used to multiply numbers in the field GF(2^8)
#define MULTIPLY(x, y) \
( \
  (((y) & 1) * (x)) ^ \
  (((y) >> 1 & 1) * XTIME(x)) ^ \
  (((y) >> 2 & 1) * XTIME(XTIME(x))) ^ \
  (((y) >> 3 & 1) * XTIME(XTIME(XTIME(x)))) ^ \
  (((y) >> 4 & 1) * XTIME(XTIME(XTIME(XTIME(x))))) \
)


void mixColumns(unsigned char* state)
{
  unsigned char Tmp, t;
  for (unsigned int i = 0; i != 4; ++i)
  {
    t = state[0 * 4 + i];
    Tmp = state[0 * 4 + i] ^ state[1 * 4 + i] ^ state[2 * 4 + i] ^ state[3 * 4 + i];
    state[0 * 4 + i] ^= XTIME(state[0 * 4 + i] ^ state[1 * 4 + i]) ^ Tmp;
    state[1 * 4 + i] ^= XTIME(state[1 * 4 + i] ^ state[2 * 4 + i]) ^ Tmp;
    state[2 * 4 + i] ^= XTIME(state[2 * 4 + i] ^ state[3 * 4 + i]) ^ Tmp;
    state[3 * 4 + i] ^= XTIME(state[3 * 4 + i] ^ t) ^ Tmp;
  }
}


void invMixColumns(unsigned char* state)
{
  unsigned char a, b, c, d;
  for (unsigned int i = 0; i != 4; ++i)
  {
    a = state[0 * 4 + i];
    b = state[1 * 4 + i];
    c = state[2 * 4 + i];
    d = state[3 * 4 + i];

    state[0 * 4 + i] = MULTIPLY(a, 0x0e) ^ MULTIPLY(b, 0x0b) ^ MULTIPLY(c, 0x0d) ^ MULTIPLY(d, 0x09);
    state[1 * 4 + i] = MULTIPLY(a, 0x09) ^ MULTIPLY(b, 0x0e) ^ MULTIPLY(c, 0x0b) ^ MULTIPLY(d, 0x0d);
    state[2 * 4 + i] = MULTIPLY(a, 0x0d) ^ MULTIPLY(b, 0x09) ^ MULTIPLY(c, 0x0e) ^ MULTIPLY(d, 0x0b);
    state[3 * 4 + i] = MULTIPLY(a, 0x0b) ^ MULTIPLY(b, 0x0d) ^ MULTIPLY(c, 0x09) ^ MULTIPLY(d, 0x0e);
  }
}


void cipher(const unsigned char* in, const unsigned char* roundKey, unsigned char* out)
{
  unsigned char state[4 * 4];

  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      state[j * 4 + i] = in[i * 4 + j];
  }

  addRoundKey(state, roundKey, 0);
  for (unsigned int round = 1; round < NUM_ROUNDS; ++round)
  {
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey, round);
  }
  subBytes(state);
  shiftRows(state);
  addRoundKey(state, roundKey, NUM_ROUNDS);

  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      out[i * 4 + j] = state[j * 4 + i];
  }
}


void decipher(const unsigned char* in, const unsigned char* roundKey, unsigned char* out)
{
  unsigned char state[4 * 4];

  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      state[j * 4 + i] = in[i * 4 + j];
  }

  addRoundKey(state, roundKey, NUM_ROUNDS);
  for (unsigned int round = NUM_ROUNDS - 1; round > 0; --round)
  {
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, roundKey, round);
    invMixColumns(state);
  }
  invShiftRows(state);
  invSubBytes(state);
  addRoundKey(state, roundKey, 0);

  for (unsigned int i = 0; i != 4; ++i)
  {
    for (unsigned int j = 0; j != 4; ++j)
      out[i * 4 + j] = state[j * 4 + i];
  }
}

unsigned char* calculate_inv_sbox(){
	unsigned char* tmp_inv_sbox = new unsigned char[256];
    for (int i = 0; i < 256; i++) {
        int index = SBOX[i];
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << index; // Use std::hex to format as hexadecimal
        std::string index_str = ss.str();
        int index1 = stoi(index_str.substr(0, 1), nullptr, 16);
        int index2 = stoi(index_str.substr(1, 1), nullptr, 16);
        int new_index = index1*16 + index2;
        std::stringstream ss2;
        ss2 << std::hex << std::setw(1) << (i / 16) << std::hex << std::setw(1) << (i % 16);
        int value = std::stoi(ss2.str(), nullptr, 16); // Convert the hexadecimal string to an integer value
        tmp_inv_sbox[new_index] = value; // Store the integer value in the unsigned char array
        //std::cout << "ss: " << ss.str() << ", index1: " << index1 << ", index2: " << index2 << ", new_index: " << new_index << ", ss2: " << ss2.str() << ", added: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(tmp_inv_sbox[new_index]) << std::endl;
	}
    return tmp_inv_sbox;
}

void init_inv_sbox() {
    if (INV_SBOX == NULL) {
        INV_SBOX = (unsigned char*)malloc(256 * sizeof(unsigned char));
        unsigned char* tmp_inv_sbox = calculate_inv_sbox();
        memcpy(INV_SBOX, tmp_inv_sbox, 256 * sizeof(unsigned char));
        delete[] tmp_inv_sbox;
    }
}

unsigned char* encrypt(const unsigned char* in, const unsigned char* roundKey) {
	unsigned char* out = new unsigned char[32];
	
	std::cout << std::endl << "SBOX:" << std::endl;
	for (int i = 0; i < 256; i++) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(SBOX[i]) << " ";
		if ((i + 1) % 16 == 0) {
			std::cout << std::endl;
		}
	}

    std::cout << std::endl << "Text before encryption:" << std::hex << std::endl;
    for (unsigned int i = 0; i < 32; ++i)
      std::cout << "0x" << std::setw(2) << std::setfill('0') << (unsigned int)in[i] << ", ";
    std::cout << std::endl;

    cipher(in, roundKey, out);

    std::cout << std::endl << "Text after encryption:" << std::hex << std::endl;
    for (unsigned int i = 0; i < 32; ++i)
      std::cout << "0x" << std::setw(2) << std::setfill('0') << (unsigned int)out[i] << ", ";
    std::cout << std::endl;
    
    return out;
}

unsigned char* decrypt(const unsigned char* in, const unsigned char* roundKey) {
	unsigned char* out = new unsigned char[32];
	
	init_inv_sbox();

	/*std::cout << std::endl << "INV_SBOX:" << std::endl;
	for (int i = 0; i < 256; i++) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(INV_SBOX[i]) << " ";
		if ((i + 1) % 16 == 0) {
			std::cout << std::endl;
		}
	}
	*/
    
    decipher(in, roundKey, out);

    /*std::cout << std::endl << "Text after decryption:" << std::hex << std::endl;
    for (unsigned int i = 0; i < 32; ++i)
      std::cout << "0x" << std::setw(2) << std::setfill('0') << (unsigned int)out[i] << ", ";
    std::cout << std::endl;
    */
    
    return out;
}

bool isAlphaNumHex(unsigned char* in, int size) {
  for (int i = 0; i < size; i++) {
    if (!((in[i] >= 0x30 && in[i] <= 0x39) || (in[i] >= 0x61 && in[i] <= 0x66) || (in[i] == 0x2e))) {
      return false;
    }
  }
  return true;
}

int countNonNull(unsigned char* arr, unsigned int size) {
    int count = 0;
    for(unsigned int i = 0; i < size; i++) {
        if(arr[i] != 0) {
            count++;
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
  unsigned char roundKey[240];

  // Sample
  {
	// Part1
	/*const unsigned char in[16] =
    {
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
      'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
    };
    const unsigned char key[16] =
    {
      0xa3, 0x28, 0x4e, 0x09, 0xc6, 0xfe, 0x53, 0x29,
      0x97, 0xef, 0x6d, 0x10, 0x74, 0xc3, 0xde, 0xad
    };
    keyExpansion(key, roundKey);
    decrypt(encrypt(in, roundKey), roundKey);
    */
    
    // Part 2
    const unsigned char ciphertext[32] =
    {
      'b', 'f', '3', 'f', 'b', '7', '7', 'd', '9', '3', 'd', 'd', '6', 'c', 'f', 'd', 'e', 'f', 'b', '8', '8', '2', '2', 'b', '8', '2', 'd', '0', '3', '5', '8', 'a'
    }; 
	
	unsigned char key[32] =
	{
	  '8', '1', '5', '9', '6', 'b', 'f', 'b', '3', '9', 'c', '6', '2', 'b', '7', '1', '6', 'e', '5', '2', 'd', 'b', '9', '1', '8', '1', '\0', '\0', '\0', '\0', '\0', '\0'
	};
	std::cout << std::endl << "The original key:" << std::hex << std::endl;
	for (unsigned int i = 0; i < 32; ++i) {
		std::cout << "0x" << (unsigned int)key[i] << ", ";
	}
	std::cout << std::endl;

	// generate list of possible hexadecimal values
	unsigned int num_possible_hex_values = 128;
	unsigned char possible_hex_values[num_possible_hex_values] = {'\0'};
	int num_valid_hex_values = 0;
	for(unsigned int c = 0; c < num_possible_hex_values; c++) {
		if((c >= 0x30 && c <= 0x39) || (c >= 0x61 && c <= 0x7a) || (c == 0x2e)) {
			possible_hex_values[num_valid_hex_values++] = c;
		}
	}
	num_possible_hex_values = countNonNull(possible_hex_values, 128);
	
	std::cout << std::endl << "Possible values:" << std::hex << std::endl;
	for (unsigned int i = 0; i < sizeof(possible_hex_values); ++i) {
		std::cout << "0x" << (unsigned int)possible_hex_values[i] << ", ";
	}
	std::cout << std::endl;

	for(int i = 26; i < 32; i++) {
		key[i] = possible_hex_values[0];
	}
	
	unsigned char** plaintext_list = new unsigned char*[1000];
	for (int i = 0; i < 1000; i++) {
        plaintext_list[i] = new unsigned char[32];
    }
    for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 32; j++) {
            plaintext_list[i][j] = '0';
        }
    }
    unsigned char** key_list = new unsigned char*[1000];
	for (int i = 0; i < 1000; i++) {
        key_list[i] = new unsigned char[32];
    }
    for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 32; j++) {
            key_list[i][j] = '\0';
        }
    }
	
	
	int index = 0;
	bool done = false; // test to stop at first match
	
	for(unsigned int a = 1; a < num_possible_hex_values; a++) {
		if (done) {
			break;
		}
		for(unsigned int b = 1; b < num_possible_hex_values; b++) {
			if (done) {
				break;
			}
			for(unsigned int c = 1; c < num_possible_hex_values; c++) {
				if (done) {
					break;
				}
				for(unsigned int d = 1; d < num_possible_hex_values; d++) {
					if (done) {
						break;
					}
					for(unsigned int e = 1; e < num_possible_hex_values; e++) {
						if (done) {
							break;
						}
						for(unsigned int f = 0; f < num_possible_hex_values; f++) {
							std::cout << std::endl << "The tested key:" << std::hex << std::endl;
							for (unsigned int i = 0; i < 32; ++i) {
								std::cout << "0x" << (unsigned int)key[i] << ", ";
							}
							std::cout << std::endl;
							
							keyExpansion(key, roundKey);
							unsigned char plaintext[32];
							unsigned char* decrypted = decrypt(ciphertext, roundKey);
							if (isAlphaNumHex(plaintext, 32)) {
								std::cout << std::endl << "###################################################" << std::endl << "Matched text:" << plaintext_list << std::endl << "###################################################" << std::endl ;
								plaintext_list[index] = decrypted;
								key_list[index] = key;
								index++;
								// test to stop at first match
								done = true;
								break;
							}
							key[31] = possible_hex_values[f];
						}
						key[30] = possible_hex_values[e];
					}
					key[29] = possible_hex_values[d];
				}
				key[28] = possible_hex_values[c];
			}
			key[27] = possible_hex_values[b];
		}
		key[26] = possible_hex_values[a];
	}

	
	// ######## TEST ################
	/*
	unsigned char plaintext[32] =
    {
      'b', 'f', '3', 'f', 'b', '7', '7', 'd', '9', '3', 'd', 'd', '6', 'c', 'f', 'd', 'e', 'f', 'b', '8', '8', '2', '2', 'b', '8', '2', 'd', '0', '3', '5', '8', 'a'
    }; 
	if (isAlphaNumHex(plaintext, 32)) {
		std::cout << std::endl << "###################################################" << std::endl << "Matched text:" << plaintext_list << std::endl << "###################################################" << std::endl ;
		plaintext_list[index] = plaintext;
		key_list[index] = key;
		index++;
	}
	*/

	for(int i=0; i<1000; i++) {
		if((key_list[i][0]=='\0')||(plaintext_list[i][0]=='\0')){
			break;
		}
		
		std::cout << std::endl << "Key value:" << std::hex << std::endl;
		for(int j=0; j<32; j++) {
			std::cout << key_list[i][j] << " ";
		}
		
		std::cout << std::endl << "Plaintext:" << std::hex << std::endl;
		for(int j=0; j<32; j++) {

			std::cout << plaintext_list[i][j] << " ";
		}
		std::cout << std::endl;
	}
  }

  return 0;
}

