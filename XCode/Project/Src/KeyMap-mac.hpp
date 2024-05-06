//
//  KeyMap-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 06/05/2024.
//

#ifndef KeyMap_mac_hpp
#define KeyMap_mac_hpp

#include <stdio.h>

#include <string>

#include "BeebWin.h"

struct KeyMapping {
	int row;    // Beeb row
	int col;    // Beeb col
	bool shift; // Beeb shift state
};

constexpr int KEYMAP_SIZE = 256;

typedef KeyMapping beebEmKeyMap[KEYMAP_SIZE][2]; // Indices are: [Virt key][shift state]

void InitKeyMap();

bool ReadKeyMap(const char *filename, beebEmKeyMap *keymap);
bool WriteKeyMap(const char *filename, beebEmKeyMap *keymap);

const char* GetPCKeyName(int PCKey);

void SetUserKeyMapping(int Row, int Column, bool BBCShift, int PCKey, bool PCShift);
void ClearUserKeyMapping(int Row, int Column, bool Shift);
std::string GetKeysUsed(int Row, int Column, bool Shift);

extern beebEmKeyMap DefaultKeyMap;
extern beebEmKeyMap LogicalKeyMap;
extern beebEmKeyMap UserKeyMap;

extern const beebEmKeyMap *transTable;

#endif /* KeyMap_mac_hpp */
